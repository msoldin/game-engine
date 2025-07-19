#include <benchmark/benchmark.h>
#include <memory>
#include <random>

import vulkan_engine.ecs;

static auto get_random(const int min, const int max) -> float {
  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(min, max);
  return dis(e);
}

struct SimulationComponent final : vulkan_engine::ecs::Component<SimulationComponent> {
  float x{get_random(0, 100)};
  float y{get_random(0, 100)};
  float speedX{get_random(-1, 1)};
  float speedY{get_random(-1, 1)};
  int width = 1;
  int height = 1;
};

struct UselessComponent final : vulkan_engine::ecs::Component<UselessComponent> {
  float i{get_random(0, 100)};
};

struct SimulationEntity final {
  std::optional<SimulationComponent> simulation_component;
  std::optional<UselessComponent> useless_component;
};

static void checkAndHandleCollision(SimulationComponent& obj1, SimulationComponent& obj2) {
  // Check if the two objects are overlapping
  const bool xOverlap = (obj1.x < obj2.x + obj2.width && obj1.x + obj1.width > obj2.x);
  const bool yOverlap = (obj1.y < obj2.y + obj2.height && obj1.y + obj1.height > obj2.y);

  if (xOverlap && yOverlap) {
    // Collision detected, reverse the velocities (bounce effect)
    obj1.speedX = -obj1.speedX;
    obj1.speedY = -obj1.speedY;

    obj2.speedX = -obj2.speedX;
    obj2.speedY = -obj2.speedY;
  }
}

// Function to handle boundary collisions with screen edges (0 to 100)
static void checkAndHandleBoundaryCollision(SimulationComponent& obj) {
  // Screen size is 100x100, so boundaries are from 0 to 100
  // Check if the object goes beyond the right or left edges (X axis)
  if (obj.x < 0) {
    obj.x = 0;                 // Set to the left edge
    obj.speedX = -obj.speedX;  // Reverse the X velocity
  }
  if (obj.x + obj.width > 100) {
    obj.x = 100 - obj.width;   // Set to the right edge
    obj.speedX = -obj.speedX;  // Reverse the X velocity
  }

  // Check if the object goes beyond the top or bottom edges (Y axis)
  if (obj.y < 0) {
    obj.y = 0;                 // Set to the top edge
    obj.speedY = -obj.speedY;  // Reverse the Y velocity
  }
  if (obj.y + obj.height > 100) {
    obj.y = 100 - obj.height;  // Set to the bottom edge
    obj.speedY = -obj.speedY;  // Reverse the Y velocity
  }
}

static void moveComponent(SimulationComponent& obj) {
  obj.x += obj.speedX;
  obj.y += obj.speedY;
}

class SimpleFixture : public benchmark::Fixture {
 public:
  void SetUp(const benchmark::State& state) override {
    for (int i = 0; i < state.range(0); i++) {
      auto entity = new SimulationEntity();
      entity->simulation_component = SimulationComponent{};
      m_entities.push_back(entity);

      entity = new SimulationEntity();
      entity->useless_component = UselessComponent{};
      m_entities.push_back(entity);
    }
  }
  void TearDown(const benchmark::State& state) override {
    for (const auto* component : m_entities) {
      delete component;
    }
    m_entities.clear();
  }
  std::vector<SimulationEntity*> m_entities;
};

BENCHMARK_DEFINE_F(SimpleFixture, CollisionDetection)(benchmark::State& state) {
  for (auto _ : state) {
    for (int frame = 0; frame < 1000; frame++) {
      for (auto* entity : m_entities) {
        if (entity->simulation_component.has_value()) {
          checkAndHandleBoundaryCollision(entity->simulation_component.value());
          for (auto* other_entity : m_entities) {
            if (other_entity->simulation_component.has_value()) {
              checkAndHandleCollision(entity->simulation_component.value(), other_entity->simulation_component.value());
              moveComponent(entity->simulation_component.value());
            }
          }
        }
      }
    }
  }
}

BENCHMARK_REGISTER_F(SimpleFixture, CollisionDetection)->Arg(10)->Arg(100)->Arg(500);

class PhysicSystem final : public vulkan_engine::ecs::System {
 public:
  explicit PhysicSystem(vulkan_engine::ecs::Coordinator& coordinator) : m_coordinator(coordinator) {}

  void preUpdate(uint64_t dt) override {};
  void update(uint64_t dt) override {
    for (const auto& component_manager = m_coordinator.getComponentManager(); auto& component : component_manager.components<SimulationComponent>()) {
      checkAndHandleBoundaryCollision(component);
      for (auto& other_component : component_manager.components<SimulationComponent>()) {
        checkAndHandleCollision(component, other_component);
        moveComponent(component);
      }
    }
  }
  void postUpdate(uint64_t dt) override {};

 private:
  vulkan_engine::ecs::Coordinator& m_coordinator;
};

class EcsFixture : public benchmark::Fixture {
 public:
  void SetUp(const benchmark::State& state) override {
    m_coordinator = vulkan_engine::ecs::create().build();
    m_coordinator->getSystemManager().add_system<PhysicSystem>(*m_coordinator);
    for (int i = 0; i < state.range(0); i++) {
      const vulkan_engine::ecs::Entity* entity = m_coordinator->getEntityManager().createEntity();
      m_coordinator->getComponentManager().addComponent<SimulationComponent>(entity->getId());
      m_coordinator->getComponentManager().addComponent<UselessComponent>(entity->getId());
    }
  }
  std::unique_ptr<vulkan_engine::ecs::Coordinator> m_coordinator;
};

BENCHMARK_DEFINE_F(EcsFixture, CollisionDetection)(benchmark::State& state) {
  for (auto _ : state) {
    for (int frame = 0; frame < 1000; frame++) {
      m_coordinator->update(0);
    }
  }
}

BENCHMARK_REGISTER_F(EcsFixture, CollisionDetection)->Arg(10)->Arg(100)->Arg(500);

BENCHMARK_MAIN();