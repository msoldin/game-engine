//
// Created by marvi on 04.05.2024.
//
module;
#include <memory>
#include <unordered_map>
#include <vector>
import vulkan_engine.memory;
export module vulkan_engine.ecs:component_manager;

import :component;

namespace vulkan_engine::ecs {

class IComponentHandle {
 public:
  virtual ~IComponentHandle() = default;
};

template <typename T>
class ComponentHandle final : public IComponentHandle {
 public:
  ~ComponentHandle() override = default;
  template <typename... Args>
  T* createComponent(Args... args) {
    T* component = m_allocator->makeNew<T>(std::forward<Args>(args)...);
    m_components.push_back(component);
    return component;
  }

  auto begin() { return m_components.begin(); }
  auto end() { return m_components.end(); }

 private:
  std::vector<IComponent*> m_components;
  std::unique_ptr<memory::PoolAllocator> m_allocator = std::make_unique<memory::PoolAllocator>(T::getMaxComponents() * sizeof(T), sizeof(T), alignof(T));
};

template <typename Iterator>
class ComponentIterator {
 public:
  ComponentIterator(Iterator begin, Iterator end) : m_begin(begin), m_end(end) {}

  Iterator begin() const { return m_begin; }
  Iterator end() const { return m_end; }

 private:
  Iterator m_begin;
  Iterator m_end;
};

export class ComponentManager {
 public:
  ComponentManager() = default;

  ~ComponentManager() = default;

  template <class T, class... Args>
  void addComponent(size_t entity_id, Args... args) {
    static_assert(std::is_base_of_v<IComponent, T>, "T must be a subclass of Component.");
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    if (const auto& it = m_component_lookup.find(component_type_id); it == m_component_lookup.end()) {
      m_component_lookup[component_type_id] = std::make_unique<ComponentHandle<T>>();
    }
    auto* component_handle = dynamic_cast<ComponentHandle<T>*>(m_component_lookup[component_type_id].get());
    auto* component = component_handle->createComponent(std::forward<Args>(args)...);
    component->m_entity_id = entity_id;
  }

  template <typename T>
  auto components() {
    static_assert(std::is_base_of_v<IComponent, T>, "T must be a subclass of Component.");
    auto* handle = getComponentHandle<T>();
    return ComponentIterator{handle->begin(), handle->end()};
  }

 private:
  template <typename T>
  ComponentHandle<T>* getComponentHandle() {
    static_assert(std::is_base_of_v<IComponent, T>, "T must be a subclass of Component.");
    const ComponentTypeId component_type_id = T::getComponentTypeId();
    const auto& it = m_component_lookup.find(component_type_id);
    if (it == m_component_lookup.end()) {
      throw std::runtime_error("Component type not found");
    }
    return dynamic_cast<ComponentHandle<T>*>(it->second.get());
  }
  std::unordered_map<ComponentTypeId, std::unique_ptr<IComponentHandle>> m_component_lookup;
};
}  // namespace vulkan_engine::ecs