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

LIBS += -L"./../../libraries/midlib2/gccRelease" \  #Aptina internal only
        -L"./../../libraries/apbase/gccRelease" \   #Aptina internal only
#        -L"./../../lib/linux/x32" \ # use this line for 32-bit Linux (download from Github)
        -L"./../../lib/linux/x64" \ # use this line for 64-bit Linux (download from Github)
#        -L"./../../lib/macos" \     # use this line for MacOS X (download from Github)
        -lusb-1.0 -lmidlib2 -lapbase -lpython3.3m

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/Release
OBJECTS_DIR += Release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(./SimpleQt/SimpleQt.pri)
win32:RC_FILE = ./SimpleQt/SimpleQt.rc
