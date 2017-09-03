######################################################################
# Automatically generated by qmake (3.1) Sun Feb 26 14:09:38 2017
######################################################################

TEMPLATE = app
TARGET = qt-test
CONFIG += debug
INCLUDEPATH += .
INCLUDEPATH += ./src/headers
INCLUDEPATH += ./src/ui
INCLUDEPATH += ./src/generated
INCLUDEPATH += /opt/local/include
QT += widgets
QT += opengl

LIBS += -L/opt/local/lib
LIBS += -lSOIL
LIBS += `guile-config link`

QMAKE_CXXFLAGS += `guile-config compile`


# Input
HEADERS += src/headers/files.h \
           src/headers/gl_apu.h \
           src/headers/mainwindow.h \
           src/headers/obj.h \
           src/headers/renderer.h \
           src/headers/shaders.h \
           src/headers/texture.h \
           src/headers/guile_thread.h \
           src/generated/propertierbase.h \
           src/generated/tile.h \
           src/generated/layer.h \
           src/generated/root.h \
           src/generated/map.h \
           src/headers/editorController.h \
           src/headers/tilelistmodel.h \
           src/headers/simplelistmodel.h \
           src/headers/layerContainer.h \
           src/headers/mapContainer.h \
           src/headers/rootContainer.h 
FORMS += src/ui/main.ui
SOURCES += src/cpp/files.cpp \
           src/cpp/gl_apu.cpp \
           src/cpp/main.cpp \
           src/cpp/mainwindow.cpp \
           src/cpp/obj.cpp \
           src/cpp/renderer.cpp \
           src/cpp/shaders.cpp \
           src/cpp/guile_thread.cpp \
           src/generated/tile.cpp \
           src/generated/propertierbase.cpp \
           src/generated/layer.cpp \
           src/generated/map.cpp \
           src/generated/root.cpp \
           src/cpp/editorController.cpp \
           src/cpp/tilelistmodel.cpp \
           src/cpp/simplelistmodel.cpp \
           src/cpp/layerContainer.cpp \
           src/cpp/mapContainer.cpp \
           src/cpp/rootContainer.cpp 
