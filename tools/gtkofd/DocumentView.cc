#include <assert.h>
#include "utils/logger.h"
#include "utils/Geometry.h"
#include "DocumentView.h"

//defined in OFDRender
extern double g_resolutionX;

using namespace ofd;

DocumentView::DocumentView(PackageViewPtr packageView)
    : m_packageView(packageView)
{
}

DocumentView::~DocumentView(){
}

void DocumentView::SetDocument(DocumentPtr document){
    m_document = document;

}

void DocumentView::Rebuild(){
    if (m_document != nullptr){
        if (m_pageWall != nullptr){
            //m_pageWall->SetWallViewArea(0, 0, 1.0);
            m_pageWall->Rebuild(m_document, m_rowPages);
        } else {
            LOG_WARN("DocumentView::SetDocument() m_pageWall == nullptr");
        }
    } else {
        LOG_WARN("DocumentView::SetDocument() document == nullptr");
        m_pageWall->Rebuild(nullptr, m_rowPages);
    }
}

void DocumentView::RedrawDocumentView(){
    if (m_drawingArea != nullptr){
        gdk_window_invalidate_rect(gtk_widget_get_window(m_drawingArea), nullptr, false);
    }
}

void DocumentView::OnSize(int width, int height){
    //if (m_ofdRender != nullptr){
        //m_ofdRender->RebuildBackgroundImage(allocation->width, allocation->height);
    //} else {
        //m_ofdRender = std::make_shared<ofd::OFDRender>(drawingArea, allocation->width, allocation->height);
    //}

    LOG_DEBUG("DocumentView::OnSize() width:%d height:%d", width, height);
    if (m_pageWall == nullptr){
        m_pageWall = std::make_shared<PageWall>(width, height);
        //m_pageWall->Rebuild(m_document, m_rowPages);
        Rebuild();
    } else {
        m_pageWall->OnSize(width, height);
    }
    RedrawDocumentView();
}

void DocumentView::OnDraw(cairo_t *cr){
    GdkRGBA rgba;
    rgba.red = 0.25;
    rgba.green = 0.25;
    rgba.blue = 0.25;
    rgba.alpha = 0.6;
    gdk_cairo_set_source_rgba(cr, &rgba);
    cairo_paint(cr);

    //if (m_ofdRender != nullptr){
        //m_ofdRender->RenderBackgroundImage(m_document, m_pageIndex);
        //cairo_surface_t *backgroundSurface = m_ofdRender->GetCairoRender()->GetCairoSurface();
        //cairo_set_source_surface(cr, backgroundSurface, 0, 0);
        //cairo_paint(cr);
    //}

    if (m_pageWall != nullptr){
        m_pageWall->RenderWall(cr);
    }
}

void DocumentView::FirstPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetPagesCount();
    m_pageIndex = 0;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, (int)total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MoveFirstPage();
}

void DocumentView::LastPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetPagesCount();
    m_pageIndex = total_pages - 1;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MoveLastPage();
}

void DocumentView::NextPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetPagesCount();
    if (m_pageIndex < total_pages - 1){
        m_pageIndex++;
    } else {
        m_pageIndex = 0;
    }
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MoveNextPage();
}

void DocumentView::PreviousPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetPagesCount();
    if (m_pageIndex > 0){
        m_pageIndex--;
    } else {
        m_pageIndex = total_pages - 1;
    }
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MovePreviousPage();
}

void DocumentView::ZoomIn(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomIn();
    }
}

void DocumentView::ZoomOut(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomOut();
    }
}

void DocumentView::ZoomFitBest(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomFitBest();
    }
}

void DocumentView::ZoomOriginal(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomOriginal();
    }
}

void DocumentView::MoveUp(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveUp();
    }
}

void DocumentView::MoveDown(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveDown();
    }
}

void DocumentView::MoveLeft(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveLeft();
    }
}

void DocumentView::MoveRight(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveRight();
    }
}

#define SCROLL_TIME_DELTA 400* 1000
//#define SCROLL_MOVE_ACCELERATION 1.2
//#define SCROLL_MAX_MOVE_ACCELERATION 10
#define SCROLL_MOVE_ACCELERATION 1.2
#define SCROLL_MAX_MOVE_ACCELERATION 10
#define SCROLL_ZOOM_ACCELERATION 1.2
#define SCROLL_MAX_ZOOM_ACCELERATION 8

