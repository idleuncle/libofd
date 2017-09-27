#include <stdlib.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkscreen.h>
#include <assert.h>

extern "C"{
#include "gtkofd_resources.h"
}
#include "OFDRender.h"

#include "ofd/Package.h"
#include "ofd/Document.h"
#include "ofd/Page.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "ofd/CairoRender.h"
using namespace ofd;

GtkWindow *m_mainWindow;

class GtkRender : public ofd::OFDRender{
public:
    GtkRender(GtkWidget *drawingArea, int screenWidth, int screenHeight, int screenBPP = 32):
        ofd::OFDRender(screenWidth, screenHeight, screenBPP) 
        //m_drawingArea(drawingArea)
    {}
    virtual ~GtkRender(){};

    virtual void UpdateScreenRenderer(){
    }
    virtual void UpdateScreen(){
    }

private:
    //__attribute__((unused)) GtkWidget *m_drawingArea;
}; // class GtkRender
std::shared_ptr<ofd::OFDRender> m_ofdRender = nullptr;

//double g_resolutionX = 240.0; // 120, 160, 240
//double g_resolutionY = 240.0;
//double ZOOM_STEP = 0.1;
//double ZOOM_BASE = exp(1.0);
//double X_STEP = 9;
//double Y_STEP = 10;

ofd::PackagePtr m_package = nullptr; 
DocumentPtr m_document = nullptr;
size_t m_pageIndex = 0;

DocumentPtr open_ofd_document(const std::string &filename){
    m_package = std::make_shared<ofd::Package>();
    if ( !m_package->Open(filename) ){
        LOG(ERROR) << "OFDPackage::Open() failed. filename:" << filename;
        return nullptr;
    }
    DocumentPtr document = m_package->GetDefaultDocument(); 
    assert(document != nullptr);
    LOG(DEBUG) << document->to_string();

    bool bOpened = document->Open();
    if ( !bOpened ){
        LOG(ERROR) << "Open OFD Document failed. filename: " << filename;
        return nullptr;
    }

    return document;
}

typedef struct{
    GtkApplicationWindow parentWindow;

    GtkWidget *drawingArea = nullptr;
    GtkWidget *infobar = nullptr;
    GtkWidget *message = nullptr;
    GtkWidget *status = nullptr;
    GMenuModel *mainMenu = nullptr;

    int width = 0;
    int heigth = 0;
    bool isMaximized = false;
    bool isFullscreen = false;

} ApplicationWindowContext;


//class MyRender : public SDLCairoRender{
//public:
    //MyRender(int screenWidth, int screenHeight, int screenBPP);
    //virtual ~MyRender(){};
    //virtual void onRender(cairo_surface_t *surface);
//};

//MyRender::MyRender(int screenWidth, int screenHeight, int screenBPP):
    //SDLCairoRender(screenWidth, screenHeight, screenBPP)
//{
//}

//void MyRender::onRender(cairo_surface_t *surface){
//}

static void activate_about(GSimpleAction *action, GVariant *parameter, gpointer user_data){

    GtkApplication *app = (GtkApplication*)user_data;
    const gchar *authors[] = {
        "Jiangwen Su <idleuncle@gmail.com>",
        nullptr
    };

    gtk_show_about_dialog(GTK_WINDOW(gtk_application_get_active_window(app)),
            "program-name", "OFD 阅读器",
            "version", g_strdup_printf ("%s,\n基于GTK+ %d.%d.%d",
                "1.0.0",
                gtk_get_major_version (),
                gtk_get_minor_version (),
                gtk_get_micro_version ()),
            "copyright", "(C) 2016-2017 The LastZ Team",
            //"license-type", GTK_LICENSE_LGPL_2_1,
            "license-type", GTK_LICENSE_MIT_X11,
            "website", "http://www.lastz.org",
            "comments", "阅读OFD文件。", 
            "authors", authors,
            "logo-icon-name", "gtk3-demo",
            "title", "关于 OFD 阅读器",
            nullptr);
}

static void activate_quit (GSimpleAction *action, GVariant *parameter, gpointer user_data){

    GtkApplication *app = (GtkApplication*)user_data;
    GtkWidget *win;
    GList *list;

    list = gtk_application_get_windows(app);
    while (list) {
        win = GTK_WIDGET(list->data);
        GList *next = list->next;

        gtk_widget_destroy(win);

        list = next;
    }
}

__attribute__((unused)) static void activate_run(GSimpleAction *action, GVariant *parameter, gpointer user_data){

    //GtkWidget *window = (GtkWidget*)user_data;

    //GtkTreeSelection *selection;
    //GtkTreeModel *model;
    //GtkTreeIter iter;

    //selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
    //if (gtk_tree_selection_get_selected (selection, &model, &iter))
        //run_example_for_row (window, model, &iter);
}

