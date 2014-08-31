#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#define QSCINTILLA_DLL

#include "latexlexer.h"
#include <Qsci/qsciscintilla.h>

class LatexEditor : public QsciScintilla
{
    Q_OBJECT

public:
    LatexEditor(QWidget *parent = 0);
    ~LatexEditor();
    void initializeMargin();
    void initializeCaretLine();
    void initializeFont();
    void initializeLexer();
    void initializeFolding();
    void initializeOutline();
    OutlineModel* getOutline();

signals:

public slots:
    void postProcessAutocomplete(const char *sel,int pos);
    void updateMargins();
    void updateOutline();

private:
    OutlineModel *outline;
};

#endif // LATEXEDITOR_H
