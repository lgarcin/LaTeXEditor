#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("LaTeX Editor");
    initializeMenu();
    initializeEditor();
    initializeOutline();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeMenu()
{
    setMenuBar(new QMenuBar(this));

    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&New", this, SLOT(onNew()), QKeySequence::New);
    fileMenu->addAction("&Open", this, SLOT(onOpen()), QKeySequence::Open);
    fileMenu->addAction("&Save as",this,SLOT(onSave()),QKeySequence::SaveAs);

    buildMenu=menuBar()->addMenu("&Build");
    buildMenu->addAction("&Build",this,SLOT(onBuild()));

    viewMenu=menuBar()->addMenu(tr("&View"));
}

void MainWindow::initializeEditor()
{
    editorTabs=new QTabWidget(this);
    editorTabs->setTabsClosable(true);
    connect(editorTabs,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    setCentralWidget(editorTabs);
    onNew();
}

void MainWindow::initializeOutline()
{
    outlineDock=new QDockWidget(tr("Outline"),this);
    outlineDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outlineView=new QTreeView(outlineDock);
    OutlineModel *outline=((LatexEditor*)editorTabs->currentWidget())->getOutline();
    outlineView->setModel(outline);
    outlineDock->setWidget(outlineView);
    addDockWidget(Qt::LeftDockWidgetArea,outlineDock);
    viewMenu->addAction(outlineDock->toggleViewAction());
    connect(editorTabs,SIGNAL(currentChanged(int)),this,SLOT(setOutline(int)));
}

void MainWindow::onNew()
{
    LatexEditor *editor=new LatexEditor(this);
    editorTabs->addTab(editor,"test");
    editorTabs->setCurrentWidget(editor);
}

void MainWindow::onOpen()
{
    QString filename =  QFileDialog::getOpenFileName(this, tr("Open .tex file"),QDir::currentPath(),"TEX (*.tex *.sty)");
    if (filename.isEmpty())
        return;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;
    LatexEditor *editor=new LatexEditor(this);
    editor->setText(file.readAll());
    editorTabs->addTab(editor,QFileInfo(file).fileName());
    editorTabs->setCurrentWidget(editor);
    file.close();
}

void MainWindow::onSave()
{
}

void MainWindow::onBuild()
{
}

void MainWindow::closeTab(int i)
{
    LatexEditor *editor=(LatexEditor*)editorTabs->widget(i);
    editorTabs->removeTab(i);
    delete editor;
}

void MainWindow::setOutline(int i)
{
    if(i!=-1){
        outlineView->setModel(((LatexEditor*)editorTabs->widget(i))->getOutline());
    }
    else{
        outlineView->setModel(new OutlineModel());
    }
}
