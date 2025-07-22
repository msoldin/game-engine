module;
export module game_engine.gfx.api:texture;

export class Texture {
 public:
  virtual ~Texture() = default;

  // Optional: dimensions, format, etc.
  virtual int width() const = 0;
  virtual int height() const = 0;
};