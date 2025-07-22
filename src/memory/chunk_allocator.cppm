module;
#include <unistd.h>

#include <cassert>
#include <memory>
#include <stack>
export module game_engine.memory:chunk_allocator;

import :pool_allocator;

namespace game_engine::memory {

export template <typename T>
class ChunkAllocator final : public IAllocator {
public:
  explicit ChunkAllocator(const size_t chunk_size) : m_chunk_size(chunk_size) { m_chunks.emplace(PoolAllocator::create<T>(chunk_size)); }

  void* allocate(const size_t size, const size_t alignment) override {
    if (m_chunks.top()->getSpaceLeft() >= size + alignment) {
      return m_chunks.top()->allocate(size, alignment);
    }
    m_chunks.emplace(PoolAllocator::create<T>(m_chunk_size));
    return m_chunks.top()->allocate(size, alignment);
  }
  void free(void* p) override {
    if (m_chunks.top()->getSpaceLeft() == m_chunks.top()->getSize() && m_chunks.size() > 1) {
      m_chunks.pop();
    }
    m_chunks.top()->free(p);
  }

 private:
  size_t m_chunk_size;
  std::stack<std::unique_ptr<PoolAllocator>> m_chunks;
};

}  // namespace game_engine::memory