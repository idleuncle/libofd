// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_user.h"
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <vector>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cairo/cairo.h>
#include "utils/logger.h"

template <typename T, int count> int lengthOf(const T (&)[count]){
    return count;
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
        LOG_ERROR("SDL_CreateRGBSurface() failed. %s", SDL_GetError());
    }

    return imageSurface;
}

#include "delegate.h"
struct Action{
    Action(const char *label, const char *name);
    Action(const char *label, const char *name, int shortcut0, int shortcut1, int shortcut2);
    bool isActive();
    bool isRequested();
    bool toolbarButton();
    void getIconPath(char *path, int max_size);

    static bool falseConst() { return false; };

    int shortcut[3];
    const char *name;
    const char *label;
    bool is_global;
    ImTextureID icon;
    utils::Delegate<void> func;
    utils::Delegate<bool> is_selected;
}; // struct Action

Action::Action(const char *label, const char *name){
    this->name = name;
    this->label = label;
    shortcut[0] = shortcut[1] = shortcut[2] = -1;
    is_global = true;
    icon = nullptr;
    is_selected.bind<falseConst>();
}

Action::Action(const char *label, const char *name, int shortcut0, int shortcut1, int shortcut2){
    this->name = name;
    this->label = label;
    shortcut[0] = shortcut0;
    shortcut[1] = shortcut1;
    shortcut[2] = shortcut2;
    is_global = true;
    icon = nullptr;
    is_selected.bind<falseConst>();
}

bool Action::toolbarButton(){
    if (!icon) return false;

    ImVec4 col_active = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    ImVec4 bg_color = is_selected.invoke() ? col_active : ImVec4(0, 0, 0, 0);
    if (ImGui::ToolbarButton(icon, bg_color, label)){
        func.invoke();
        return true;
    }
    return false;
}

void Action::getIconPath(char *path, int max_size){
}

bool Action::isRequested(){
    if (ImGui::IsAnyItemActive()) return false;

    bool *keysDown = ImGui::GetIO().KeysDown;
    float *keysDownDuration = ImGui::GetIO().KeysDownDuration;
    if (shortcut[0] == -1) return false;

    for (int i = 0 ; i < lengthOf(shortcut) + 1 ; ++i){
        if ( i == lengthOf(shortcut) || shortcut[i] == -1 ){
            return true;
        }
        
        if (!keysDown[shortcut[i]] || keysDownDuration[shortcut[i]] > 0) return false;
    }
    return false;
}

bool Action::isActive(){
    if (ImGui::IsAnyItemActive()) return false;
    if (shortcut[0] == -1) return false;

    int key_count;
    auto *state = SDL_GetKeyboardState(&key_count);

    for (int i = 0 ; i < lengthOf(shortcut) + 1 ; ++i){
        if ( i == lengthOf(shortcut) || shortcut[i] == -1 ){
            return true;
        }
        
        if (shortcut[i] >= key_count || !state[shortcut[i]]) return false;
    }
    return false;
}


class ImGuiApp{
public:
    virtual ~ImGuiApp(){};

    static ImGuiApp* Create();
    static void Destroy(ImGuiApp& app);

    virtual void OnRender(cairo_surface_t *surface){};
    virtual Action* GetAction(const char *name) const = 0;
    virtual void update() = 0;
    virtual void run() = 0;
}; // class ImGuiApp

class ImOFDApp : public ImGuiApp {
public:
    ImOFDApp(double screenWidth, double screenHeight, int screenBPP);
    virtual ~ImOFDApp();

    // -------- Actions --------
    void FileNew(){
    }

    void FileOpen(){
    }

    void FileSave(){
    }

    void FileSaveAs(){
    }

    void FileExit(){
        m_finished = true;
    }

private:
    bool init(double screenWidth, double screenHeight, int screenBPP);
    bool initSDL(double screenWidth, double screenHeight, int screenBPP);
    bool initImGui();
    float showMainMenu();
    float showMainToolbar(float menu_height);

    void fileMenu();
    void viewMenu();
    void toolsMenu();

    void doMenuItem(Action& a, bool enabled);

public:

