
TEMPLATE = app
TARGET = SimpleQt
DESTDIR = .
QT += core gui opengl
CONFIG += release
DEFINES += QT_OPENGL_LIB
INCLUDEPATH += ./../../common \
    ./../../libraries \
    ./../../include \
    ./../../libraries/midlib2 \
    ./../../libraries/apbase/apbase \
    ./GeneratedFiles \
    ./GeneratedFiles/Release \
    .
LIBS +=-L"./../../libraries/midlib2/gccRelease" \
    -L"./../../libraries/apbase/gccRelease" \
    -lusb-1.0 -lmidlib2 -lapbase

#LIBS += -L"./../../lib" \
#    -L"./../../libraries/midlib2/gccRelease" \
#    -L"./../../libraries/apbase/gccRelease" \
#    -lGL -lGLU -lusb-1.0 -lmidlib2 -lapbase

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/Release
OBJECTS_DIR += Release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(./SimpleQt/SimpleQt.pri)
win32:RC_FILE = ./SimpleQt/SimpleQt.rc
