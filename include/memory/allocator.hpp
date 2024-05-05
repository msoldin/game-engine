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
  Allocator(size_t memorySize, void* memoryStart)
      : m_size(memorySize),
        m_start(memoryStart),
        m_spaceLeft(memorySize),
        m_numAllocations(0) {}
  virtual ~Allocator() = default;
  Allocator(Allocator&&) = delete;
  Allocator(const Allocator&) = delete;
  Allocator& operator=(Allocator&&) = delete;
  Allocator& operator=(const Allocator&) = delete;

  template <typename T, typename... Args>
  T* makeNew(Args&&... args) {
    return new (allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
  }

  void free(void* p) { deallocate(p); }

  size_t getSize() const { return m_size; }
  size_t getSpaceLeft() const { return m_spaceLeft; }
  size_t getNumAllocations() const { return m_numAllocations; }

 private:
  virtual void* allocate(size_t size, size_t alignment) = 0;
  virtual void deallocate(void* p) = 0;

 protected:
  void* m_start;
  size_t m_size;
  size_t m_spaceLeft;
  size_t m_numAllocations;
};

}  // namespace vulkan_engine::memory

#endif  //ALLOCATOR_HPP