    virtual void OnRender(cairo_surface_t *surface) override;
    virtual Action* GetAction(const char *name) const override;
    void processSystemEvents();
    virtual void update() final;
    virtual void run() final;

private:
    bool m_finished = false;
    std::vector<Action*> m_actions;
    std::vector<Action*> m_toolbar_actions;
    std::vector<Action*> m_window_actions;
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_screenRenderer = nullptr;
    SDL_GLContext m_glContext = 0;
    ImFont *m_font = nullptr;

    void addActions();
    void addAction(Action *action);

    template<void (ImOFDApp::*func)()>
        Action& addAction(const char *label, const char *name){
            auto *a = new Action(label, name);
            a->func.bind<ImOFDApp, func>(this);
            addAction(a);
            return *a;
        }

    template<void (ImOFDApp::*func)()>
        void addAction(const char *label, const char *name, int shortcut0, int shortcut1, int shortcut2){
            auto *a = new Action(label, name, shortcut0, shortcut1, shortcut2);
            a->func.bind<ImOFDApp, func>(this);
            addAction(a);
        }

    void guiBeginFrame();
    void guiEndFrame();
    void renderScreen() const;
    void renderCairo();
    void refreshScreen(uint64_t color) const;

    bool m_is_entity_list_opened = true;
    void onEntityListGUI();
    void toggleEntityList() {m_is_entity_list_opened = !m_is_entity_list_opened;};

    bool m_is_pack_data_dialog_opened = false;
    void onPackDataGUI();
    void showPackDataDialog();

    // -------- SDL Cairo Render --------
    SDL_Surface *m_imageSurface = nullptr;
    double m_screenWidth = 0.0;
    double m_screenHeight = 0.0;
    int m_screenBPP = 32;

    // -------- Window Events --------
    bool m_mousePressed[3] = { false, false, false };
    float m_mouseWheel = 0.0f;

    double m_time = 0.0f;

}; // class ImOFDApp

//static size_t alignMask(size_t _value, size_t _mask){
    //return (_value + _mask) & ((~0) & (~_mask));
//}

//static void *alignPtr(void *_ptr, size_t _align){
    //union { void *ptr; size_t addr; } un;
    //un.ptr = _ptr;
    //size_t mask = _align - 1;
    //size_t aligned = alignMask(un.addr, mask);
    //un.addr = aligned;
    //return un.ptr;
//}

//struct NewPlaceholder {};
//inline void* operator new(size_t, NewPlaceholder, void *where){ return where; };
//inline void operator delete(void*, NewPlaceholder, void*){};
//#define NEW(allocator, ...) new (NewPlaceholder(), (allocator).allocate_aligned(sizeof(__VA_ARGS__), ALIGN_OF(__VA_ARGS__))) __VA_ARGS__
//#define DELETE(allocator, var) (allocator).deleteObject(var);

//#define ALIGN_OF(...) __alignof(__VA_ARGS__)

ImGuiApp* ImGuiApp::Create(){
    //static char buf[sizeof(ImGuiApp) * 2];
    //return new (NewPlaceholder(), alignPtr(buf, ALIGN_OF(ImOFDApp))) ImOFDApp;
    int screenWidth = 794;
    int screenHeight = 1122;
    int screenBPP = 32;
    return new ImOFDApp(screenWidth, screenHeight, screenBPP);
}

void ImGuiApp::Destroy(ImGuiApp& app){
    app.~ImGuiApp();
}

ImOFDApp::ImOFDApp(double screenWidth, double screenHeight, int screenBPP)//:
    //m_finished(false),
    //m_font(nullptr),
    //m_is_entity_list_opened(true),
    //m_is_pack_data_dialog_opened(false)
{
    init(screenWidth, screenHeight, screenBPP);
}

bool ImOFDApp::init(double screenWidth, double screenHeight, int screenBPP){

    initImGui();
    initSDL(screenWidth, screenHeight, screenBPP);

    addActions();

    return true;
}

