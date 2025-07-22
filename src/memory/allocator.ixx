module;
#include <iostream>
#include <memory>
#include <utility>
export module game_engine.memory:allocator;

namespace game_engine::memory {

export class IAllocator {
 public:
  virtual ~IAllocator() = default;

  template <typename T, typename... Args>
  T* makeNew(Args&&... args) {
    return new (allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
  }

  virtual void* allocate(size_t size, size_t alignment) = 0;
  virtual void free(void* p) = 0;
};

export class Allocator : public IAllocator {
 public:
  explicit Allocator(const size_t memory_size) : Allocator(malloc(memory_size), memory_size) {}

  Allocator(void* memory_start, const size_t memory_size) : m_memory_start(memory_start), m_size(memory_size), m_space_left(memory_size) {}

  ~Allocator() override { ::free(m_memory_start); }

  Allocator(Allocator&&) = delete;

  Allocator(const Allocator&) = delete;

  Allocator& operator=(Allocator&&) = delete;

  Allocator& operator=(const Allocator&) = delete;

  [[nodiscard]] size_t getSize() const { return m_size; }
  [[nodiscard]] size_t getSpaceLeft() const { return m_space_left; }
  [[nodiscard]] size_t getNumAllocations() const { return m_num_allocations; }

 protected:
  void* m_memory_start;
  size_t m_size;
  size_t m_space_left;
  size_t m_num_allocations{0};
};
}  // namespace game_engine::memory
