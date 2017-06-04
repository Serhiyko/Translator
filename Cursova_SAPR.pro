#-------------------------------------------------
#
# Project created by QtCreator 2017-05-01T12:14:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cursova_SAPR
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lexicalanalyzer.cpp \
    syntaxanalyzer.cpp \
    reversenotation.cpp \
    calculatepolis.cpp \
    window.cpp

HEADERS  += mainwindow.h \
    lexicalanalyzer.h \
    syntaxanalyzer.h \
    data.h \
    reversenotation.h \
    calculatepolis.h \
    window.h

FORMS    += mainwindow.ui
