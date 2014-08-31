#include "latexeditor.h"

LatexEditor::LatexEditor(QWidget *parent) : QsciScintilla(parent)
{
    //setUtf8(true); // Pose problème au lexer
    initializeFont();
    initializeMargin();
    initializeCaretLine();
    initializeLexer();
    initializeFolding();
    initializeOutline();

    setBraceMatching(QsciScintilla::StrictBraceMatch);
    setAutoIndent(true);

    connect(this,SIGNAL(SCN_AUTOCSELECTION(const char*,int)),this,SLOT(postProcessAutocomplete(const char*,int)),Qt::QueuedConnection);
}

LatexEditor::~LatexEditor()
{
}

void LatexEditor::initializeFont()
{
    QFont font("Courier", 12);
    font.setFixedPitch(true);
    setFont(font);
}

void LatexEditor::initializeMargin()
{
    QFontMetrics fontmetrics = QFontMetrics(font());
    setMarginsFont(font());
    setMarginWidth(0, fontmetrics.width(QString::number(lines())) + 6);
    setMarginLineNumbers(0, true);
    setMarginsBackgroundColor(QColor("#cccccc"));

    connect(this, SIGNAL(linesChanged()), this, SLOT(updateMargins()));
}

void LatexEditor::initializeLexer(){
    setLexer(new LatexLexer());
    setAutoCompletionThreshold(3);
    setAutoCompletionSource(QsciScintilla::AcsAPIs);
}

void LatexEditor::initializeCaretLine()
{
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor("#ffe4e4"));
}

void LatexEditor::initializeFolding()
{
    QsciScintilla::FoldStyle state = static_cast<QsciScintilla::FoldStyle>((!folding()) * 5);
    if (!state)
        foldAll(false);
    setFolding(state);
}

void LatexEditor::initializeOutline()
{
    outline=new OutlineModel(this);
    connect(this,SIGNAL(textChanged()),this,SLOT(updateOutline()));
}

void LatexEditor::updateMargins()
{
    QFontMetrics fontmetrics = fontMetrics();
    setMarginWidth(0, fontmetrics.width(QString::number(lines())) + 6);
}

void LatexEditor::postProcessAutocomplete(const char *sel,int pos)
{
    QString cp=QString(sel);
    if(!cp.startsWith("\\")){
        int line,index;
        SendScintilla(QsciScintilla::SCI_BEGINUNDOACTION);
        insert("}\n");
        insert(cp);
        insert("}\n\n\\end{");
        lineIndexFromPosition(pos,&line,&index);
        insertAt("\\begin{",line,index);
        SendScintilla(QsciScintilla::SCI_LINEDOWN);
        SendScintilla(QsciScintilla::SCI_ENDUNDOACTION);
    }
}

void LatexEditor::updateOutline()
{
    QStringList lines;
    outline->setupModelData(lines);
}

OutlineModel* LatexEditor::getOutline()
{
    return outline;
}
