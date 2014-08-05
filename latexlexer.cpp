#include "latexlexer.h"
#include <QDebug>
#include <Qsci/qsciscintilla.h>

LatexLexer::LatexLexer(QObject *parent):QsciLexerCustom(parent)
{
}

LatexLexer::~LatexLexer()
{
}

const char* LatexLexer::language() const
{
    return "LaTeX";
}

QString LatexLexer::description(int style) const
{
    switch (style) {
    case Default:
        return "Default";
        break;
    case Comment:
        return "Comment";
        break;
    case Command:
        return "Command";
        break;
    case MathInline:
        return "MathInline";
        break;
    case MathDisplay:
        return "MathDisplay";
        break;
    case SpecialChar:
        return "SpecialChar";
        break;
    default:
        return "";
        break;
    }
}

void LatexLexer::styleText(int start,int end)
{
    if (!editor())
            return;

    char *chars = (char *) malloc (end - start);

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, chars);
    QString source = QString(chars);
    free(chars);

    startStyling(start,0x1F);
    QStringList list=source.split("\n");
    qDebug()<<source;
    /*for (int i = 0; i < list.size(); i++) {
        QString line=list.at(i);
        int len=line.size();
        int pos=line.indexOf("%");
        if(pos==-1){
            setStyling(len,defaultStyle(Default));
        }
        else{
            setStyling(pos,defaultStyle(Default));
            setStyling(len-pos,defaultStyle(CommentLine));
        }
    }*/
    int lineIndex=editor()->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, start);
    int style,nextStyle;
    if(lineIndex>0){
        int pos = editor()->SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, lineIndex - 1);
        style = editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos);
        qDebug()<<style;
    }
    else{
        style=Default;
    }
    QChar previousCharacter=0,character;
    for (int i = 0; i < list.size(); i++) {
        QString line=list.at(i);
        int len=line.size();
        for(int j=0;j<len;j++){
            character=line.at(j);
            if(character=="%"){
                style=Comment;
                nextStyle=Comment;
            }
            else if(character=="\\"){
                if(style!=Comment){
                    style=Command;
                    nextStyle=Command;
                }
            }
            else if(character=="{" || character=="}"){
                if(style!=Comment){
                    if(previousCharacter=="\\")
                    {
                        style=Command;

                    }
                    else{
                        style=SpecialChar;
                    }
                    nextStyle=Default;
                }
            }
            else if(character=="["){
                if(style!=Comment){
                    if(previousCharacter=="\\")
                    {
                        style=Command;
                        nextStyle=MathDisplay;

                    }
                    else{
                        style=SpecialChar;
                        nextStyle=Default;
                    }
                }
            }
            else if(character=="]"){
                if(style!=Comment){
                    if(previousCharacter=="\\")
                    {
                        style=Command;
                        nextStyle=Default;

                    }
                    else{
                        style=SpecialChar;
                        nextStyle=Default;
                    }
                }
            }
            else if(character=="$"){
                if(style!=Comment){
                    if(previousCharacter=="\\")
                    {
                        style=Command;
                        nextStyle=Default;
                    }
                    else{
                        if(style==MathInline){
                            nextStyle=Default;
                        }
                        else{
                            nextStyle=MathInline;
                        }
                        style=SpecialChar;
                    }
                }
            }
            else if(character.isDigit()){
                if(style!=Comment){
                    style=Digit;
                    nextStyle=Default;
                }
            }
            else{
                nextStyle=style;
            }
            previousCharacter=character;
            setStyling(1,defaultStyle(style));
            style=nextStyle;
        }
    }

}

QColor LatexLexer::defaultColor(int style) const
{
    switch(style){
    case Default:
        return QColor(Qt::black);
        break;
    case Comment:
        return QColor(Qt::lightGray);
        break;
    case Command:
        return QColor(Qt::darkBlue);
        break;
    case MathInline:
        return QColor(Qt::darkGreen);
        break;
    case MathDisplay:
        return QColor(Qt::blue);
        break;
    case SpecialChar:
        return QColor(Qt::darkMagenta);
        break;
    case Digit:
        return QColor(Qt::darkCyan);
        break;
    default:
        return QsciLexerCustom::defaultColor();
        break;
    }
}

QFont LatexLexer::defaultFont(int style) const
{
    switch(style){
    case Default:
        return QFont("Helvetica",11);
        break;
    case Comment:
        return QFont("Helvetica",11,-1,true);
        break;
    case Command:
        return QFont("Helvetica",11);
        break;
    case MathInline:
        return QFont("Helvetica",11);
        break;
    case MathDisplay:
        return QFont("Helvetica",11);
        break;
    case SpecialChar:
        return QFont("Helvetica",11);
        break;
    case Digit:
        return QFont("Helvetica",11);
        break;
    default:
        return QsciLexerCustom::defaultFont();
        break;
    }
}

QColor LatexLexer::defaultPaper(int style) const
{
    return QsciLexerCustom::defaultPaper(style);
}

bool LatexLexer::defaultEolFill(int style) const
{
    return QsciLexerCustom::defaultEolFill(style);
}

QsciStyle LatexLexer::defaultStyle(int style) const
{
    return QsciStyle(style,description(style),defaultColor(style),defaultPaper(style),defaultFont(style),defaultEolFill(style));
}

const char* LatexLexer::wordCharacters() const{
    return "abcdefghijklmnopqrstuvwxyz\\@";
}