__attribute__((unused)) static GdkPixbuf *frame = nullptr;
static GResource *resource = nullptr;
static GtkWidget *drawingArea = nullptr;
static GtkWidget *infobar = nullptr;
static GtkWidget *message = nullptr;
static GtkWindow *renderingWindow = nullptr;

static void redraw_page(){
    gdk_window_invalidate_rect(gtk_widget_get_window(drawingArea), nullptr, false);
}

static void first_page(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = 0;
    m_ofdRender->SetOffsetX(0.0);
    m_ofdRender->SetOffsetY(0.0);
    redraw_page();
    LOG(DEBUG) << "Page " << m_pageIndex + 1 <<  "/" << total_pages;
}

static void last_page(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = total_pages - 1;
    m_ofdRender->SetOffsetX(0.0);
    m_ofdRender->SetOffsetY(0.0);
    redraw_page();
    LOG(DEBUG) << "Page " << m_pageIndex + 1 <<  "/" << total_pages;
}
static void next_page(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    if (m_pageIndex < total_pages - 1){
        m_pageIndex++;
    } else {
        m_pageIndex = 0;
    }
    m_ofdRender->SetOffsetX(0.0);
    m_ofdRender->SetOffsetY(0.0);
    redraw_page();
    LOG(DEBUG) << "Page " << m_pageIndex + 1 <<  "/" << total_pages;
}

static void prev_page(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    if (m_pageIndex > 0){
        m_pageIndex--;
    } else {
        m_pageIndex = total_pages - 1;
    }
    m_ofdRender->SetOffsetX(0.0);
    m_ofdRender->SetOffsetY(0.0);
    redraw_page();
    LOG(DEBUG) << "Page " << m_pageIndex + 1 <<  "/" << total_pages;
}

static void size_allocate_cb(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data){

    if (m_ofdRender != nullptr){
        m_ofdRender->RebuildBackgroundImage(allocation->width, allocation->height);
    } else {
        m_ofdRender = std::make_shared<GtkRender>(drawingArea, allocation->width, allocation->height);
    }
}

static std::string choose_file(){
    std::string fileName;

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
            m_mainWindow,
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Cancel",
            GTK_RESPONSE_CANCEL,
            "_Open",
            GTK_RESPONSE_ACCEPT,
            nullptr);
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        char *filename = gtk_file_chooser_get_filename(chooser);
        fileName = filename;
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    return fileName;
}

// https://developer.gnome.org/gtk3/3.2/GtkWidget.html#Signal-Details

static gboolean key_press_cb(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
    switch (event->keyval){
    case GDK_KEY_i:
        m_ofdRender->ZoomIn();
        redraw_page();
        break;
    case GDK_KEY_o:
        m_ofdRender->ZoomOut();
        redraw_page();
        break;
    case GDK_KEY_f:
        m_ofdRender->ZoomFitBest();
        redraw_page();
        break;
    case GDK_KEY_h:
        m_ofdRender->MoveLeft();
        redraw_page();
        break;
    case GDK_KEY_j:
        m_ofdRender->MoveDown();
        redraw_page();
        break;
    case GDK_KEY_k:
        m_ofdRender->MoveUp();
        redraw_page();
        break;
    case GDK_KEY_l:
        m_ofdRender->MoveRight();
        redraw_page();
        break;
    case GDK_KEY_n:
        next_page();
        redraw_page();
        break;
    case GDK_KEY_p:
        prev_page();
        redraw_page();
        break;
    default:
        //LOG(DEBUG) << "Key pressed. keyval:" << event->keyval;
        return false;
    }; // switch

    return true;
}

