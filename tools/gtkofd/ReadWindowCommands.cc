#include <string>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "ReadWindow.h"

static std::string choose_file(GtkWindow *parentWindow){
    std::string fileName;

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
            parentWindow,
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

void ReadWindow::CmdFileOpen(){
    std::string fileName = choose_file(m_mainWindow);
    if (!fileName.empty()){
        OpenOFDFile(fileName);
        //DocumentPtr document = OpenOFDFile(fileName);
        //if (document != nullptr){
            //m_document = document;
            //m_pageIndex = 0;
            //m_pageWall->Rebuild(document, 1);
            //m_pageWall->RebuildWall();

            CmdZoomFitBest();
        //}
    }
}

void ReadWindow::CmdFileSave(){
}

void ReadWindow::CmdFileSaveAs(){
}

void ReadWindow::CmdFileExport(){
}

void ReadWindow::CmdFilePrint(){
}

void ReadWindow::CmdFileProperties(){
}

void ReadWindow::CmdFileClose(){
    CloseOFDFile();
}

void ReadWindow::CmdFirstPage(){
    FirstPage();
    RedrawPage();
}

void ReadWindow::CmdLastPage(){
    LastPage();
    RedrawPage();
}

void ReadWindow::CmdNextPage(){
    NextPage();
    RedrawPage();
}

void ReadWindow::CmdPreviousPage(){
    PreviousPage();
    RedrawPage();
}

void ReadWindow::CmdGotoPage(){
    RedrawPage();
}

void ReadWindow::CmdZoomIn(){
    //ZoomIn();
    //RedrawPage();
    ScrollIn();
}

void ReadWindow::CmdZoomOut(){
    //ZoomOut();
    //RedrawPage();
    ScrollOut();
}

void ReadWindow::CmdZoomFitBest(){
    ZoomFitBest();
    RedrawPage();
}

void ReadWindow::CmdZoomOriginal(){
    ZoomOriginal();
    RedrawPage();
}

void ReadWindow::CmdMoveUp(){
    //MoveUp();
    //RedrawPage();
    ScrollUp();
}

void ReadWindow::CmdMoveDown(){
    //MoveDown();
    //RedrawPage();
    ScrollDown();
}

void ReadWindow::CmdMoveLeft(){
    //MoveLeft();
    //RedrawPage();
    ScrollLeft();
}

void ReadWindow::CmdMoveRight(){
    //MoveRight();
    //RedrawPage();
    ScrollRight();
}


// ==================== Tools Submenu ====================

void ReadWindow::CmdToolsNormal(){
    if (m_action->GetType() != Action::Type::NORMAL){
        m_action = ActionFactory::CreateAction(Action::Type::NORMAL);
    }
}

void ReadWindow::CmdToolsSelectAnnotation(){
    if (m_action->GetType() != Action::Type::SELECTANNOTATION){
        m_action = ActionFactory::CreateAction(Action::Type::SELECTANNOTATION);
    }
}

void ReadWindow::CmdToolsSelectText(){
    if (m_action->GetType() != Action::Type::SELECTTEXT){
        m_action = ActionFactory::CreateAction(Action::Type::SELECTTEXT);
    }
}

void ReadWindow::CmdToolsSnapshot(){
    if (m_action->GetType() != Action::Type::SNAPSHOT){
        m_action = ActionFactory::CreateAction(Action::Type::SNAPSHOT);
    }
}

void ReadWindow::CmdToolsDrawLine(){
    if (m_action->GetType() != Action::Type::DRAWLINE){
        m_action = ActionFactory::CreateAction(Action::Type::DRAWLINE);
    }
}

void ReadWindow::CmdToolsDrawRect(){
    if (m_action->GetType() != Action::Type::DRAWRECT){
        m_action = ActionFactory::CreateAction(Action::Type::DRAWRECT);
    }
}

void ReadWindow::CmdToolsDrawPolyline(){
    if (m_action->GetType() != Action::Type::DRAWPOLYLINE){
        m_action = ActionFactory::CreateAction(Action::Type::DRAWPOLYLINE);
    }
}
