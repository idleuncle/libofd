#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <cairo/cairo.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("qtOFDReader（测试版）");

    QGraphicsScene graphicsScene;
    graphicsScene.addLine(0, 0, 150, 150);

    m_graphicsView = new QGraphicsView(&graphicsScene);
    m_graphicsView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

    this->connect(m_graphicsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenu(QPoint)));
    this->setCentralWidget(m_graphicsView);

    QPainter(m_graphicsView->viewport());

    connectSlots();
}

MainWindow::~MainWindow()
{
    delete m_graphicsView;
    delete ui;
}

void MainWindow::onTest1Menu(bool){
    statusBar()->showMessage("Test1 Menu");
}

void MainWindow::onTest2Menu(bool){
    statusBar()->showMessage("Test2 Menu");
}

void MainWindow::onTest3Menu(bool){
    statusBar()->showMessage("Test3 Menu");
}

void MainWindow::onContextMenu(const QPoint&){
    QMenu *cmenu = new QMenu(this);
    QAction *action1 = cmenu->addAction("Menu 1");
    QAction *action2 = cmenu->addAction("Menu 2");
    QAction *action3 = cmenu->addAction("Menu 3");
    connect(action1, SIGNAL(triggered(bool)), this, SLOT(onTest1Menu(bool)));
    connect(action2, SIGNAL(triggered(bool)), this, SLOT(onTest2Menu(bool)));
    connect(action3, SIGNAL(triggered(bool)), this, SLOT(onTest3Menu(bool)));
    cmenu->exec(QCursor::pos());
}

void MainWindow::connectSlots(){

    connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(slotFileOpen()));
    connect(ui->actionFileSave, SIGNAL(triggered()), this, SLOT(slotFileSave()));
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), this, SLOT(slotFileSaveAs()));
    connect(ui->actionFileClose, SIGNAL(triggered()), this, SLOT(slotFileClose()));
    connect(ui->actionFileCloseAll, SIGNAL(triggered()), this, SLOT(slotFileCloseAll()));
    connect(ui->actionFilePrint, SIGNAL(triggered()), this, SLOT(slotFilePrint()));

    connect(ui->actionViewFullScreen, SIGNAL(triggered()), this, SLOT(slotViewFullScreen()));

    connect(ui->actionOFDSinglePage, SIGNAL(triggered()), this, SLOT(slotOFDSinglePage()));
    connect(ui->actionOFDSinglePageContinuous, SIGNAL(triggered()), this, SLOT(slotOFDSinglePageContinuous()));
    connect(ui->actionOFDTwoPages, SIGNAL(triggered()), this, SLOT(slotOFDTwoPages()));
    connect(ui->actionOFDTwoPagesContinuous, SIGNAL(triggered()), this, SLOT(slotOFDTwoPagesContinuous()));
    connect(ui->actionOFDAutomaticallyResize, SIGNAL(triggered()), this, SLOT(slotOFDAutomaticallyResize()));
    connect(ui->actionOFDActualSize, SIGNAL(triggered()), this, SLOT(slotOFDActualSize()));
    connect(ui->actionOFDZoomToFit, SIGNAL(triggered()), this, SLOT(slotZoomToFit()));
    connect(ui->actionOFDZoomIn, SIGNAL(triggered()), this, SLOT(slotOFDZoomIn()));
    connect(ui->actionOFDZoomOut, SIGNAL(triggered()), this, SLOT(slotOFDZoomOut()));
    connect(ui->actionOFDCurrentSettingsDefault, SIGNAL(triggered()), this, SLOT(slotOFDCurrentSettingsDefault()));

    connect(ui->actionToolsTextTool, SIGNAL(triggered()), this, SLOT(slotToolsTextTool()));
    connect(ui->actionToolsLineTool, SIGNAL(triggered()), this, SLOT(slotToolsLineTool()));

    connect(ui->actionBookmarksAddBookmark, SIGNAL(triggered()), this, SLOT(slotBookmarksAddBookmark()));
    connect(ui->actionBookmarksManage, SIGNAL(triggered()), this, SLOT(slotBookmarksManage()));

    connect(ui->actionHelpQTOFDReaderHelp, SIGNAL(triggered()), this, SLOT(slotHelpQTOFDReaderHelp()));

}

// ---------------- fileMenu slots ----------------
void MainWindow::slotFileOpen(){
    statusBar()->showMessage("Openning...");
}

void MainWindow::slotFileSave(){
    statusBar()->showMessage("Saving...");
}

void MainWindow::slotFileSaveAs(){
    statusBar()->showMessage("SaveAs");
}

void MainWindow::slotFileClose(){
}

void MainWindow::slotFileCloseAll(){
}

void MainWindow::slotFilePrint(){
}


// ---------------- viewMenu slots ----------------
void MainWindow::slotViewFullScreen(){
}


// ---------------- ofdMenu slots ----------------
void MainWindow::slotOFDSinglePage(){
}

void MainWindow::slotOFDSinglePageContinuous(){
}

void MainWindow::slotOFDTwoPages(){
}

void MainWindow::slotOFDTwoPagesContinuous(){
}

void MainWindow::slotOFDAutomaticallyResize(){
}

void MainWindow::slotOFDActualSize(){
}

void MainWindow::slotOFDZoomToFit(){
}

void MainWindow::slotOFDZoomIn(){
}

void MainWindow::slotOFDZoomOut(){
}

void MainWindow::slotOFDCurrentSettingsDefault(){
}


// ---------------- toolsMenu slots ----------------
void MainWindow::slotToolsTextTool(){
}

void MainWindow::slotToolsLineTool(){
}


// ---------------- bookmarksMenu slots ----------------
void MainWindow::slotBookmarksAddBookmark(){
}

void MainWindow::slotBookmarksManage(){
}

// ---------------- helpMenu slots ----------------
void MainWindow::slotHelpQTOFDReaderHelp(){
}