static gboolean key_release_cb(GtkWidget *widget, GdkEventKey *event, gpointer user_data){
    switch (event->keyval){
    case GDK_KEY_o:
        if (event->state & GDK_CONTROL_MASK){
            LOG(DEBUG) << "Key Ctrl+o released. keyval:" << event->keyval;
            std::string fileName = choose_file();
            if (!fileName.empty()){
                DocumentPtr document = open_ofd_document(fileName);
                if (document != nullptr){
                    m_document = document;
                    m_pageIndex = 0;
                    redraw_page();
                }
            }
        }
        break;
    case GDK_KEY_plus:
        if (event->state & GDK_CONTROL_MASK){
            // 放大 Ctrl + +
            m_ofdRender->ZoomIn();
            redraw_page();
            LOG(DEBUG) << "Key Ctrl++ released. keyval:" << event->keyval;
        }
        break;
    case GDK_KEY_minus:
        if (event->state & GDK_CONTROL_MASK){
            // 缩小 Ctrl + -
            m_ofdRender->ZoomOut();
            redraw_page();
            LOG(DEBUG) << "Key Ctrl+- released. keyval:" << event->keyval;
        }
        break;
    case GDK_KEY_1:
        if (event->state & GDK_CONTROL_MASK){
            // 原始大小 Ctrl + 1
            LOG(DEBUG) << "Key Ctrl+1 released. keyval:" << event->keyval;
        }
        break;
    case GDK_KEY_2:
        if (event->state & GDK_CONTROL_MASK){
            // 适合页面 Ctrl + 2
            m_ofdRender->ZoomFitBest();
            redraw_page();
            LOG(DEBUG) << "Key Ctrl+2 released. keyval:" << event->keyval;
        }
        break;
    case GDK_KEY_3:
        if (event->state & GDK_CONTROL_MASK){
            // 适合宽度 Ctrl + 3
            LOG(DEBUG) << "Key Ctrl+3 released. keyval:" << event->keyval;
        }
        break;
    case GDK_KEY_4:
        if (event->state & GDK_CONTROL_MASK){
            // 适合高度 Ctrl + 4 
            LOG(DEBUG) << "Key Ctrl+4 released. keyval:" << event->keyval;
        }
        break;
    case GDK_KEY_Home:
        first_page();
        break;
    case GDK_KEY_End:
        last_page();
        break;
    case GDK_KEY_Page_Down:
        LOG(DEBUG) << "Page Down KEY RELEASED!";
        next_page();
        break;
    case GDK_KEY_Page_Up:
        LOG(DEBUG) << "Page Up KEY RELEASED!";
        prev_page();
        break;
    case GDK_KEY_Down:
        LOG(DEBUG) << "Down KEY RELEASED!";
        break;
    case GDK_KEY_Up:
        LOG(DEBUG) << "Up KEY RELEASED!";
        break;
    case GDK_KEY_space:
        LOG(DEBUG) << "SPACE KEY RELEASED!";
        break;
    default:
        //LOG(DEBUG) << "Key released. keyval:" << event->keyval;
        return false;
    }; // switch

    return true;
}
/*
struct GdkEventButton {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  gdouble x;
  gdouble y;
  gdouble *axes;
  guint state;
  guint button;
  GdkDevice *device;
  gdouble x_root, y_root;
};
 */
static gboolean button_press_cb(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
    if (event->button == GDK_BUTTON_PRIMARY){
        LOG(DEBUG) << "LEFT BUTTON PRESSED! x:" << event->x << " y:" << event->y;
        //next_page();
    } else if (event->button == GDK_BUTTON_SECONDARY){
        LOG(DEBUG) << "RIGHT BUTTON PRESSED! x:" << event->x << " y:" << event->y;
        //prev_page();
    }

    return false;
}

__attribute__((unused)) static gboolean button_release_cb(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
    return false;
}

/*
struct GdkEventConfigure {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gint x, y;
  gint width;
  gint height;
};
 */
__attribute__((unused)) static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data){
    return false;
}

static gboolean motion_notify_cb(GtkWidget *widget, GdkEventMotion *event, gpointer user_data){
    int x, y;
    GdkModifierType state;

    gdk_window_get_device_position(event->window, event->device, &x, &y, &state);

    //if (state & GDK_BUTTON1_MASK){
        //LOG(DEBUG) << "MOTION NOTIFY! x:" << x << " y:" << y;
    //}
    return true;
}

/*
struct GdkEventScroll {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  guint32 time;
  gdouble x;
  gdouble y;
  guint state;
  GdkScrollDirection direction;
  GdkDevice *device;
  gdouble x_root, y_root;
  gdouble delta_x;
  gdouble delta_y;
  guint is_stop : 1;
};
 */
__attribute__((unused)) gboolean scroll_event_cb(GtkWidget *widget, GdkEventScroll *event, gpointer user_data){
    GdkScrollDirection direction = event->direction;

    LOG(DEBUG) << "Scroll Event. Delta x:" << event->delta_x << " Delta y:" << event->delta_y << " Direction:" << direction;

    if (event->state & GDK_SHIFT_MASK){
        if (event->direction == GDK_SCROLL_UP){
            m_ofdRender->ZoomIn();
            redraw_page();
        } else if (event->direction == GDK_SCROLL_DOWN){
            m_ofdRender->ZoomOut();
            redraw_page();
        }
    } else {
        if (event->direction == GDK_SCROLL_UP){
            m_ofdRender->MoveUp();
            redraw_page();
        } else if (event->direction == GDK_SCROLL_DOWN){
            m_ofdRender->MoveDown();
            redraw_page();
        } else if (event->direction == GDK_SCROLL_LEFT){
            m_ofdRender->MoveLeft();
            redraw_page();
        } else if (event->direction == GDK_SCROLL_RIGHT){
            m_ofdRender->MoveRight();
            redraw_page();
        }
    }

    return false;
}

