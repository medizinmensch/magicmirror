QT += core gui
QT += charts
QT += network

HEADERS += \
    themewidget.h \
    weatherdatapoint.h \
    weatherforecast.h

SOURCES += \
    main.cpp \
    themewidget.cpp \
    weatherdatapoint.cpp \
    weatherforecast.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target

FORMS += \
    themewidget.ui
