//
// Created by marvi on 15.11.2024.
//
module;

#include <cstdint>
#include <functional>

import vulkan_engine.memory;

export module vulkan_engine.ecs:system;

namespace vulkan_engine::ecs {
export class System {
 public:
  virtual ~System() = default;

  virtual void preUpdate(uint64_t dt) = 0;

  virtual void update(uint64_t dt) = 0;

  virtual void postUpdate(uint64_t dt) = 0;
};

export class ISystemCreator {
 public:
  virtual ~ISystemCreator() = default;

  virtual size_t getSize() = 0;

  virtual System* create(memory::Allocator* allocator) = 0;
};

export template <typename T>
class SystemCreator final : public ISystemCreator {
 public:
  template <typename... Args>
  explicit SystemCreator(Args&&... args) {
    m_create = [args...](memory::Allocator* allocator) mutable { return allocator->makeNew<T>(args...); };
  }

  ~SystemCreator() override = default;

  size_t getSize() override {
    // Object size+alignment is important for the allocator
    return sizeof(T) + alignof(T);
  }

  System* create(memory::Allocator* allocator) override { return m_create(allocator); }

 private:
  std::function<T*(memory::Allocator*)> m_create;
};
}  // namespace vulkan_engine::ecs