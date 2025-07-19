module;
#include <algorithm>  // For std::erase_if
#include <memory>
#include <typeindex>
#include <vector>
#include <unordered_map>
export module vulkan_engine.ecs:system_manager;

import :system;
import :component_manager;
import :entity_manager;

import vulkan_engine.memory;

namespace vulkan_engine::ecs {

export class SystemManager final {
 public:
  explicit SystemManager() {}

  SystemManager(SystemManager&&) = delete;

  SystemManager(const SystemManager&) = delete;

  SystemManager& operator=(SystemManager&&) = delete;

  SystemManager& operator=(const SystemManager&) = delete;

  void update(const uint64_t dt) const {
    for (const auto& system : m_systems) {
      system->preUpdate(dt);
    }
    for (const auto& system : m_systems) {
      system->update(dt);
    }
    for (const auto& system : m_systems) {
      system->postUpdate(dt);
    }
  }

  template <class T, typename... Args>
  SystemManager& add_system(Args&&... args) {
    m_systems.push_back(std::make_unique<T>(args...));
    sort_systems();
    return *this;
  }

  template <class T>
  void remove_system() {
    std::erase_if(m_systems, [](const std::unique_ptr<System>& system) { return dynamic_cast<T*>(system.get()) != nullptr; });
  }

  template <class... Systems>
  void specifyOrder() {
    int priority_start = 0;
    (register_system_priority<Systems>(priority_start++), ...);
    sort_systems();
  }

 private:
  std::vector<std::unique_ptr<System>> m_systems;
  std::unordered_map<std::type_index, size_t> m_system_priorities;

  template <typename T>
  void register_system_priority(const int priority) {
    m_system_priorities[typeid(T)] = priority;
  }

  void sort_systems() {
    std::ranges::stable_sort(m_systems, [this](const std::unique_ptr<System>& a, const std::unique_ptr<System>& b) {
      const int priority_a = m_system_priorities.contains(typeid(*a)) ? m_system_priorities[typeid(*a)] : std::numeric_limits<int>::max();
      const int priority_b = m_system_priorities.contains(typeid(*b)) ? m_system_priorities[typeid(*b)] : std::numeric_limits<int>::max();
      return priority_a < priority_b;
    });
  }
};

}  // namespace vulkan_engine::ecs