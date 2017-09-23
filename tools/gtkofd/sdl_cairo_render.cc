#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cairo/cairo.h>
#include <assert.h>
#include "sdl_cairo_render.h"
//#include "utils/logger.h"

SDLCairoRender::SDLCairoRender(int screenWidth, int screenHeight, int screenBPP):
    m_screenWidth(screenWidth), m_screenHeight(screenHeight),
    m_screenBPP(screenBPP)
{
    init();
}

SDLCairoRender::~SDLCairoRender(){

    if (m_imageSurface != nullptr){
        SDL_FreeSurface(m_imageSurface);
        m_imageSurface = nullptr;
    }
    if (m_screenRenderer != nullptr){
        SDL_DestroyRenderer(m_screenRenderer);
        m_screenRenderer = nullptr;
    }
    if (m_window != nullptr){
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool SDLCairoRender::init(){

    m_window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr){
        //LOG(ERROR) << "SDL_CreateWindow() failed. " << SDL_GetError();
        return false;
    }

    m_screenRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    if ( m_screenRenderer == nullptr ){
        //LOG(ERROR) << "SDL_CreateRenderer() failed. " << SDL_GetError();
        return false;
    }

    return RebuildBackgroundImage(m_screenWidth, m_screenHeight);

}

// -------- create_image_surface() --------
SDL_Surface *create_image_surface(int width, int height, int bpp){
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000; 
    gmask = 0x00ff0000; 
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000; 
    amask = 0xff000000; 
#endif

    SDL_Surface *imageSurface = SDL_CreateRGBSurface(0, width, height, bpp, 
            rmask, gmask, bmask, amask);
    if ( imageSurface == nullptr ){
        //LOG(ERROR) << "SDL_CreateRGBSurface() failed. " << SDL_GetError();
    }

    return imageSurface;
}

bool SDLCairoRender::RebuildBackgroundImage(int screenWidth, int screenHeight){

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    if (m_imageSurface != nullptr) SDL_FreeSurface(m_imageSurface);
    m_imageSurface = create_image_surface(m_screenWidth, m_screenHeight, m_screenBPP);
    if ( m_imageSurface == nullptr ){
        //LOG(ERROR) << "create_image_surface() failed. " << SDL_GetError();
        return false;
    }
    return true;
}


void SDLCairoRender::RenderBackgroundImage(){

    SDL_FillRect(m_imageSurface, NULL, 0xFFFFFF);

    cairo_surface_t *cairoSurface = cairo_image_surface_create_for_data(
            (unsigned char*)m_imageSurface->pixels,
            CAIRO_FORMAT_ARGB32,
            m_imageSurface->w,
            m_imageSurface->h,
            m_imageSurface->pitch
            );
    assert(cairoSurface != nullptr);

    onRender(cairoSurface);

    cairo_surface_destroy(cairoSurface);
}

void SDLCairoRender::UpdateScreenRenderer(){

    assert(m_screenRenderer != nullptr);
    assert(m_imageSurface != nullptr);

    // water mask
    //SDL_Texture *backgroundTexture = SDL_CreateTexture(m_screenRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_screenWidth, m_screenHeight);
    //SDL_UpdateTexture(backgroundTexture, nullptr, m_imageSurface->pixels, m_imageSurface->pitch);

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(m_screenRenderer, m_imageSurface);

    // Clear screen
    SDL_SetRenderDrawColor(m_screenRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_screenRenderer);

    // Render texture to screen
    SDL_Rect destRect{0, 0, m_screenWidth, m_screenHeight};
    SDL_RenderCopy(m_screenRenderer, backgroundTexture, nullptr, &destRect);
    SDL_DestroyTexture(backgroundTexture);
}


void SDLCairoRender::UpdateScreen(){
    SDL_RenderPresent(m_screenRenderer);
}


