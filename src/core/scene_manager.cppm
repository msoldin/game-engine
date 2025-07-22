module;
#include <memory>
#include <vector>
export module game_engine.core:scene_manager;

import :scene;

namespace game_engine::core {
export class SceneManager {
 public:
  void update(const uint64_t dt) {
    m_inactive_scenes.clear();
    if (!m_active_scenes.empty()) {
      m_active_scenes.back()->update(dt);
    }
  }

  void enter_new_scene(const std::shared_ptr<Scene>& scene) {
    if (!m_active_scenes.empty()) {
      m_active_scenes.back()->obscuring();
    }
    m_active_scenes.push_back(scene);
    m_active_scenes.back()->entered();
  }

  void leave_current_scene() {
    if (!m_active_scenes.empty()) {
      const std::shared_ptr<Scene>& scene = m_active_scenes.back();
      scene->leaving();
      m_active_scenes.pop_back();

      m_inactive_scenes.push_back(scene);

      if (!m_active_scenes.empty()) {
        m_active_scenes.back()->revealed();
      }
    }
  }

  void replace_current_scene(const std::shared_ptr<Scene>& scene) {
    leave_current_scene();
    enter_new_scene(scene);
  }

  Scene& get_current_scene() const { return *m_active_scenes.back(); }

 private:
  std::vector<std::shared_ptr<Scene>> m_active_scenes;
  std::vector<std::shared_ptr<Scene>> m_inactive_scenes;
};

std::shared_ptr<SceneManager> create_scene_manager() { return std::make_shared<SceneManager>(); }
}  // namespace game_engine::core
