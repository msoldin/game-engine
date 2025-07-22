module;
#include <unistd.h>

#include <cassert>
#include <memory>
#include <stdexcept>
export module game_engine.memory:pool_allocator;

import :allocator;
import :pointer_math;

namespace game_engine::memory {

export class PoolAllocator final : public Allocator {
 public:
  template <typename T>
  static std::unique_ptr<PoolAllocator> create(const size_t memorySize) {
    return std::make_unique<PoolAllocator>(sizeof(T) * memorySize, sizeof(T), alignof(T));
  }
  PoolAllocator(const size_t memorySize, const size_t objectSize, const size_t objectAlignment)
      : Allocator(memorySize), m_object_size(objectSize), m_object_alignment(objectAlignment) {
    const size_t adjustment = pointer_math::alignForwardAdjustment(m_memory_start, objectAlignment);
    m_free_list = static_cast<void**>(pointer_math::add(m_memory_start, adjustment));
    const size_t num_blocks = (memorySize - adjustment) / objectSize;
    void** p = m_free_list;

    for (size_t i = 0; i < num_blocks - 1; i++) {
      *p = pointer_math::add(p, objectSize);  // NOLINT(*-multi-level-implicit-pointer-conversion)
      p = static_cast<void**>(*p);
    }

    *p = nullptr;
  }

  void* allocate(const size_t size, const size_t alignment) override {
    assert(size == m_object_size && alignment == m_object_alignment && "PoolAllocator can only allocate objects of the same size and alignment.");

    if (this->m_free_list == nullptr) {
      throw std::out_of_range("PoolAllocator overflow");
    }

    void* result = this->m_free_list;  // NOLINT(*-multi-level-implicit-pointer-conversion)

    m_free_list = static_cast<void**>(*this->m_free_list);
    m_space_left -= size;
    m_num_allocations++;

    return result;
  }

  void free(void* p) override {
    *static_cast<void**>(p) = m_free_list;  // NOLINT(*-multi-level-implicit-pointer-conversion)
    m_free_list = static_cast<void**>(p);
    m_space_left += m_object_size;
    m_num_allocations--;
  }

 private:
  void** m_free_list;
  size_t m_object_size;
  size_t m_object_alignment;
};
}  // namespace game_engine::memory