ImOFDApp::~ImOFDApp(){

    if ( m_imageSurface != nullptr ){
        SDL_FreeSurface(m_imageSurface);
        m_imageSurface = nullptr;
    }

    if ( m_screenRenderer != nullptr ){
        SDL_DestroyRenderer(m_screenRenderer);
        m_screenRenderer = nullptr;
    }

    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void ImOFDApp::addAction(Action *action){
    m_actions.push_back(action);
}

Action* ImOFDApp::GetAction(const char *name) const{
    for ( auto *a : m_actions ){
        if ( strcmp(a->name, name) == 0 ) return a;
    }
    return nullptr;
}

void ImOFDApp::addActions(){
    addAction<&ImOFDApp::FileNew>("New", "new");
    addAction<&ImOFDApp::FileOpen>("Open", "open");
    addAction<&ImOFDApp::FileSave>("Save", "save");
    addAction<&ImOFDApp::FileSaveAs>("SaveAs", "saveAs");
    addAction<&ImOFDApp::FileExit>("Exit", "exit");

    addAction<&ImOFDApp::toggleEntityList>("Entity List", "entityList");
    addAction<&ImOFDApp::showPackDataDialog>("Pack data", "pack_data");
}

static void getShortcut(const Action& action, char *buf, int max_size){
    return;
}

void ImOFDApp::doMenuItem(Action& a, bool enabled){
    char buf[20];
    getShortcut(a, buf, sizeof(buf));
    if (ImGui::MenuItem(a.label, buf, a.is_selected.invoke(), enabled)){
        a.func.invoke();
    }
}

void ImOFDApp::fileMenu(){
    if (!ImGui::BeginMenu("File")) return;

    doMenuItem(*GetAction("new"), true);
    doMenuItem(*GetAction("open"), true);
    doMenuItem(*GetAction("save"), false);
    doMenuItem(*GetAction("saveAs"), false);
    doMenuItem(*GetAction("exit"), true);

    ImGui::EndMenu();
}

void ImOFDApp::viewMenu(){
    if (!ImGui::BeginMenu("View")) return;

    doMenuItem(*GetAction("entityList"), true);

    ImGui::Separator();


    ImGui::EndMenu();
}

void ImOFDApp::toolsMenu(){
    if (!ImGui::BeginMenu("Tools")) return;

    doMenuItem(*GetAction("pack_data"), true);

    ImGui::EndMenu();
}

float ImOFDApp::showMainMenu(){

    float menu_height = 0.0;

    if (ImGui::BeginMainMenuBar()){
        fileMenu();
        viewMenu();
        toolsMenu();

        menu_height = ImGui::GetWindowSize().y;
        ImGui::EndMainMenuBar();
    }

    return menu_height;
}

float ImOFDApp::showMainToolbar(float menu_height){
    if (m_toolbar_actions.empty()) return menu_height;

    auto frame_padding = ImGui::GetStyle().FramePadding;
    float padding = frame_padding.y * 2;
    //ImVec4 active_color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    //ImVec4 inactive_color(0, 0, 0, 0);
    ImVec2 toolbar_size(ImGui::GetIO().DisplaySize.x, 24 + padding);
    if (ImGui::BeginToolbar("main_toolbar", ImVec2(1, menu_height), toolbar_size)){
        //auto& render_interface = getRenderInterface();
        for (auto *action : m_toolbar_actions){
            action->toolbarButton();
        }
    }
    ImGui::EndToolbar();
    return menu_height + 24 + padding;

    return menu_height;
}


bool ImOFDApp::initSDL(double screenWidth, double screenHeight, int screenBPP){

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_screenBPP = screenBPP;

    // Setup SDL
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

    m_window = SDL_CreateWindow("Immediate Mode OFD Reader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

    //m_screenRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    //if ( m_screenRenderer == nullptr ){
        //LOG(ERROR) << "SDL_CreateRenderer() failed. " << SDL_GetError();
        //return false;
    //}

    m_glContext = SDL_GL_CreateContext(m_window);

    gl3wInit();

    m_imageSurface = create_image_surface(m_screenWidth, m_screenHeight, m_screenBPP);
    if ( m_imageSurface == nullptr ){
        LOG_ERROR("create_image_surface() failed. %s", SDL_GetError());
        return false;
    }
    return true;
}

// defined in imgui_impl_sdl_gl3.cc
void ImGui_ImplSdlGL3_RenderDrawLists(ImDrawData* draw_data);
//static const char* ImGui_ImplSdlGL3_GetClipboardText(void*);
//static void ImGui_ImplSdlGL3_SetClipboardText(void*, const char* text);

bool ImOFDApp::initImGui(){

    ImGuiIO& io = ImGui::GetIO();

    // -------- ImGui join SDL --------
    //ImGui_ImplSdlGL3_Init(m_window);
    io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;                     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDLK_a;
    io.KeyMap[ImGuiKey_C] = SDLK_c;
    io.KeyMap[ImGuiKey_V] = SDLK_v;
    io.KeyMap[ImGuiKey_X] = SDLK_x;
    io.KeyMap[ImGuiKey_Y] = SDLK_y;
    io.KeyMap[ImGuiKey_Z] = SDLK_z;

    io.RenderDrawListsFn = ImGui_ImplSdlGL3_RenderDrawLists;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
    //io.SetClipboardTextFn = ImGui_ImplSdlGL3_SetClipboardText;
    //io.GetClipboardTextFn = ImGui_ImplSdlGL3_GetClipboardText;
    io.ClipboardUserData = nullptr;

#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    io.ImeWindowHandle = wmInfo.info.win.window;
#else
    (void)m_window;
#endif

    // -------- ImGui Font --------
    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //io.Fonts->AddFontDefault();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("./fonts/msyh.ttf", 18.0f);
    //m_font = io.Fonts->AddFontFromFileTTF("./fonts/msyhbd.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("./fonts/AdobeSongStd-Light.otf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("./fonts/SimSun.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("./fonts/Times New Roman Italic.ttf", 15.0f);
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    return true;
}

void ImOFDApp::processSystemEvents(){
    SDL_Event event;
    //auto& io = ImGui::GetIO();
    while (SDL_PollEvent(&event)){
        if ( event.type == SDL_QUIT ){
            m_finished = true;
            break;
        }

        switch (event.type) {
        case SDL_MOUSEWHEEL: {
                if (event.wheel.y > 0)
                    m_mouseWheel = 1;
                if (event.wheel.y < 0)
                    m_mouseWheel = -1;
                return;
            }
        case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button == SDL_BUTTON_LEFT) m_mousePressed[0] = true;
                if (event.button.button == SDL_BUTTON_RIGHT) m_mousePressed[1] = true;
                if (event.button.button == SDL_BUTTON_MIDDLE) m_mousePressed[2] = true;
                return;
            }
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                m_screenWidth = event.window.data1;
                m_screenHeight = event.window.data2;
                SDL_FreeSurface(m_imageSurface);
                m_imageSurface = create_image_surface(m_screenWidth, m_screenHeight, m_screenBPP);
                SDL_RenderPresent(m_screenRenderer);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(m_screenRenderer);
                break;
            }
            break;
        }; // switch (event->type)
        //ImGui_ImplSdlGL3_ProcessEvent(&event);

    }; // while (SDL_PollEvent(&event))

}

