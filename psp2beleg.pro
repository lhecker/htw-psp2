# Name: Leonard Hecker
# Studiengruppe: 13/041/61
# MatrNr: XXXXX

QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = psp2beleg
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

DEFINES += "QT_USE_QSTRINGBUILDER"

SOURCES += \
	src/basicvalidatingitemdelegate.cc \
	src/cataloguewidget.cc \
	src/issuewidget.cc \
	src/issuewidget_p.cc \
	src/main.cc \
	src/main_p.cc \
	src/mainwindow.cc \
	src/mainwindow_p.cc \
	src/readerswidget.cc \

HEADERS  += \
	src/basicvalidatingitemdelegate.h \
	src/cataloguewidget.h \
	src/issuewidget.h \
	src/issuewidget_p.h \
	src/main_p.h \
	src/mainwindow.h \
	src/mainwindow_p.h \
	src/readerswidget.h \
