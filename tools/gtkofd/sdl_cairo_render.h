#ifndef __SDL_CAIRO_RENDER_H__
#define __SDL_CAIRO_RENDER_H__

#include <stdlib.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct _cairo_surface;

class SDLCairoRender{
public:
    SDLCairoRender(int screenWidth, int screenHeight, int screenBPP);
    virtual ~SDLCairoRender();

    bool RebuildBackgroundImage(int screenWidth, int screenHeight);
    void RenderBackgroundImage();
    void UpdateScreenRenderer(); 
    void UpdateScreen();

protected:
    virtual void onRender(_cairo_surface *surface) = 0;

private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_screenRenderer = nullptr;
    SDL_Surface *m_imageSurface = nullptr;
    int m_screenWidth = 0;
    int m_screenHeight = 0;
    int m_screenBPP = 32;

private:
    bool init();

}; // class SDLCairoRender

#endif // __SDL_CAIRO_RENDER_H__
