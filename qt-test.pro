######################################################################
# Automatically generated by qmake (3.1) Sun Feb 26 14:09:38 2017
######################################################################

TEMPLATE = app
TARGET = qt-test
CONFIG += debug
# CONFIG+=c++14
INCLUDEPATH += .
INCLUDEPATH += ./src/headers
INCLUDEPATH += ./src/ui
INCLUDEPATH += /opt/local/include
INCLUDEPATH += ./libzippp/src
QT += widgets
QT += opengl
QT += network
QT += testlib

LIBS += -L/opt/local/lib
LIBS += -lzip
LIBS += -lecl
OBJECTS = ./libzippp/obj/libzippp.o
LIBS += `ecl-config --ldflags`

QMAKE_CXXFLAGS += `ecl-config --cflags`
QMAKE_LFLAGS += `ecl-config --ldflags`

DESTDIR = ./bin

QMAKE_POST_LINK += $$quote(cp ./src/lisp/*.lisp $${DESTDIR} && cp ./src/lisp/*.asd $${DESTDIR})

# Input
HEADERS += src/headers/files.h \
           src/headers/renderable.h \
           src/headers/mainwindow.h \
           src/headers/new_obj.h \
           src/headers/renderer.h \
           src/headers/guile_thread.h \
           src/headers/editorController.h \
           src/headers/editorController_guile.h \
           src/headers/propertyEditor.h \
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
           src/headers/animationLoaderUi.h \
           src/headers/spriteMover.h \
           src/headers/spriteRotater.h \
           src/headers/engine.h \
           src/headers/engine_renderer.h \
           src/headers/guile_qt_keymapping.h \
           src/headers/guile_fn.h \
           src/headers/holder.h \
           src/headers/new_map_ui.h \
           src/headers/resize-form.h \
           src/headers/selection.h \
           src/headers/colored_rect.h
           
FORMS += \
    src/forms/main.ui \
    src/forms/resize_ui.ui \
    src/forms/animationloader.ui \
    src/forms/engine.ui \
    src/forms/new_map_ui.ui
    
SOURCES += src/cpp/files.cpp \
           src/cpp/main.cpp \
           src/cpp/mainwindow.cpp \
           src/cpp/new_obj.cpp \
           src/cpp/renderer.cpp \
           src/cpp/guile_thread.cpp \
           src/cpp/editorController.cpp \
           src/cpp/editorController_guile.cpp \
           src/cpp/propertyEditor.cpp \
           src/cpp/doc-server.cpp \
           src/cpp/nsValidators.cpp \
           src/cpp/tileview_renderer.cpp \
           src/cpp/pen.cpp \
           src/cpp/tool.cpp \
           src/cpp/rotater.cpp \
           src/cpp/tool_list.cpp \
           src/cpp/scroll.cpp \
           src/cpp/animationLoaderUi.cpp \
           src/cpp/spriteMover.cpp \
           src/cpp/spriteRotater.cpp \
           src/cpp/engine.cpp \
           src/cpp/engine_renderer.cpp \
           src/cpp/guile_qt_keymapping.cpp \
           src/cpp/guile_fn.cpp \
           src/cpp/holder.cpp \
           src/cpp/new_map_ui.cpp \
           src/cpp/resize-form.cpp \
           src/cpp/selection.cpp \
           src/cpp/colored_rect.cpp