/*
struct GdkEventCrossing {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  GdkWindow *subwindow;
  guint32 time;
  gdouble x;
  gdouble y;
  gdouble x_root;
  gdouble y_root;
  GdkCrossingMode mode;
  GdkNotifyType detail;
  gboolean focus;
  guint state;
};
 */
__attribute__((unused)) static gboolean enter_notify_cb(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data){
    LOG(DEBUG) << "ENTER NOTIFY x:" << event->x << " y:" << event->y;
    return false;
}

__attribute__((unused)) static gboolean leave_notify_cb(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data){
    LOG(DEBUG) << "LEAVE NOTIFY x:" << event->x << " y:" << event->y;
    return false;
}

/*
typedef enum {
  GTK_DIR_TAB_FORWARD,
  GTK_DIR_TAB_BACKWARD,
  GTK_DIR_UP,
  GTK_DIR_DOWN,
  GTK_DIR_LEFT,
  GTK_DIR_RIGHT
} GtkDirectionType;
 */
__attribute__((unused)) static gboolean focus_cb(GtkWidget *widget, GtkDirectionType direction, gpointer user_data){
    LOG(DEBUG) << "FOCUS direction:" << direction;
    return false;
}

/*
struct GdkEventFocus {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gint16 in;
};
 */
__attribute__((unused)) static gboolean focus_in_event_cb(GtkWidget *widget, GdkEventFocus *event, gpointer user_data){
    LOG(DEBUG) << "Keyboard FOCUS in EVENT. Gained keyboard focus:" << event->in;
    return false;
}

__attribute__((unused)) static gboolean focus_out_event_cb(GtkWidget *widget, GdkEventFocus *event, gpointer user_data){
    LOG(DEBUG) << "Keyboard FOCUS out EVENT. Gained keyboard focus:" << event->in;
    return false;
}

static gint draw_cb(GtkWindow *widget, cairo_t *cr, gpointer data){
    //gdk_cairo_set_source_pixbuf(cr, frame, 0, 0);

    GdkRGBA rgba;
    rgba.red = 1.0;
    rgba.green = 1.0;
    rgba.blue = 1.0;
    rgba.alpha = 1.0;
    gdk_cairo_set_source_rgba(cr, &rgba);
    cairo_paint(cr);

    if (m_ofdRender != nullptr){
        m_ofdRender->RenderBackgroundImage(m_document, m_pageIndex);
        cairo_surface_t *backgroundSurface = m_ofdRender->GetCairoRender()->GetCairoSurface();
        cairo_set_source_surface(cr, backgroundSurface, 0, 0);
        cairo_paint(cr);
    }

    return true;
}

__attribute__((unused)) static void showInfoBar(GSimpleAction *action, GVariant *parameter, gpointer data){

    const gchar *name = g_action_get_name(G_ACTION(action));
    const gchar *value = g_variant_get_string(parameter, nullptr);

    gchar *text = g_strdup_printf("Your activated action: \"%s\".\n"
            "Current value: %s", name ,value);
    gtk_label_set_text(GTK_LABEL(message), text);
    gtk_widget_show(infobar);

    g_free(text);
}

void adjust_window_size(GtkWindow *window){

    GdkRectangle workArea;

    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    gdk_monitor_get_workarea(monitor, &workArea);

    //GdkScreen *screen = gdk_screen_get_default();
    //gint monitor_num = gdk_screen_get_primary_monitor(screen);
    //gdk_screen_get_monitor_workarea(screen, monitor_num, &workArea);


    GtkRequisition minimum_size;
    GtkRequisition natural_size;
    gtk_widget_get_preferred_size(GTK_WIDGET(window), &minimum_size, &natural_size);

    gtk_window_set_default_size(window, natural_size.width, workArea.height);
    //gtk_window_set_default_size(window, natural_size.width, natural_size.height);
    //gtk_window_set_position(window, GTK_WIN_POS_CENTER);
}

