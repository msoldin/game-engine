//
// Created by marvi on 11.03.2024.
//
#pragma once

#include <SDL.h>

namespace Engine {
    class Engine;

    class RenderManager final {
        friend class Engine;

    private:
        const int SCREEN_WIDTH = 1280;
        const int SCREEN_HEIGHT = 720;
        const char *WINDOW_TITLE = "Mario Bros";
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Color m_clearColor;

    public:
        void render() const;

        void clearBackBuffer() const;

        void setRenderScale(float scaleX, float scaleY) const;

        void setRenderScaleToViewSize(float scaleX, float scaleY) const;

        //Getter
        int getScreenWidth() const;

        int getScreenHeight() const;

        //Draw Methods
        void drawRectangle(const SDL_Rect *rect) const;

        void drawRectangle(const SDL_Rect *rect, const SDL_Color &color) const;

        void drawFilledRectangle(const SDL_Rect *rect, const SDL_Color &color) const;

        void drawTexture(SDL_Texture *tex, const SDL_Rect *clip, const SDL_Rect *rend, const double angle,
                         const SDL_RendererFlip flip) const;

    private:
        RenderManager();

        ~RenderManager();

        void setRenderDrawColor(const SDL_Color &color) const;
    };
}
