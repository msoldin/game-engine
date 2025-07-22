module;
#include <cstddef>
#include <optional>
#include <utility>
export module game_engine.ecs:entity;

import :component;
import :component_manager;

namespace game_engine::ecs {
export class Entity final {
  friend class EntityManager;
  ComponentManager* m_component_manager = nullptr;

 protected:
  std::size_t m_entity_id = 0;

 public:
  size_t getId() const { return m_entity_id; }
  template <IsComponent T, class... Args>
  void addComponent(Args... args) {
    m_component_manager->addComponent<T>(m_entity_id, std::forward<T>(args)...);
  }
  template <IsComponent T>
  void removeComponent() const {
    m_component_manager->removeComponent<T>(m_entity_id);
  }
  template <IsComponent T>
  std::optional<std::reference_wrapper<T>> getComponent() {
    return m_component_manager->getComponent<T>(m_entity_id);
  }
  template <IsComponent... Ts>
  auto getComponents() const {
    return m_component_manager->getComponents<Ts...>(m_entity_id);
  }
};
}  // namespace game_engine::ecs