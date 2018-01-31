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
QT += network

LIBS += -L/opt/local/lib
LIBS += -lSOIL
LIBS += `guile-config link`

QMAKE_CXXFLAGS += `guile-config compile`


# Input
HEADERS += src/headers/files.h \
           src/headers/renderable.h \
           src/headers/mainwindow.h \
           src/headers/new_obj.h \
           src/headers/renderer.h \
           src/headers/guile_thread.h \
           src/headers/tilesetContainer.h \
           src/generated/propertierbase.h \
           src/generated/tile.h \
           src/generated/layer.h \
           src/generated/root.h \
           src/generated/map.h \
           src/generated/tileset.h \
           src/generated/script.h \
           src/generated/texture.h \
           src/generated/resize_obj.h \
           src/headers/editorController.h \
           src/headers/editorController_guile.h \
           src/headers/tilelistmodel.h \
           src/headers/textureContainer.h \
           src/headers/layerContainer.h \
           src/headers/mapContainer.h \
           src/headers/rootContainer.h \
           src/headers/propertyEditor.h \
           src/headers/script-types.h \
           src/headers/tileview_renderer.h \
           src/headers/doc-server.h \
           src/headers/either.h \
           src/headers/nsValidators.h \
           src/headers/tool.h\
           src/headers/pen.h \
           src/headers/rotater.h \
           src/headers/tool_list.h \
           src/headers/scroll.h \
           src/headers/resize-enums.h \
           src/headers/json.hpp
           
FORMS += \
    src/forms/main.ui 
SOURCES += src/cpp/files.cpp \
           src/cpp/main.cpp \
           src/cpp/mainwindow.cpp \
           src/cpp/new_obj.cpp \
           src/cpp/renderer.cpp \
           src/cpp/guile_thread.cpp \
           src/generated/tile.cpp \
           src/generated/propertierbase.cpp \
           src/generated/layer.cpp \
           src/generated/map.cpp \
           src/generated/root.cpp \
           src/generated/tileset.cpp \
           src/generated/script.cpp \
           src/generated/texture.cpp \
           src/generated/resize_obj.cpp \
           src/cpp/editorController.cpp \
           src/cpp/editorController_guile.cpp \
           src/cpp/tilelistmodel.cpp \
           src/cpp/layerContainer.cpp \
           src/cpp/mapContainer.cpp \
           src/cpp/rootContainer.cpp \
           src/cpp/propertyEditor.cpp \
           src/cpp/textureContainer.cpp \
           src/cpp/doc-server.cpp \
           src/cpp/nsValidators.cpp \
           src/cpp/tileview_renderer.cpp \
           src/cpp/tilesetContainer.cpp \
           src/cpp/pen.cpp \
           src/cpp/tool.cpp \
           src/cpp/rotater.cpp \
           src/cpp/tool_list.cpp \
           src/cpp/scroll.cpp
