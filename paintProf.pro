QT += core gui
QT += widgets
QT += svg

TARGET = paintProf
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

INCLUDEPATH += ./include

SOURCES += $$files(./src/*.cpp)
HEADERS += $$files(./include/*.h)

FORMS += $$files(./forms/*.ui)


