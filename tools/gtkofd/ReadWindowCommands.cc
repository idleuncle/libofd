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

void ReadWindow::CmdJumpPage(){
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
