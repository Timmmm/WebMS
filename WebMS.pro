#-------------------------------------------------
#
# Project created by QtCreator 2010-06-15T21:19:07
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = WebMS
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

#LIBS += -lboost_regex
LIBS += -static-libgcc -L. -lcrypt

SOURCES += main.cpp \
    Query.cpp \
    Database.cpp \
    cgi/template.cpp \
    cgi/cgi.cpp \
	common/common.cpp \
    Page.cpp \
    Auth.cpp \
    Base64.cpp \
    EpochTime.cpp \
    Sha1.cpp

HEADERS += \
    Query.h \
    Database.h \
    cgi/template.h \
    cgi/cgi.h \
	common/common.h \
    Page.h \
    Auth.h \
    Base64.h \
    EpochTime.h \
    Sha1.h \
    Config.h

OTHER_FILES += \
    Todo.mkd \
    Readme.mkd
