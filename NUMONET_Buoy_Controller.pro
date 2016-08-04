QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NUMONET_Buoy_Controller
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    xbee.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    xbee.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

ICON = Images/Modem.icns

DISTFILES +=
