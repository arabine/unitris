QT += quick
CONFIG += c++11
QMAKE_CFLAGS += -std=c99

BASE_DIR = $${PWD}/../..

CONFIG(debug, debug|release) {
debug:      DESTDIR = $$BASE_DIR/build-unitris/debug
}

CONFIG(release, debug|release) {
release:    DESTDIR = $$BASE_DIR/build-unitris/release
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    UnitrisWrapper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

RC_FILE += unitris.rc

HEADERS += \
    UnitrisWrapper.h

# ==========================================================================
# Unitris Engine
# ==========================================================================

VPATH += $$BASE_DIR/components/unitris
INCLUDEPATH += $$BASE_DIR/components/unitris

SOURCES += unitris.c
HEADERS += unitris.h

# ==========================================================================
# EGL library
# ==========================================================================

VPATH += $$BASE_DIR/components/egl
INCLUDEPATH += $$BASE_DIR/components/egl

SOURCES += egl.c characters_map.c
HEADERS += egl.h

# ==========================================================================
# custom printf library
# ==========================================================================

VPATH += $$BASE_DIR/components/sys_printf
INCLUDEPATH += $$BASE_DIR/components/sys_printf

SOURCES += sys_printf.c
HEADERS += sys_printf.h

# ==========================================================================
# Entity: lcd monochrome render
# ==========================================================================

VPATH += $$BASE_DIR/entities/render_lcd_mono
INCLUDEPATH += $$BASE_DIR/entities/render_lcd_mono

SOURCES += render_lcd_mono.c
HEADERS += render_lcd_mono.h

# ==========================================================================
# BMP library
# ==========================================================================
VPATH += $$BASE_DIR/components/bmplib
INCLUDEPATH += $$BASE_DIR/components/bmplib

SOURCES += bmplib.c
HEADERS += bmplib.h

# ==========================================================================
# Debug library
# ==========================================================================
VPATH += $$BASE_DIR/components/debug
INCLUDEPATH += $$BASE_DIR/components/debug

SOURCES += debug.c
HEADERS += debug.h
