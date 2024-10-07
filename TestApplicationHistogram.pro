QT += quick core widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        filereader.cpp \
        main.cpp \
        qmlplot.cpp \
        uimanager.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    TestApplicationHistogram_ru_RU.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    defines.h \
    filereader.h \
    qmlplot.h \
    uimanager.h

DEFINES += QCUSTOMPLOT_USE_LIBRARY

    # Link with debug version of qcustomplot if compiling in debug mode, else with release library:
    CONFIG(debug, release|debug) {
      win32:QCPLIB = CustomPlot/qcustomplotd2
      else: QCPLIB = CustomPlot/qcustomplotd
    } else {
      win32:QCPLIB = qcustomplot2
      else: QCPLIB = qcustomplot
    }
    INCLUDEPATH += $$PWD/CustomPlot
LIBS += -L$$PWD\ -l$$QCPLIB