void app_set_theme_from_file(const gchar *theme_path) {
    static GtkCssProvider *provider = NULL;
    GFile *file;
    GdkScreen *screen;
    screen = gdk_screen_get_default();
    if(theme_path!=NULL) {
        file = g_file_new_for_path(theme_path);
        g_print("Loading GTK3 CSS File: %s\n", theme_path);
        if(file!=NULL)
        {
            if(provider==NULL)
                provider = gtk_css_provider_new();
            gtk_css_provider_load_from_file(provider, file, NULL);
            gtk_style_context_add_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(provider),
                GTK_STYLE_PROVIDER_PRIORITY_USER);
            gtk_style_context_reset_widgets(screen);
            g_print("Loaded GTK3 CSS File Successfully!\n");
        }
    }
    else
    {
        if(provider!=NULL)
        {
            gtk_style_context_remove_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(provider));
            g_object_unref(provider);
            provider = NULL;
        }
        gtk_style_context_reset_widgets(screen);
    }
}

void app_set_theme_from_resource(const gchar *theme_path) {
    static GtkCssProvider *provider = NULL;
    GdkScreen *screen;
    screen = gdk_screen_get_default();
    if(theme_path!=NULL) {
        g_print("Loading GTK3 CSS File: %s\n", theme_path);
        if(provider==NULL)
            provider = gtk_css_provider_new();
        gtk_css_provider_load_from_resource(provider, theme_path);
        gtk_style_context_add_provider_for_screen(screen,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_style_context_reset_widgets(screen);
        g_print("Loaded GTK3 CSS File Successfully!\n");
    }
    else
    {
        if(provider!=NULL)
        {
            gtk_style_context_remove_provider_for_screen(screen,
                GTK_STYLE_PROVIDER(provider));
            g_object_unref(provider);
            provider = NULL;
        }
        gtk_style_context_reset_widgets(screen);
    }
}

#define TOOL_BUTTON_SIZE GTK_ICON_SIZE_SMALL_TOOLBAR
//#define TOOL_BUTTON_SIZE GTK_ICON_SIZE_DND

GtkWidget* GTK_IMAGE_FROM_RESOURCE(const std::string &iconName){
    //std::string resourcePath = std::string("/icons/gtkofd/faenza/24/") + iconName + ".png";
    //std::string resourcePath = std::string("/icons/gtkofd/faenza/48/") + iconName + ".png";
    std::string resourcePath = std::string("/icons/gtkofd/emerald/24/") + iconName + ".svg";
    return gtk_image_new_from_resource(resourcePath.c_str());
}

static void activate(GApplication *app){

    // -------- Load widgets from resource by GtkBuilder --------
    //GtkBuilder *builder = gtk_builder_new_from_file("./app.ui");
    GtkBuilder *builder = gtk_builder_new_from_resource("/ui/app.ui");

    // -------- mainWindow --------
    m_mainWindow = (GtkWindow*)gtk_builder_get_object(builder, "mainWindow");
    assert(m_mainWindow != nullptr);

    // -------- mainToolbar --------
    GtkToolbar *mainToolbar = (GtkToolbar*)gtk_builder_get_object(builder, "mainToolbar");
    assert(mainToolbar != nullptr);
    gtk_toolbar_set_style(GTK_TOOLBAR(mainToolbar), GTK_TOOLBAR_BOTH);
    //gtk_toolbar_set_style(GTK_TOOLBAR(mainToolbar), GTK_TOOLBAR_ICONS);

    //gtk_toolbar_set_icon_size(mainToolbar, GTK_ICON_SIZE_MENU); // 16px for menu
    gtk_toolbar_set_icon_size(mainToolbar, GTK_ICON_SIZE_SMALL_TOOLBAR); // 16px for small toolbar
    //gtk_toolbar_set_icon_size(mainToolbar, GTK_ICON_SIZE_LARGE_TOOLBAR); // 24px for large toolbar
    //gtk_toolbar_set_icon_size(mainToolbar, GTK_ICON_SIZE_DND); // 32px for drang and drop
    //gtk_toolbar_set_icon_size(mainToolbar, GTK_ICON_SIZE_DIALOG); // 48px for dialog

    gtk_container_set_border_width(GTK_CONTAINER(mainToolbar), 0);

    // https://specifications.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
    
    //GtkToolItem *toolWindowNew = gtk_tool_button_new(gtk_image_new_from_icon_name("window-new", TOOL_BUTTON_SIZE), "新建");
    //gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolWindowNew, -1);

    //GtkToolItem *toolFileOpen = gtk_tool_button_new(gtk_image_new_from_icon_name("document-open", TOOL_BUTTON_SIZE), "打开");
    //GtkToolItem *toolFileOpen = gtk_tool_button_new(gtk_image_new_from_resource("/icons/gtkofd/emerald/24/document-open.svg"), "打开");
    GtkToolItem *toolFileOpen = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("document-open"), "打开");
    gtk_tool_item_set_tooltip_text(toolFileOpen, "打开");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolFileOpen, -1);

    //GtkToolItem *toolFileSave = gtk_tool_button_new(gtk_image_new_from_icon_name("document-save", TOOL_BUTTON_SIZE), "保存");
    GtkToolItem *toolFileSave = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("document-save"), "保存");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolFileSave, -1);

    //GtkToolItem *toolFileSaveAs = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("document-save-as"), "另存");
    //gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolFileSaveAs, -1);

    // -------- separator tool item --------
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), gtk_separator_tool_item_new(), -1);

    GtkToolItem *toolGoFirst = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("go-first"), "首页");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolGoFirst, -1);

    GtkToolItem *toolGoPrevious = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("go-previous"), "前页");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolGoPrevious, -1);

    GtkToolItem *toolGoNext = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("go-next"), "后页");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolGoNext, -1);

    GtkToolItem *toolGoLast = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("go-last"), "尾页");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolGoLast, -1);

    GtkToolItem *toolGoJump = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("go-jump"), "跳转");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolGoJump, -1);

    // -------- separator tool item --------
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), gtk_separator_tool_item_new(), -1);

    GtkToolItem *toolZoomIn = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("zoom-in"), "放大");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolZoomIn, -1);

    GtkToolItem *toolZoomOut = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("zoom-out"), "缩小");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolZoomOut, -1);

    GtkToolItem *toolZoomFitBest = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("zoom-fit-best"), "合适");
    gtk_tool_item_set_tooltip_text(toolZoomFitBest, "合适大小");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolZoomFitBest, -1);

    GtkToolItem *toolZoomOriginal = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("zoom-original"), "原始");
    gtk_tool_item_set_tooltip_text(toolZoomOriginal, "原始大小");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolZoomOriginal, -1);

    // -------- separator tool item --------
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), gtk_separator_tool_item_new(), -1);

    GtkToolItem *toolEditCopy = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("edit-copy"), "拷贝");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolEditCopy, -1);

    GtkToolItem *toolEditFind = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("edit-find"), "查找");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolEditFind, -1);

    GtkToolItem *toolFileProperties = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("document-properties"), "属性");
    gtk_tool_item_set_tooltip_text(toolFileProperties, "文档属性");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolFileProperties, -1);

    GtkToolItem *toolPageSetup = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("document-page-setup"), "设置");
    gtk_tool_item_set_tooltip_text(toolPageSetup, "页面设置");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolPageSetup, -1);

    GtkToolItem *toolFilePrint = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("document-print"), "打印");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolFilePrint, -1);


    // -------- separator tool item --------
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), gtk_separator_tool_item_new(), -1);

    GtkToolItem *toolExit = gtk_tool_button_new(GTK_IMAGE_FROM_RESOURCE("application-exit"), "退出");
    gtk_toolbar_insert(GTK_TOOLBAR(mainToolbar), toolExit, -1);
    g_signal_connect_swapped(G_OBJECT(m_mainWindow),"destroy",G_CALLBACK(gtk_main_quit), nullptr);
    g_signal_connect(G_OBJECT(toolExit), "clicked",G_CALLBACK(gtk_main_quit), nullptr);

    // -------- infobar --------
    infobar = GTK_WIDGET(gtk_builder_get_object(builder, "infobar"));
    assert(infobar != nullptr);

    // -------- message --------
    message = GTK_WIDGET(gtk_builder_get_object(builder, "message"));
    assert(message != nullptr);

    // -------- drawingArea --------
    drawingArea = GTK_WIDGET(gtk_builder_get_object(builder, "drawingArea"));
    assert(drawingArea != nullptr);

    // -------- renderingWindow --------
    renderingWindow = (GtkWindow*)gtk_builder_get_object(builder, "renderingWindow");
    assert(renderingWindow != nullptr);


    g_object_unref(builder);

    // -------- Widgets callbacks --------

    static GActionEntry win_entries[] = {
        //{ "run", activate_run, nullptr, nullptr, nullptr },
        { "bold", activate_about, nullptr, nullptr, nullptr },
    };
    g_action_map_add_action_entries(G_ACTION_MAP(m_mainWindow),
            win_entries, G_N_ELEMENTS(win_entries), m_mainWindow);
    g_signal_connect(drawingArea, "draw", G_CALLBACK(draw_cb), nullptr);
    g_signal_connect(drawingArea, "size-allocate", G_CALLBACK(size_allocate_cb), nullptr);

    gtk_widget_hide(GTK_WIDGET(mainToolbar));

    // -------- Drawing Widget --------
    GtkWidget *drawingWidget = GTK_WIDGET(drawingArea);

    g_signal_connect(G_OBJECT(m_mainWindow), "key-press-event", G_CALLBACK(key_press_cb), nullptr);
    g_signal_connect(G_OBJECT(m_mainWindow), "key-release-event", G_CALLBACK(key_release_cb), nullptr);

    g_signal_connect(G_OBJECT(drawingWidget), "button-press-event", G_CALLBACK(button_press_cb), nullptr);
    g_signal_connect(G_OBJECT(drawingWidget), "motion-notify-event", G_CALLBACK(motion_notify_cb), nullptr);
    //g_signal_connect(G_OBJECT(drawingWidget), "enter-notify-event", G_CALLBACK(enter_notify_cb), nullptr);
    //g_signal_connect(G_OBJECT(drawingWidget), "leave-notify-event", G_CALLBACK(leave_notify_cb), nullptr);
    g_signal_connect(G_OBJECT(drawingWidget), "focus", G_CALLBACK(focus_cb), nullptr);
    g_signal_connect(G_OBJECT(drawingWidget), "focus-in-event", G_CALLBACK(focus_in_event_cb), nullptr);
    g_signal_connect(G_OBJECT(drawingWidget), "focus-out-event", G_CALLBACK(focus_out_event_cb), nullptr);

    g_signal_connect(G_OBJECT(drawingWidget), "scroll-event", G_CALLBACK(scroll_event_cb), nullptr);

    gtk_widget_set_events(drawingWidget, gtk_widget_get_events(drawingWidget)
            | GDK_KEY_PRESS_MASK
            | GDK_ENTER_NOTIFY_MASK
            | GDK_LEAVE_NOTIFY_MASK
            | GDK_BUTTON_PRESS_MASK
            | GDK_SCROLL_MASK
            | GDK_POINTER_MOTION_MASK
            | GDK_POINTER_MOTION_HINT_MASK);

    // -------- Main Window --------
    gtk_window_set_title(m_mainWindow, "OFD 阅读器");
    // Hide the title bar and the board.
    //gtk_window_set_decorated(m_mainWindow, false);
    adjust_window_size(m_mainWindow);
    gtk_window_activate_focus(m_mainWindow);


    //gtk_window_set_opacity(m_mainWindow, 0.85);
    //gtk_window_set_focus_on_map(m_mainWindow, true);

    //gtk_window_set_icon(m_mainWindow, create_pixbuf("./app.png"));

    gtk_application_add_window(GTK_APPLICATION(app), m_mainWindow);
    gtk_widget_show_all(GTK_WIDGET(m_mainWindow));

    gtk_widget_hide(infobar);

    //app_set_theme_from_resource("/themes/Adwaita/gtk-3.22/gtk-dark.css");
    //app_set_theme_from_resource("/themes/Adwaita/gtk-3.22/gtk-light.css");

    // -------- United-Ubuntu --------
    //app_set_theme_from_resource("/themes/United-Ubuntu/gtk-3.0/gtk.css");
    //app_set_theme_from_resource("/themes/United-Ubuntu/gtk-3.0/gtk-dark.css");

    // -------- Gnome OSX-III-1.0 --------
    //app_set_theme_from_resource("/themes/Gnome OSX-III-1.0/gtk-3.0/gtk.css");
    //app_set_theme_from_resource("/themes/Gnome OSX-III-1.0/gtk-3.0/gtk-dark.css");

    // -------- Minwaita-OSX --------
    //app_set_theme_from_resource("/themes/Minwaita-OSX-Dark/gtk-3.0/gtk.css");
    //app_set_theme_from_resource("/themes/Minwaita-OSX/gtk-3.0/gtk.css");
    //app_set_theme_from_resource("/themes/Minwaita-OSX/gtk-3.0/gtk-dark.css");

    // -------- OSX-Arc-White --------
    app_set_theme_from_resource("/themes/OSX-Arc-White/gtk-3.22/gtk-light.css");
    //app_set_theme_from_resource("/themes/OSX-Arc-White/gtk-3.22/gtk-dark.css");

    //app_set_theme_from_resource("/themes/OSX-Arc-White/gtk-3.22/gtk-darker.css");

    //app_set_theme("/themes/OSX-Arc-White/gtk-3.22/gtk-light.css");
    ////app_set_theme_from_file("/themes/OSX-Arc-White/gtk-3.22/gtk-darker.css");
    //app_set_theme("./themes/OSX-Arc-White/gtk-3.22/gtk-dark.css");
    //app_set_theme("./themes/OSX-Arc-White/gtk-3.22/gtk-solid-dark.css");
    //app_set_theme("./themes/OSX-Arc-White/gtk-3.22/gtk-solid-darker.css");
    //app_set_theme("./themes/OSX-Arc-White/gtk-3.22/gtk-solid.css");
    
    //app_set_theme("./themes/United-Ubuntu-Dark/gtk-3.0/gtk.css");
    
    //app_set_theme("./themes/Vimix-Theme/VimixLight/gtk-3.0/gtk-dark.css");
    //app_set_theme("./themes/Vimix-Theme/VimixDark-Beryl/gtk-3.0/gtk-dark.css");
    //app_set_theme("./themes/Vimix-Theme/VimixDark-Beryl/gtk-3.0/gtk.css");
    
    //app_set_theme("./themes/equilux-theme-v20170913/Equilux/gtk-3.22/gtk.css");


    // GdkScrollDirection direction; // GDK_SCROLL_DOWN, GDK_SCROLL_SMOOTH, GDK_SCROLL_LEFT, GDK_SCROLL_RIGHT
    //g_signal_connect(G_OBJECT(renderingWindow), "keyboard_press", G_CALLBACK(key_press_cb), nullptr);
    //gtk_window_set_focus_on_map(renderingWindow, true);

    g_signal_emit_by_name(G_OBJECT(drawingArea), "activate");
    gtk_widget_grab_focus(drawingArea);
}

