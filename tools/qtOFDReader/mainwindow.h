#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotFileOpen();
    void slotFileSave();
    void slotFileSaveAs();
    void slotFileClose();
    void slotFileCloseAll();
    void slotFilePrint();

    void slotViewFullScreen();

    void slotOFDSinglePage();
    void slotOFDSinglePageContinuous();
    void slotOFDTwoPages();
    void slotOFDTwoPagesContinuous();
    void slotOFDAutomaticallyResize();
    void slotOFDActualSize();
    void slotOFDZoomToFit();
    void slotOFDZoomIn();
    void slotOFDZoomOut();
    void slotOFDCurrentSettingsDefault();

    void slotToolsTextTool();
    void slotToolsLineTool();

    void slotBookmarksAddBookmark();
    void slotBookmarksManage();

    void slotHelpQTOFDReaderHelp();

    void onTest1Menu(bool);
    void onTest2Menu(bool);
    void onTest3Menu(bool);
    void onContextMenu(const QPoint& pos);

private:
    void connectSlots();

private:
    Ui::MainWindow *ui;
    QGraphicsView *m_graphicsView = nullptr;
};

#endif // MAINWINDOW_H
