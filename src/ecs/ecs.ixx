export module game_engine.ecs;
export import :entity;
export import :system;
export import :component;
export import :coordinator;
export import :system_manager;
export import :component_manager;

namespace game_engine::ecs {
export CoordinatorBuilder create() { return CoordinatorBuilder{}; }
}  // namespace game_engine::ecs