// ======== ImOFDApp::renderScreen() ========
void ImOFDApp::renderScreen() const {
    // Render(imageSurface)
    //SDL_Texture *backgroundTexture = SDL_CreateTexture(m_screenRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_screenWidth, m_screenHeight);
    //if ( backgroundTexture == nullptr ){
        //LOG(ERROR) << "SDL_CreateTexture() failed. " << SDL_GetError();
        //return -1;
    //}
    //SDL_UpdateTexture(backgroundTexture, NULL, m_imageSurface->pixels, m_imageSurface->pitch);

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(m_screenRenderer, m_imageSurface);

    // Clear screen
    SDL_SetRenderDrawColor(m_screenRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_screenRenderer);
    // Render texture to screen
    SDL_Rect destRect{0, 0, (int)m_screenWidth, (int)m_screenHeight};
    SDL_RenderCopy(m_screenRenderer, backgroundTexture, NULL, &destRect);

    // Update screen
    SDL_RenderPresent(m_screenRenderer);

    SDL_DestroyTexture(backgroundTexture);
}

// ======== ImOFDApp::refreshScreen() ========
void ImOFDApp::refreshScreen(uint64_t color) const{
    SDL_FillRect(m_imageSurface, NULL, color);
}


// -------- create_cairo_surface_from_sdl_surface() --------
cairo_surface_t *create_cairo_surface_from_sdl_surface(SDL_Surface *sdl_surface){
    cairo_surface_t *cairo_surface = nullptr;

    cairo_surface = cairo_image_surface_create_for_data(
            (unsigned char*)sdl_surface->pixels,
            CAIRO_FORMAT_ARGB32,
            sdl_surface->w,
            sdl_surface->h,
            sdl_surface->pitch
            );

    return cairo_surface;
}

