#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTreeView>

#include "latexeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeMenu();
    void initializeEditor();
    void initializeOutline();

public slots:
    void onNew();
    void onOpen();
    void onSave();
    void onBuild();
    void closeTab(int i);
    void setOutline(int i);

private:
    QMenu *fileMenu;
    QMenu *buildMenu;
    QMenu *viewMenu;

    QTabWidget *editorTabs;
    QDockWidget *outlineDock;
    QTreeView *outlineView;
};

#endif // MAINWINDOW_H
