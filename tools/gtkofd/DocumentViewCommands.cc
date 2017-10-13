#include <assert.h>
#include <string>
#include <algorithm>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "DocumentView.h"
#include "ofd/Package.h"
#include "utils/logger.h"

void DocumentView::CmdFileSave(){
}

void DocumentView::CmdFileSaveAs(){
}

void DocumentView::CmdFileExport(){
}

void DocumentView::CmdFilePrint(){
}

static void set_property_label(GtkBuilder *builder, const std::string &widgetName, const std::string &text){
    GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(builder, widgetName.c_str()));
    assert(widget != nullptr);
    gtk_label_set_text(GTK_LABEL(widget), text.c_str());
}

void DocumentView::CmdFileProperties(){

    GtkBuilder *builder = gtk_builder_new_from_resource("/ui/app.ui");

    GtkDialog *docinfoDialog = (GtkDialog*)gtk_builder_get_object(builder, "docinfoDialog");
    if (docinfoDialog == nullptr){
        LOG_WARN("Get DocInfo dialog from resource /ui/app.ui is failed.");
        return;
    }

    std::string strDocID;
    std::string strTitle;
    std::string strAuthor;
    std::string strSubject;
    std::string strAbstract;
    std::string strCreationDate;
    std::string strModDate;
    std::string strDocUsage;
    std::string strCover;
    std::string strKeywords;
    std::string strCreator;
    std::string strCreatorVersion;
    std::string strCustomDatas;

    if (m_document != nullptr){
        const ofd::Document::CT_DocInfo &docInfo = m_document->GetDocBody().DocInfo;

        strDocID = docInfo.DocID;
        strTitle = docInfo.Title;
        strAuthor = docInfo.Author;
        strSubject = docInfo.Subject;
        strAbstract = docInfo.Abstract;
        strCreationDate = docInfo.CreationDate;
        strModDate = docInfo.ModDate;
        strDocUsage = docInfo.DocUsage;
        strCover = docInfo.Cover;
        //strKeywords = docInfo.Keywords;
        std::for_each(docInfo.Keywords.begin(), docInfo.Keywords.end(), 
                [&strKeywords](const std::string& k){strKeywords += k + " ";});
        strCreator = docInfo.Creator;
        strCreatorVersion = docInfo.CreatorVersion;
        //strCustomDatas = docInfo.CustomDatas;
    }

    set_property_label(builder, "textDocID", strDocID);
    set_property_label(builder, "textTitle", strTitle);
    set_property_label(builder, "textAuthor", strAuthor);
    set_property_label(builder, "textSubject", strSubject);
    set_property_label(builder, "textAbstract", strAbstract);
    set_property_label(builder, "textCreationDate", strCreationDate);
    set_property_label(builder, "textModDate", strModDate);
    set_property_label(builder, "textDocUsage", strDocUsage);
    set_property_label(builder, "textCover", strCover);
    set_property_label(builder, "textKeywords", strKeywords);
    set_property_label(builder, "textCreator", strCreator);
    set_property_label(builder, "textCreatorVersion", strCreatorVersion);
    set_property_label(builder, "textCustomDatas", strCustomDatas);

    g_signal_connect(G_OBJECT(docinfoDialog),"destroy",G_CALLBACK(gtk_widget_destroy), nullptr);
    gtk_window_set_title(GTK_WINDOW(docinfoDialog), "DocInfo");
    gtk_widget_show_all(GTK_WIDGET(docinfoDialog));
}

void DocumentView::CmdFirstPage(){
    FirstPage();
    RedrawDocumentView();
}

void DocumentView::CmdLastPage(){
    LastPage();
    RedrawDocumentView();
}

void DocumentView::CmdNextPage(){
    NextPage();
    RedrawDocumentView();
}

void DocumentView::CmdPreviousPage(){
    PreviousPage();
    RedrawDocumentView();
}

void DocumentView::CmdGotoPage(){
    RedrawDocumentView();
}

void DocumentView::CmdZoomIn(){
    //ZoomIn();
    //RedrawDocumentView();
    ScrollIn();
}

void DocumentView::CmdZoomOut(){
    //ZoomOut();
    //RedrawDocumentView();
    ScrollOut();
}

void DocumentView::CmdZoomFitBest(){
    ZoomFitBest();
    RedrawDocumentView();
}

void DocumentView::CmdZoomOriginal(){
    ZoomOriginal();
    RedrawDocumentView();
}

void DocumentView::CmdMoveUp(){
    //MoveUp();
    //RedrawDocumentView();
    ScrollUp();
}

void DocumentView::CmdMoveDown(){
    //MoveDown();
    //RedrawDocumentView();
    ScrollDown();
}

void DocumentView::CmdMoveLeft(){
    //MoveLeft();
    //RedrawDocumentView();
    ScrollLeft();
}

void DocumentView::CmdMoveRight(){
    //MoveRight();
    //RedrawDocumentView();
    ScrollRight();
}


// ==================== Tools Submenu ====================

void DocumentView::CmdToolsNormal(){
    if (m_action->GetType() != Action::Type::NORMAL){
        m_action = ActionFactory::CreateAction(Action::Type::NORMAL);
    }
}

void DocumentView::CmdToolsSelectAnnotation(){
    if (m_action->GetType() != Action::Type::SELECTANNOTATION){
        m_action = ActionFactory::CreateAction(Action::Type::SELECTANNOTATION);
    }
}

void DocumentView::CmdToolsSelectText(){
    if (m_action->GetType() != Action::Type::SELECTTEXT){
        m_action = ActionFactory::CreateAction(Action::Type::SELECTTEXT);
    }
}

void DocumentView::CmdToolsSnapshot(){
    if (m_action->GetType() != Action::Type::SNAPSHOT){
        m_action = ActionFactory::CreateAction(Action::Type::SNAPSHOT);
    }
}

void DocumentView::CmdToolsDrawLine(){
    if (m_action->GetType() != Action::Type::DRAWLINE){
        m_action = ActionFactory::CreateAction(Action::Type::DRAWLINE);
    }
}

void DocumentView::CmdToolsDrawRect(){
    if (m_action->GetType() != Action::Type::DRAWRECT){
        m_action = ActionFactory::CreateAction(Action::Type::DRAWRECT);
    }
}

void DocumentView::CmdToolsDrawPolyline(){
    if (m_action->GetType() != Action::Type::DRAWPOLYLINE){
        m_action = ActionFactory::CreateAction(Action::Type::DRAWPOLYLINE);
    }
}
