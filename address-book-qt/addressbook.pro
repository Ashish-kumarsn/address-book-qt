QT += core gui sql widgets

CONFIG += c++17

TARGET = addressbook
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/contact.cpp \
    src/databasemanager.cpp \
    src/mainwindow.cpp \
    src/contactdialog.cpp

HEADERS += \
    src/contact.h \
    src/databasemanager.h \
    src/mainwindow.h \
    src/contactdialog.h

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
DESTDIR = build
