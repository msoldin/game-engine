module;
#include <format>
#include <iostream>
#include <sstream>
#include <stdexcept>
export module vulkan_engine.error;

namespace vulkan_engine::error {
export enum class ErrorType {
  kEcsError,
};

export class Error final : public std::runtime_error {
 public:
  //explicit Error(const ErrorType error_type, const std::string& message) : std::runtime_error(message), m_error_type(error_type) {}

  template <typename... Args>
  explicit Error(const ErrorType error_type, const std::string& message, Args&&... args)
      : std::runtime_error(std::vformat(message, std::make_format_args(args...))), m_error_type(error_type) {}

  const char* what() const noexcept override {
    static std::string full_message;
    std::ostringstream oss;
    oss << "Type: ";

    switch (m_error_type) {
      case ErrorType::kEcsError:
        oss << "EcsError";
        break;
      default:
        oss << "Unknown";
        break;
    }

    oss << " - " << std::runtime_error::what();
    full_message = oss.str();
    return full_message.c_str();
  }

 private:
  ErrorType m_error_type;
};
}  // namespace vulkan_engine::error