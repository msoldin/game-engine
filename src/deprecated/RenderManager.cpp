//
// Created by marvi on 11.03.2024.
//
#include "deprecated/RenderManager.h"

#include <cstdio>
#include <SDL_image.h>
#include <iostream>

namespace Engine {
    RenderManager::RenderManager() : m_window(nullptr), m_renderer(nullptr), m_clearColor({173, 216, 230, 0}) {
        //Initialize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf("SDL Initialization Error: %s\n", SDL_GetError());
        }

        //Create the Window
        m_window = SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        if (m_window == nullptr) {
            printf("Window Creation Error: %s\n", SDL_GetError());
        }

        //Creating the renderer
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

        //Handling with the renderer creation errors
        if (m_renderer == nullptr) {
            printf("Renderer Creation Error: %s\n", SDL_GetError());
        }

        //Setting the renderer's clear color to white
        SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        //Initializing the SDL_image library and handling initialization errors
        const int flags = IMG_INIT_PNG;
        if (!(IMG_Init(flags) & flags)) {
            printf("IMG Initialization Error: %s\n", IMG_GetError());
        }
    }

    RenderManager::~RenderManager() {
        SDL_DestroyWindow(m_window);
        SDL_DestroyRenderer(m_renderer);

        IMG_Quit();
        SDL_Quit();
    }

    void RenderManager::render() const {
        SDL_RenderPresent(m_renderer);
    }

    void RenderManager::clearBackBuffer() const {
        SDL_RenderClear(m_renderer);
    }

    void RenderManager::setRenderDrawColor(const SDL_Color &color) const {
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    }

    void RenderManager::setRenderScaleToViewSize(float scaleX, float scaleY) const {
        setRenderScale(SCREEN_WIDTH / scaleX, SCREEN_HEIGHT / scaleY);
    }

    void RenderManager::setRenderScale(float scaleX, float scaleY) const {
        SDL_RenderSetScale(m_renderer, scaleX, scaleY);
    }

    void RenderManager::drawRectangle(const SDL_Rect *rect) const {
        drawRectangle(rect, {0, 0, 0, 255});
    }

    void RenderManager::drawRectangle(const SDL_Rect *rect, const SDL_Color &color) const {
        setRenderDrawColor(color);
        SDL_RenderDrawRect(m_renderer, rect);
        //Set Color back to white
        setRenderDrawColor(m_clearColor);
    }

    void RenderManager::drawFilledRectangle(const SDL_Rect *rect, const SDL_Color &color) const {
        setRenderDrawColor(color);
        SDL_RenderFillRect(m_renderer, rect);
        //Set Color back to white
        setRenderDrawColor(m_clearColor);
    }

    void RenderManager::drawTexture(SDL_Texture *tex, const SDL_Rect *clip, const SDL_Rect *rend, const double angle,
                                    const SDL_RendererFlip flip) const {
        SDL_RenderCopyEx(m_renderer, tex, clip, rend, angle, nullptr, flip);
    }

    int RenderManager::getScreenWidth() const {
        return SCREEN_WIDTH;
    }


    int RenderManager::getScreenHeight() const {
        return SCREEN_HEIGHT;
    }
}
