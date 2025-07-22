module;
export module game_engine.input;

export import game_engine.input.api;

#ifdef USE_SDL
export import game_engine.input.sdl;
#endif