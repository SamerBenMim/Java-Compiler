#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Window Styles
    this->resize(this->screen()->availableGeometry().size()/1.4);
    QPalette darkPalette;
QColor darkColor(45, 45, 45);
QColor textColor(220, 220, 220);
darkPalette.setColor(QPalette::Window, darkColor);
darkPalette.setColor(QPalette::WindowText, textColor);
darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
darkPalette.setColor(QPalette::AlternateBase, darkColor);
darkPalette.setColor(QPalette::ToolTipBase, textColor);
darkPalette.setColor(QPalette::ToolTipText, textColor);
darkPalette.setColor(QPalette::Text, textColor);
darkPalette.setColor(QPalette::Button, darkColor);
darkPalette.setColor(QPalette::ButtonText, textColor);
darkPalette.setColor(QPalette::BrightText, Qt::red);
darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
darkPalette.setColor(QPalette::HighlightedText, Qt::black);

QApplication::setPalette(darkPalette);
    // Icon provider
    this->iconProvider = new QFileIconProvider();


    // Tab Holder
    this->tabHolder = new TabHolder(this, iconProvider);
    this->setCentralWidget(tabHolder);
    connect(this->tabHolder, &TabHolder::documentExists, this, &MainWindow::toggleDocumentOperations);
    this->tabHolder->addNewTab();


    // Bottom Bar
    this->bottomBar = new Bottombar(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, this->bottomBar);

    // Menu Bar
    this->createMenuBar();
    this->setWindowTitle("SSA");

    // Sidebar
    this->sideBar = new Sidebar(this, iconProvider);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->sideBar);
    connect(this->sideBar, &Sidebar::fileOpened, this->tabHolder, &TabHolder::loadFile);


    // Status Bar
    this->createStatusBar();
}

MainWindow::~MainWindow()
{
}


void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (this->tabHolder->handleCloseAll()) {
        event->accept();
    } else {
        event->ignore();
    }
}
