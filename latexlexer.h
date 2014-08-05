#ifndef LATEXLEXER_H
#define LATEXLEXER_H

#include <Qsci/qscilexercustom.h>
#include <Qsci/qscistyle.h>

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
        Digit=6
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

};

#endif // LATEXLEXER_H
