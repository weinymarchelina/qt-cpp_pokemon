QT       += core gui \
            multimedia \
            widgets \
            multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Item.cpp \
    Move.cpp \
    Pokemon.cpp \
    Trainer.cpp \
    gamewindow.cpp \
    loseresultwindow.cpp \
    main.cpp \
    menuwindow.cpp \
    winresultwindow.cpp

HEADERS += \
    Item.h \
    Move.h \
    Pokemon.h \
    Trainer.h \
    gamewindow.h \
    loseresultwindow.h \
    menuwindow.h \
    winresultwindow.h

FORMS += \
    gamewindow.ui \
    loseresultwindow.ui \
    menuwindow.ui \
    winresultwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
