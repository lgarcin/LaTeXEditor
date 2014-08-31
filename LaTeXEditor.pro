#-------------------------------------------------
#
# Project created by QtCreator 2014-06-27T23:44:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaTeXEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    latexlexer.cpp \
    latexeditor.cpp \
    outline.cpp

HEADERS  += mainwindow.h \
    latexlexer.h \
    latexeditor.h \
    outline.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Qt/5.3/msvc2013_64/lib/ -lqscintilla2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Qt/5.3/msvc2013_64/lib/ -lqscintilla2d
else:unix: LIBS += -L$$PWD/../../Qt/5.3/msvc2013_64/lib/ -lqscintilla2

INCLUDEPATH += $$PWD/../../Qt/5.3/msvc2013_64/include
DEPENDPATH += $$PWD/../../Qt/5.3/msvc2013_64/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Qt/5.3/msvc2013_64_opengl/lib/ -lqscintilla2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Qt/5.3/msvc2013_64_opengl/lib/ -lqscintilla2d
else:unix: LIBS += -L$$PWD/../../Qt/5.3/msvc2013_64_opengl/lib/ -lqscintilla2

INCLUDEPATH += $$PWD/../../Qt/5.3/msvc2013_64_opengl/include
DEPENDPATH += $$PWD/../../Qt/5.3/msvc2013_64_opengl/include
