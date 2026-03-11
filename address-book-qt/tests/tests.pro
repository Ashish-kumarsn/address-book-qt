QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_addressbook

INCLUDEPATH += ../src

SOURCES += \
    tst_contact.cpp \
    ../src/contact.cpp \
    ../src/databasemanager.cpp

HEADERS += \
    ../src/contact.h \
    ../src/databasemanager.h

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
DESTDIR = build
