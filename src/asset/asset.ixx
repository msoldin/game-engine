module;
export module game_engine.asset;

export import game_engine.asset.api;

#ifdef USE_SDL
export import game_engine.asset.sdl;
#endif