// ======== ImOFDApp::renderCairo() ========
void ImOFDApp::renderCairo(){
    refreshScreen(0xFFFFFF);

    cairo_surface_t *cairoSurface = create_cairo_surface_from_sdl_surface(m_imageSurface);
    if ( cairoSurface == nullptr ){
        LOG_ERROR("create_cairo_surface_from_sdl_surface() failed.");
        return;
    }

    OnRender(cairoSurface);

    cairo_surface_destroy(cairoSurface);
}

void ImOFDApp::guiBeginFrame(){
    ImGuiIO &io = ImGui::GetIO();
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    io.DisplaySize = ImVec2((float)w, (float)h);

    int display_w, display_h;
    SDL_GL_GetDrawableSize(m_window, &display_w, &display_h);
    io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

    //io.DeltaTime = ;
    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);

    //ImGui_ImplSdlGL3_NewFrame(m_window);

    ImGui_ImplSdlGL3_CreateDeviceObjects();
    //ImGui_ImplSdlGL3_CreateFontsTexture();

    // Setup time step
    Uint32	time = SDL_GetTicks();
    double current_time = time / 1000.0;
    io.DeltaTime = m_time > 0.0 ? (float)(current_time - m_time) : (float)(1.0f / 60.0f);
    m_time = current_time;

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from SDL_PollEvent())
    int mx, my;
    Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
    if (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MOUSE_FOCUS)
        io.MousePos = ImVec2((float)mx, (float)my);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
    else
        io.MousePos = ImVec2(-1, -1);

    io.MouseDown[0] = m_mousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] = m_mousePressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = m_mousePressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    m_mousePressed[0] = m_mousePressed[1] = m_mousePressed[2] = false;

    io.MouseWheel = m_mouseWheel;
    m_mouseWheel = 0.0f;

    // Hide OS mouse cursor if ImGui is drawing it
    SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

    // Start the frame
    ImGui::NewFrame();

    ImGui::PushFont(m_font);
}

void ImOFDApp::guiEndFrame(){
    float menu_height = showMainMenu();
    float toolbar_bottom = showMainToolbar(menu_height);
    if (ImGui::GetIO().DisplaySize.y > 0){
        auto pos = ImVec2(0, toolbar_bottom);
        auto size = ImGui::GetIO().DisplaySize;;
        size.y -= pos.y;
        ImGui::RootDock(pos, size);
    }

    // -------- Dock GUI --------

    onEntityListGUI();
    onPackDataGUI();

    ImGui::PopFont();

    // GL Rendering
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    ImVec4 clear_color = ImColor(114, 144, 154);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);


    ImGui::Render();
    SDL_GL_SwapWindow(m_window);

    //renderCairo();
    //renderScreen();

}

void ImOFDApp::onEntityListGUI(){
    //if (ImGui::BeginDock("Entity List", &m_is_entity_list_opened)){
        if (ImGui::Button("First one")){
        }
    //}
    //ImGui::EndDock();

    //if (ImGui::BeginDock("Entity List2", &m_is_entity_list_opened)){
        if (ImGui::Button("Second one")){
        }
    //}
    //ImGui::EndDock();

    //if (ImGui::BeginDock("Entity List3", &m_is_entity_list_opened)){
        if (ImGui::Button("Third one")){
        }
    //}
    //ImGui::EndDock();
}

void ImOFDApp::showPackDataDialog(){
    m_is_pack_data_dialog_opened = true;
}

