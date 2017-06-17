TARGET = RTKUSER

CONFIG += \
    c++14 \
    static

QT += \
    core \
    gui \
    widgets

SOURCES += \
    src/main.cpp \
    src/window.cpp

HEADERS += \
    src/window.h

FORMS += \
    src/window.ui

copydata.commands = $$QMAKE_COPY $$PWD/configs.conf $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
