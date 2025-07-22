module;
#include <cstdint>
export module game_engine.gfx.api:color;

export struct Color final {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};