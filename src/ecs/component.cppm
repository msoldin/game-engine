module;
#include <concepts>
#include <cstddef>
export module game_engine.ecs:component;

namespace game_engine::ecs {

using ComponentTypeId = std::size_t;

namespace {
ComponentTypeId generateComponentTypeId() {
  static ComponentTypeId last_id = 0;
  return last_id++;
}
}  // namespace

class IComponent {
  template <typename T>
  friend class ComponentRegistry;

 public:
  virtual ~IComponent() = default;
  size_t getEntityId() const { return m_entity_id; }

 private:
  size_t m_entity_id = 0;
};

export template <typename T>
class Component : public IComponent {
 public:
  static ComponentTypeId getComponentTypeId() {
    // static only get initialized once, the first time this function is called
    // each template type will have its own static variable
    static ComponentTypeId const kTypeId = generateComponentTypeId();
    return kTypeId;
  }
};

template <typename T>
concept IsComponent = std::derived_from<T, IComponent>;
}  // namespace game_engine::ecs