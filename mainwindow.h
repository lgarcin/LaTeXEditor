#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qsci/qsciscintilla.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeUI();
    void initializeEditor();
    void initializeLexer();
    void initializeFolding();
    void initializeMargin();
    void initializeCaretLine();
    void initializeFont();
public slots:
    void onTextChanged();
    void onOpen();
    void onSave();
    void onBuild();
    void postProcessAutocomplete(const char* sel,int pos);
private:
    QsciScintilla *editor;
};

#endif // MAINWINDOW_H
