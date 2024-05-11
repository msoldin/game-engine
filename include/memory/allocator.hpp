//
// Created by marvi on 04.05.2024.
//

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP
#include <memory>
#include <utility>

namespace vulkan_engine::memory {

class Allocator {

public:
  explicit Allocator(const size_t memorySize)
    : m_memoryStart(malloc(memorySize)), m_size(memorySize), m_spaceLeft(memorySize), m_numAllocations(0) {}

  virtual ~Allocator() { free(m_memoryStart); }

  Allocator(Allocator&&) = delete;

  Allocator(const Allocator&) = delete;

  Allocator& operator=(Allocator&&) = delete;

  Allocator& operator=(const Allocator&) = delete;

  template <typename T, typename... Args>
  T* makeNew(Args&&... args) {
    return new(allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
  }

  size_t getSize() const { return m_size; }
  size_t getSpaceLeft() const { return m_spaceLeft; }
  size_t getNumAllocations() const { return m_numAllocations; }

private:
  virtual void* allocate(size_t size, size_t alignment) = 0;

  virtual void deallocate(void* p) = 0;

protected:
  void* m_memoryStart;
  size_t m_size;
  size_t m_spaceLeft;
  size_t m_numAllocations;
};

} // namespace vulkan_engine::memory

#endif  //ALLOCATOR_HPP