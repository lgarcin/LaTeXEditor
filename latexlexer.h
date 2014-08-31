#ifndef LATEXLEXER_H
#define LATEXLEXER_H

#include <Qsci/qscilexercustom.h>
#include <Qsci/qscistyle.h>
#include <Qsci/qsciapis.h>
#include "outline.h"

class LatexLexer : public QsciLexerCustom
{

public:
    enum{
        Default=0,
        Comment=1,
        Command=2,
        MathInline=3,
        MathDisplay=4,
        SpecialChar=5,
        Digit=6,
        Environment=7,
        Part=8,
        Chapter=9,
        Section=10,
        SubSection=11,
        SubSubSection=12,
        Paragraph=13,
        SubParagraph=14
    };
    LatexLexer(QObject *parent=0);
    ~LatexLexer();
    const char* language() const;
    QString description(int) const;
    void styleText(int,int);
    QColor defaultColor(int) const;
    QFont defaultFont(int) const;
    QColor defaultPaper(int) const;
    bool defaultEolFill(int) const;
    QsciStyle defaultStyle(int) const;
    const char* wordCharacters() const;

private:
    int PartFoldingLevel,ChapterFoldingLevel,SectionFoldingLevel,SubSectionFoldingLevel,SubSubSectionFoldingLevel,ParagraphFoldingLevel,SubParagraphFoldingLevel;
    int getDiffLevel(QString line);
    bool isHeaderLine(QString line);
    QString trimComment(QString str);
    QsciAPIs *apis;
    QString getLine(int i);
};

#endif // LATEXLEXER_H