double DocumentView::applyScrollAccelerate(int scrollAction, double acceleration, double maxAcceleration){
    if (m_scrollAction != scrollAction){
        m_scrollAction = scrollAction;
        m_lastScrollTime = 0;
        m_scrollAcceRate = 1.0;
        m_scrollCount = 0;
    }

    m_scrollCount++;
    double scrollAcceRate = m_scrollAcceRate;

    unsigned long long scrollTime = utils::GetTimeTick();
    if (m_lastScrollTime != 0){
        if (scrollTime - m_lastScrollTime < SCROLL_TIME_DELTA){
            if (m_scrollCount % 2 == 0){
                scrollAcceRate = m_scrollAcceRate * acceleration;
                if (scrollAcceRate >= maxAcceleration){
                    scrollAcceRate = maxAcceleration;
                }
                //LOG_NOTICE("scrollAcceRate:%.6f", scrollAcceRate);
            }
        } else {
            scrollAcceRate = 1.0;
            m_scrollCount = 0;
            //LOG_INFO("scrollAcceRate set to 1.0");
        }
    }
    m_lastScrollTime = scrollTime;

    return scrollAcceRate;
}

void DocumentView::ScrollUp(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(0, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveUp(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollDown(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(1, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveDown(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollLeft(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(2, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveLeft(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollRight(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(3, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveRight(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollIn(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(4, SCROLL_ZOOM_ACCELERATION, SCROLL_MAX_ZOOM_ACCELERATION);
    m_pageWall->ZoomIn(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollOut(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(5, SCROLL_ZOOM_ACCELERATION, SCROLL_MAX_ZOOM_ACCELERATION);
    m_pageWall->ZoomOut(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::OffsetPageWall(double offsetX, double offsetY){
    if (m_pageWall != nullptr){
        m_pageWall->OffsetViewArea(offsetX, offsetY);
    }
}

// 
void DocumentView::SelectText(double x0, double y0, double x1, double y1){
    if (m_pageWall != nullptr){

        double wallOffsetX = 0;
        double wallOffsetY = 0;
        double wallScaling = 1.0;
        std::tie(wallOffsetX, wallOffsetY, wallScaling) = m_pageWall->GetWallViewArea();
        double xx0 = utils::geometry::pixel_to_mm((x0 + wallOffsetX) / wallScaling, g_resolutionX);
        double yy0 = utils::geometry::pixel_to_mm((y0 + wallOffsetY) / wallScaling, g_resolutionX);
        double xx1 = utils::geometry::pixel_to_mm((x1 + wallOffsetX) / wallScaling, g_resolutionX);
        double yy1 = utils::geometry::pixel_to_mm((y1 + wallOffsetY) / wallScaling, g_resolutionX);

        int pageIndex = m_pageWall->GetPageIndexFromWallPoint(x0, y0);
        //LOG_DEBUG("SelectText() pageIndex=%d", pageIndex);
        ofd::DocumentPtr document = m_pageWall->GetDocument();
        ofd::PagePtr page = document->GetPage(pageIndex);
        ofd::text::TextPagePtr textPage = page->GetTextPage();
        for (size_t i = 0 ; i < textPage->GetTextLinesCount() ; i++){
            ofd::text::TextLinePtr textLine = textPage->GetTextLine(i);
            textLine->SelectText(xx0, yy0, xx1, yy1);
        }
    }
}

// A typical use for ::begin-print is to use the parameters from the GtkPrintContext and paginate the document accordingly, and then set the number of pages with gtk_print_operation_set_n_pages().
static gboolean begin_print_cb(
        GtkPrintOperation *operation, 
        GtkPrintContext   *context, 
        gpointer          user_data){
    LOG_DEBUG("Print begin print.");
    return false;
}

// Emitted for every page that is printed. The signal handler must render the page_nr 's page onto the cairo context obtained from context using gtk_print_context_get_cairo_context().
static gboolean draw_page_cb(
        GtkPrintOperation *operation, 
        GtkPrintContext   *context, 
        gint              page_nr,
        gpointer          user_data){
    LOG_DEBUG("Print draw page %d.", page_nr);

    cairo_t *cr = gtk_print_context_get_cairo_context(context);
    //gdouble width = gtk_print_context_get_width(context);

    ////cairo_rectangle(cr, 0, 0, width, HEADER_HEIGHT);
    //cairo_rectangle(cr, 0, 0, width, 50);

    //cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    //cairo_fill(cr);


    DocumentView *documentView = (DocumentView*)user_data;
    assert(documentView != nullptr);
    int dpi = documentView->m_print_dpi;

    DocumentPtr document = documentView->GetDocument();
    assert(document != nullptr);
    PagePtr page = document->GetPage(page_nr);
    if (page != nullptr){
        page->Open();

        double pixelWidth = 0.0;
        double pixelHeight = 0.0;
        bool bOK = false;
        std::unique_ptr<CairoRender> cairoRender = 
            utils::make_unique<ofd::CairoRender>(pixelWidth, pixelHeight, dpi, dpi);
        std::tie(pixelWidth, pixelHeight, bOK) = cairoRender->RenderPage(page, dpi, cr);
        if (!bOK){
            LOG_WARN("Page %d draw failed.", page->ID);
        } 
    }

    //double pixelWidth = cairo_image_surface_get_width(backgroundSurface);
    //double pixelHeight = cairo_image_surface_get_height(backgroundSurface);


    //PangoLayout *layout = gtk_print_context_create_pango_layout(context);
    //PangoFontDescription *desc = pango_font_description_from_string("sans:14");
    //pango_layout_set_font_description(layout, desc);
    //pango_font_description_free(desc);

    //pango_layout_set_text(layout, "some text", -1);
    //pango_layout_set_width(layout, width * PANGO_SCALE);
    //pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

    //gdouble text_height;
    //pango_layout_get_size(layout, nullptr, &layout_height);
    //text_height = (gdouble)layout_height / PANGO_SCALE;

    //cairo_move_to(cr, width / 2, (HEADER_HEIGHT - text_height) / 2);
    //pango_cairo_show_layout(cr, layout);

    //g_object_unref(layout);

    return false;
}

// Emitted after all pages have been rendered. A handler for this signal can clean up any resources that have been allocated in the “begin-print” handler.
static gboolean end_print_cb(
        GtkPrintOperation *operation, 
        GtkPrintContext   *context, 
        gpointer          user_data){
    LOG_DEBUG("Print end print.");
    return false;
}

static gboolean preview_cb(
        GtkPrintOperation        *operation, 
        GtkPrintOperationPreview *preview, 
        GtkPrintContext          *context,
        GtkWindow                *parent,
        gpointer                 user_data){
    LOG_NOTICE("Print preview");

    cairo_t *cr = gtk_print_context_get_cairo_context(context);
    gdouble width = gtk_print_context_get_width(context);

    //cairo_rectangle(cr, 0, 0, width, HEADER_HEIGHT);
    cairo_rectangle(cr, 0, 0, width, 50);

    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_fill(cr);

    return false;
}

static gboolean done_cb(
        GtkPrintOperation       *operation,
        GtkPrintOperationResult result,
        gpointer                user_data){
    return false;
}

static gboolean status_changed_cb(GtkPrintOperation *operation, gpointer user_data){
    const GtkPrintStatus status = gtk_print_operation_get_status(operation);
    std::string status_string = gtk_print_operation_get_status_string(operation);
    LOG_NOTICE("Print status: (%d) %s", status, status_string.c_str());
    return false;
}


// GtkPrintOperation
// https://developer.gnome.org/gtk3/stable/gtk3-High-level-Printing-API.html#GtkPrintOperation-begin-print
// Signals:
//  begin-print
//  draw-page
//  end-print
//  preview
void DocumentView::DoPrint() const{

    GtkPrintSettings *settings = gtk_print_settings_new();

    gtk_print_settings_set_resolution(settings, m_print_dpi);
    //gtk_print_settings_set_scale(settings, 1.0);
    // GTK_PRINt_PAGES_ALL
    // GTK_PRINt_PAGES_CURRENT
    // GTK_PRINt_PAGES_RANGES
    // GTK_PRINt_PAGES_SELECTION
    gtk_print_settings_set_print_pages(settings, GTK_PRINT_PAGES_ALL);
    //gtk_print_settings_set_print_pages(settings, GTK_PRINT_PAGES_RANGES);
    //GtkPageRange pageRange; 
    //pageRange.start = 0;
    //pageRange.end = 1;
    //gtk_print_settings_set_page_ranges(settings, &pageRange, 1);

    // GTK_PAGE_SET_ALL
    // GTK_PAGE_SET_EVEN
    // GTK_PAGE_SET_ODD
    gtk_print_settings_set_page_set(settings, GTK_PAGE_SET_ALL);

    GtkPaperSize *paperSize = gtk_paper_size_new(GTK_PAPER_NAME_A4);
    double mmPaperWidth = gtk_paper_size_get_width(paperSize, GTK_UNIT_MM);
    double mmPaperHeight = gtk_paper_size_get_height(paperSize, GTK_UNIT_MM);
    double pixelPaperWidth = gtk_paper_size_get_width(paperSize, GTK_UNIT_PIXEL);
    double pixelPaperHeight = gtk_paper_size_get_height(paperSize, GTK_UNIT_PIXEL);
    double pointPaperWidth = gtk_paper_size_get_width(paperSize, GTK_UNIT_POINTS);
    double pointPaperHeight = gtk_paper_size_get_height(paperSize, GTK_UNIT_POINTS);
    LOG_INFO("Default paper size mm:(%.2f, %.2f) pixel:(%.2f, %.2f) point:(%.2f, %.2f)", 
            mmPaperWidth, mmPaperHeight,
            pixelPaperWidth,pixelPaperHeight,
            pointPaperWidth, pointPaperHeight);

    gtk_print_settings_set_paper_size(settings, paperSize);

    // GTK_PAGE_ORIENTATION_PORTRAIT
    // GTK_PAGE_ORIENTATION_LANDSCAPE
    // GTK_PAGE_ORIENTATION_REVERSE_PORTRAIT
    // GTK_PAGE_ORIENTATION_REVERSE_LANDSCAPE
    gtk_print_settings_set_orientation(settings, GTK_PAGE_ORIENTATION_PORTRAIT);

    gtk_print_settings_set_use_color(settings, true);
    gtk_print_settings_set_reverse(settings, false);
    // GTK_PRINT_DUPLEX_SIMPLEX
    // GTK_PRINT_DUPLEX_HORIZONTAL
    // GTK_PRINT_DUPLEX_VERTICAL
    gtk_print_settings_set_duplex(settings, GTK_PRINT_DUPLEX_SIMPLEX);
    //// GTK_PRINT_QUALITY_LOW
    //// GTK_PRINT_QUALITY_NORMAL
    //// GTK_PRINT_QUALITY_HIGH
    //// GTK_PRINT_QUALITY_DRAFT
    gtk_print_settings_set_quality(settings, GTK_PRINT_QUALITY_NORMAL);
    gtk_print_settings_set_n_copies(settings, 1);
    //gtk_print_settings_set_number_up(settings, GTK_NUMBER_UP_LAYOUT_LEFT_TO_RIGHT_TOP_TO_BOTTOM);


    GtkPrintOperationResult res;
    GtkPrintOperation *print = gtk_print_operation_new();
    if (settings != nullptr){
        gtk_print_operation_set_print_settings(print, settings);
    }

    DocumentPtr document = this->GetDocument();
    if (document == nullptr){
        LOG_WARN("document == nullptr");
        return;
    }
    size_t totalPages = document->GetPagesCount();

    gtk_print_operation_set_allow_async(print, true);
    gtk_print_operation_set_n_pages(print, totalPages);
    gtk_print_operation_set_show_progress(print, true);
    //gtk_print_operation_set_export_filename(print, "xxx.pdf");
    //gtk_print_operation_set_embed_page_setup(print, true);
    //gtk_print_operation_set_current_page(print, 0);
    //gtk_print_operation_set_use_full_page(print, true);

    g_signal_connect(print, "begin-print", G_CALLBACK(begin_print_cb), (gpointer)this);
    g_signal_connect(print, "draw-page", G_CALLBACK(draw_page_cb), (gpointer)this);
    g_signal_connect(print, "end-print", G_CALLBACK(end_print_cb), (gpointer)this);
    g_signal_connect(print, "preview", G_CALLBACK(preview_cb), (gpointer)this);
    g_signal_connect(print, "done", G_CALLBACK(done_cb), (gpointer)this);
    g_signal_connect(print, "status-changed", G_CALLBACK(status_changed_cb), (gpointer)this);

    res = gtk_print_operation_run(print, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,
            GTK_WINDOW(nullptr), nullptr);

    if (res == GTK_PRINT_OPERATION_RESULT_APPLY){
        if (settings != nullptr){
            g_object_unref(settings);
            settings = nullptr;
        }
        settings = (GtkPrintSettings*)g_object_ref(gtk_print_operation_get_print_settings(print));
    }

    g_object_unref(print);
    g_object_unref(settings);
    //g_object_unref(paperSize);

}    
