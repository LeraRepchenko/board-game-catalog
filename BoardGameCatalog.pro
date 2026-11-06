QT += core gui widgets
CONFIG += c++17 console

TARGET = BoardGameCatalog
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gamedelegate.cpp

HEADERS += \
    mainwindow.h \
    gamedelegate.h \
    game.h