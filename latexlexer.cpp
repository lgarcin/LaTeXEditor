#include "latexlexer.h"
#include <QDebug>
#include <Qsci/qsciscintilla.h>
#include <QRegularExpression>

LatexLexer::LatexLexer(QObject *parent) : QsciLexerCustom(parent)
{
    apis = new QsciAPIs(this);
    apis->add("\\test");
    apis->add("test123");
    apis->add("foobar");
    apis->prepare();
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
    case Digit:
        return "Digit";
        break;
    case Environment:
        return "Environment";
        break;
    case Part:
        return "Part";
        break;
    case Chapter:
        return "Chapter";
        break;
    case Section:
        return "Section";
        break;
    case SubSection:
        return "SubSection";
        break;
    case SubSubSection:
        return "SubSubSection";
        break;
    case Paragraph:
        return "Paragraph";
        break;
    case SubParagraph:
        return "SubParagraph";
        break;
    default:
        return "";
        break;
    }
}

void LatexLexer::styleText(int start,int end)
{
    // Ajouter support verbatim
    // Colorier les erreurs de syntaxe
    if (!editor())
        return;

    char *lineChars = new char[end - start+1];
    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, lineChars);
    QString source = QString(lineChars);
    free(lineChars);

    startStyling(start,0xFF);
    QStringList list=source.split('\n');
    int lineIndex=editor()->SendScintilla(QsciScintilla::SCI_LINEFROMPOSITION, start);

    int style=Default,nextStyle=Default;
    if(lineIndex>0){
        int pos = editor()->SendScintilla(QsciScintilla::SCI_GETLINEENDPOSITION, lineIndex-1 );
        if(pos>0){
            int s = editor()->SendScintilla(QsciScintilla::SCI_GETSTYLEAT, pos);
            if(s==MathInline || s==MathDisplay || s==Environment){
                style=s;
                nextStyle=s;
            }
        }
    }

    int level=QsciScintilla::SC_FOLDLEVELBASE;
    if(lineIndex>0){
        QString previousLine = getLine(lineIndex-1);
        level=(editor()->SendScintilla(QsciScintilla::SCI_GETFOLDLEVEL,lineIndex-1)&QsciScintilla::SC_FOLDLEVELNUMBERMASK)+getDiffLevel(previousLine);
    }

    int nbCharsToStyle;
    int styleBeforeMathDisplay=Default,styleBeforeMathInline=Default,styleInBrace=Default,styleBeforeBrace=Default;
    for (int i = 0; i < list.size(); i++) {
        QString line=list.at(i);
        int lineLength=line.size();
        if(style!=MathInline && style!=MathDisplay && style!=Environment){
            style=Default;
            nextStyle=Default;
        }
        int j=0;
        while(j<lineLength){
            QString l=line.mid(j);
            if(l.startsWith("\\%")){
                nbCharsToStyle=2;
                nextStyle=style;
                style=Command;
            }
            else if(l.startsWith("%")){
                nbCharsToStyle=lineLength-j;
                style=Comment;
                nextStyle=Comment;
            }
            else if(l.startsWith("\\[")){
                nbCharsToStyle=2;
                styleBeforeMathDisplay=style;
                style=Command;
                nextStyle=MathDisplay;
            }
            else if(l.startsWith("\\]")){
                nbCharsToStyle=2;
                style=Command;
                nextStyle=styleBeforeMathDisplay;
            }
            else if(l.startsWith("\\(")){
                nbCharsToStyle=2;
                styleBeforeMathInline=style;
                style=Command;
                nextStyle=MathInline;
            }
            else if(l.startsWith("\\)")){
                nbCharsToStyle=2;
                style=Command;
                nextStyle=styleBeforeMathInline;
            }
            else if(l.startsWith("\\$") || l.startsWith("\\{") || l.startsWith("\\}")){
                nbCharsToStyle=2;
                nextStyle=style;
                style=Command;
            }
            else if(l.startsWith("\\")){
                nbCharsToStyle=1;
                nextStyle=style;
                while(nbCharsToStyle<l.size() && l.at(nbCharsToStyle).isLetter()){
                    nbCharsToStyle++;
                }
                QString s=l.left(nbCharsToStyle);
                if(s=="\\begin" || s=="\\end"){
                    styleInBrace=Environment;
                }
                else if(s=="\\part"){
                    styleInBrace=Part;
                }
                else if(s=="\\chapter"){
                    styleInBrace=Chapter;
                }
                else if(s=="\\section"){
                    styleInBrace=Section;
                }
                else if(s=="\\subsection"){
                    styleInBrace=SubSection;
                }
                else if(s=="\\subsubsection"){
                    styleInBrace=SubSubSection;
                }
                else if(s=="\\paragraph"){
                    styleInBrace=Paragraph;
                }
                else if(s=="\\subparagraph"){
                    styleInBrace=SubParagraph;
                }
                style=Command;
            }
            else if(l.startsWith("$")){
                nbCharsToStyle=1;
                if(style==MathInline){
                    nextStyle=styleBeforeMathInline;
                }
                else{
                    styleBeforeMathInline=style;
                    nextStyle=MathInline;
                }
                style=SpecialChar;
            }
            else if(l.at(0).isDigit()){
                bool isNumber=true;
                nbCharsToStyle=1;
                nextStyle=style;
                while(isNumber && nbCharsToStyle<l.size()){
                    nbCharsToStyle++;
                    l.left(nbCharsToStyle).toFloat(&isNumber);
                }
                if(!isNumber){
                    nbCharsToStyle--;
                }
                style=Digit;
            }
            else if(l.startsWith("(")||l.startsWith(")")||l.startsWith("[")||l.startsWith("]")||l.startsWith("&")||l.startsWith("#")){
                nbCharsToStyle=1;
                nextStyle=style;
                style=SpecialChar;
            }
            else if(l.startsWith("{")){
                nbCharsToStyle=1;
                styleBeforeBrace=style;
                nextStyle=styleInBrace;
                style=SpecialChar;
            }
            else if(l.startsWith("}")){
                nbCharsToStyle=1;
                nextStyle=styleBeforeBrace;
                style=SpecialChar;
                styleInBrace=Default;
            }
            else{
                nbCharsToStyle=1;
                if(l.at(0)!=' '){
                    styleInBrace=style;
                }
            }
            setStyling(nbCharsToStyle,defaultStyle(style));
            style=nextStyle;
            j+=nbCharsToStyle;

        }
        setStyling(1,defaultStyle(style));


        QString s=trimComment(line);
        if(s.contains("\\part")){
            level=2000;

        }
        else if(s.contains("\\chapter")){
            level=2001;

        }
        else if(s.contains("\\section")){
            level=2002;

        }
        else if(s.contains("\\subsection")){
            level=2003;

        }
        else if(s.contains("\\subsubsection")){
            level=2004;

        }
        else if(s.contains("\\paragraph")){
            level=2005;

        }
        else if(s.contains("\\subparagraph")){
            level=2006;

        }
        if(isHeaderLine(s)){
            editor()->SendScintilla(QsciScintilla::SCI_SETFOLDLEVEL,lineIndex,level|QsciScintilla::SC_FOLDLEVELHEADERFLAG);
        }
        /*
        else if(line.trimmed().isEmpty()){
            editor()->SendScintilla(QsciScintilla::SCI_SETFOLDLEVEL,lineIndex,level|QsciScintilla::SC_FOLDLEVELWHITEFLAG);
        }
        */
        else{
            editor()->SendScintilla(QsciScintilla::SCI_SETFOLDLEVEL,lineIndex,level);

        }
        level+=getDiffLevel(s);

        lineIndex++;
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
    case Environment:
        return QColor(Qt::darkRed);
        break;
    case Part:
    case Chapter:
    case Section:
    case SubSection:
    case SubSubSection:
    case Paragraph:
    case SubParagraph:
        return QColor(Qt::black);
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
        return QFont("Courier",11);
        break;
    case Comment:
        return QFont("Courier",11,QFont::Normal,true);
        break;
    case Command:
        return QFont("Courier",11);
        break;
    case MathInline:
        return QFont("Courier",11);
        break;
    case MathDisplay:
        return QFont("Courier",11);
        break;
    case SpecialChar:
        return QFont("Courier",11);
        break;
    case Digit:
        return QFont("Courier",11);
        break;
    case Environment:
        return QFont("Courier",11,QFont::Bold);
        break;
    case Part:
        return QFont("Century",11,QFont::Bold);
        break;
    case Chapter:
        return QFont("Century",11,QFont::Bold);
        break;
    case Section:
        return QFont("Century",11,QFont::Bold);
        break;
    case SubSection:
        return QFont("Century",11,QFont::Bold,true);
        break;
    case SubSubSection:
        return QFont("Century",11,QFont::Normal);
        break;
    case Paragraph:
        return QFont("Century",11,QFont::Normal,true);
        break;
    case SubParagraph:
        return QFont("Century",11,QFont::Normal,true);
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

int LatexLexer::getDiffLevel(QString line){
    int diff=line.count("\\begin")-line.count("\\end");
    if(line.contains("\\part")){
        diff++;
    }
    if(line.contains("\\chapter")){
        diff++;
    }
    if(line.contains("\\section")){
        diff++;
    }
    if(line.contains("\\subsection")){
        diff++;
    }
    if(line.contains("\\subsubsection")){
        diff++;
    }
    if(line.contains("\\paragraph")){
        diff++;
    }
    if(line.contains("\\subparagraph")){
        diff++;
    }
    return diff;
}

bool LatexLexer::isHeaderLine(QString line){
    return line.contains("\\begin")||line.contains("\\part")||line.contains("\\chapter")||line.contains("\\section")||line.contains("\\subsection")||line.contains("\\subsubsection")||line.contains("\\paragraph")||line.contains("\\subparagraph");
}

QString LatexLexer::trimComment(QString str){
    return str.left(str.indexOf(QRegularExpression("(?<!\\\\)%")));
}

QString LatexLexer::getLine(int i)
{
    int length=editor()->lineLength(i);
    char *chars=new char[length];
    editor()->SendScintilla(QsciScintilla::SCI_GETLINE, i, chars);
    chars[length-1]='\0';
    QString line = QString(chars);
    free(chars);
    return line;
}
