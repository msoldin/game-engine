//
// Created by marvi on 10.05.2024.
//

#ifndef COORDINATOR_BUILDER_HPP
#define COORDINATOR_BUILDER_HPP

#include <glog/logging.h>

#include <memory>

#include "coordinator.h"
#include "system_builder.inl"

namespace vulkan_engine::ecs {
class CoordinatorBuilder final {
public:
  CoordinatorBuilder() = default;

  ~CoordinatorBuilder() = default;

  CoordinatorBuilder(CoordinatorBuilder&&) = delete;

  CoordinatorBuilder(const CoordinatorBuilder&) = delete;

  CoordinatorBuilder& operator=(CoordinatorBuilder&&) = delete;

  CoordinatorBuilder& operator=(const CoordinatorBuilder&) = delete;

  void registerSystem(std::unique_ptr<ISystemCreator> systemCreator) {
    m_systemCreators.push_back(std::move(systemCreator));
  }

  std::unique_ptr<Coordinator> build() {
    return std::unique_ptr<Coordinator>(new Coordinator(m_systemCreators));
  }

private:
  std::vector<std::unique_ptr<ISystemCreator> > m_systemCreators;
};
} // namespace vulkan_engine::ecs


#endif //COORDINATOR_BUILDER_HPP