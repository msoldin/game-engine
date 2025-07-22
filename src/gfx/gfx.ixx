module;
export module game_engine.gfx;

export import game_engine.gfx.api;

#ifdef USE_SDL
export import game_engine.gfx.sdl;
#endif