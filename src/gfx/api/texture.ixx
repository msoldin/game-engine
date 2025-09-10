module;
#include <cstdint>
#include <string>
export module game_engine.gfx.api:texture;

export struct TextureDescription {
  std::string path;
  float width;
  float height;
  float offset_x;
  float offset_y;
};

export class Texture {
 public:
  virtual ~Texture() = default;

  // Optional: dimensions, format, etc.
  virtual float width() const = 0;
  virtual float height() const = 0;
  virtual float offset_x() const = 0;
  virtual float offset_y() const = 0;
};

export class Animation : public Texture {
 public:
  virtual void update(uint64_t const dt) = 0;
};