static void startup(GApplication *app){

    resource = gtkofd_get_resource();
    if (resource == nullptr){
        LOG(ERROR) << "gtkofd_get_resource() return nullptr." << std::endl;
    }
    g_resources_register(resource);

    //GtkBuilder *builder = gtk_builder_new_from_file("./app.ui");
    //GtkBuilder *builder = gtk_builder_new_from_resource("/ui/app.ui");
    //GtkBuilder *builder = gtk_builder_new_from_file("./menus.ui");
    GtkBuilder *builder = gtk_builder_new_from_resource("/ui/menus.ui");

    //GObject *appMenu = gtk_builder_get_object(builder, "appmenu");
    GObject *menuBar = gtk_builder_get_object(builder, "menubar");
    //GObject *menuBar = gtk_builder_get_object(builder, "mainMenuBar");
    //gtk_application_set_app_menu(GTK_APPLICATION(app), G_MENU_MODEL(appMenu));
    gtk_application_set_menubar(GTK_APPLICATION(app), G_MENU_MODEL(menuBar));

    g_object_unref (builder);


    std::string filename = "./data/2.ofd";
    m_document = open_ofd_document(filename);
    if (m_document != nullptr){
        size_t total_pages = m_document->GetNumPages();
        LOG(INFO) << total_pages << " pages in " << filename;
        if ( total_pages > 0 ){
            //int screenWidth = 794;
            //int screenHeight = 1122;
            //int screenBPP = 32;
        }
    }

}