void ImOFDApp::onPackDataGUI(){
    //if (ImGui::BeginDock("Pack data", &m_is_pack_data_dialog_opened)){
    if (m_is_pack_data_dialog_opened){
        ImGui::LabelText("Destination dir", "%s", "xxxx");
        ImGui::SameLine();
        if (ImGui::Button("Choose dir")){
            //char dest_dir[256];
            //if (PlatformInterface::getOpenDirectory(dest_dir, lengthOf(dest_dir), ".")){
                ////dest_dir << "/";
            //}
        }

        int mode = 0;
        ImGui::Combo("Mode", (int*)&mode, "All files\0Loaded universe\0");

        if (ImGui::Button("Pack")){
            //packData();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) m_is_pack_data_dialog_opened = false;
    }
    //ImGui::EndDock();
}

void ImOFDApp::update(){
    guiBeginFrame();

    guiEndFrame();
}

void ImOFDApp::run(){
    while (!m_finished){
        processSystemEvents();
        if (!m_finished){
            update();
        }
    };
}

void ImOFDApp::OnRender(cairo_surface_t *surface){
}

int imgui_main(int, char**) {

    ImGuiApp *app = ImGuiApp::Create();
    app->run();
    ImGuiApp::Destroy(*app);

    return 0;

    //// Setup SDL
    //if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    //{
        //printf("Error: %s\n", SDL_GetError());
        //return -1;
    //}

    //// Setup window
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    //SDL_DisplayMode current;
    //SDL_GetCurrentDisplayMode(0, &current);
    //SDL_Window *window = SDL_CreateWindow("ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    //SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    //gl3wInit();

    //// Setup ImGui binding
    //ImGui_ImplSdlGL3_Init(window);

    //// Load Fonts
    //// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    ////ImGuiIO& io = ImGui::GetIO();
    ////io.Fonts->Clear();
    ////io.Fonts->AddFontFromFileTTF("./fonts/msyhbd.ttf", 16.0f);
    ////io.Fonts->AddFontFromFileTTF("./fonts/msyh.ttf", 13.0f);
    ////io.Fonts->AddFontFromFileTTF("./fonts/AdobeSongStd-Light.otf", 13.0f);
    ////io.Fonts->AddFontFromFileTTF("./fonts/SimSun.ttf", 13.0f);
    ////io.Fonts->AddFontFromFileTTF("./fonts/Times New Roman Italic.ttf", 15.0f);
    ////io.Fonts->AddFontDefault();
    ////io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    ////io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    ////io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    ////io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    ////io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    //bool show_test_window = true;
    //bool show_another_window = false;
    //ImVec4 clear_color = ImColor(114, 144, 154);

    //// Main loop
    //bool done = false;
    //while (!done)
    //{
        //SDL_Event event;
        //while (SDL_PollEvent(&event))
        //{
            //ImGui_ImplSdlGL3_ProcessEvent(&event);
            //if (event.type == SDL_QUIT)
                //done = true;
        //}
        //ImGui_ImplSdlGL3_NewFrame(window);

        ////ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

        //// 1. Show a simple window
        //// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        //{
            //static float f = 0.0f;
            //ImGui::Text("Hello, world!");
            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            //ImGui::ColorEdit3("clear color", (float*)&clear_color);
            //if (ImGui::Button("测试窗口")) show_test_window ^= 1;
            //if (ImGui::Button("Another Window")) show_another_window ^= 1;
            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //}

        //// 2. Show another simple window, this time using an explicit Begin/End pair
        //if (show_another_window)
        //{
            //ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiCond_FirstUseEver);
            //ImGui::Begin("Another Window", &show_another_window);
            //ImGui::Text("Hello");
            //ImGui::End();
        //}

        //// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        //if (show_test_window)
        //{
            //ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
            //ImGui::ShowTestWindow(&show_test_window);
        //}

        //// Rendering
        //glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);

        ////ImGui::PopFont();

        //ImGui::Render();
        //SDL_GL_SwapWindow(window);
    //}

    //// Cleanup
    //ImGui_ImplSdlGL3_Shutdown();
    //SDL_GL_DeleteContext(glcontext);
    //SDL_DestroyWindow(window);
    //SDL_Quit();


    //return 0;
}