static gint command_line(GApplication *app, GApplicationCommandLine *cmdline){

    activate(app);

    return 0;
}

__attribute__((unused)) static void show_action_dialog(GSimpleAction *action){
    const gchar *name;
    GtkWidget *dialog;

    name = g_action_get_name(G_ACTION(action));
    dialog = gtk_message_dialog_new(nullptr,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_CLOSE,
            "You activated action: \"%s\"",
            name);
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), nullptr);

    gtk_widget_show(dialog);
}

static void activate_toggle(GSimpleAction *action, GVariant *parameter, gpointer user_data){
    GVariant *state;

    //show_action_dialog(action);

    state = g_action_get_state(G_ACTION(action));
    g_action_change_state(G_ACTION(action), g_variant_new_boolean(!g_variant_get_boolean(state)));

    g_variant_unref(state);
}

static void change_theme_state(GSimpleAction *action, GVariant *state, gpointer user_data){
    GtkSettings *settings = gtk_settings_get_default();

    g_object_set(G_OBJECT(settings),
            "gtk-application-prefer-dark-theme",
            g_variant_get_boolean(state),
            nullptr);

    g_simple_action_set_state(action, state);
}

int main(int argc, char *argv[]){

    Logger::Initialize(1);

    GtkApplication *app;
    static GActionEntry app_entries[] = {
        { "about", activate_about, nullptr, nullptr, nullptr },
        { "quit", activate_quit, nullptr, nullptr, nullptr },
        { "dark", activate_toggle, nullptr, "false", change_theme_state },
    };

    app = gtk_application_new("org.gtk.Demo", 
            //GApplicationFlags(G_APPLICATION_FLAGS_NONE));
            GApplicationFlags(G_APPLICATION_NON_UNIQUE|G_APPLICATION_HANDLES_COMMAND_LINE));


    g_action_map_add_action_entries(G_ACTION_MAP(app),
            app_entries, G_N_ELEMENTS(app_entries),
            app);
    
    g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_signal_connect(app, "command-line", G_CALLBACK(command_line), NULL);
    //g_signal_connect(app, "handle-local-options", G_CALLBACK(local_options), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);

    return 0;
}
