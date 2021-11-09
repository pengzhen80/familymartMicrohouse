QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql network websockets 3dinput

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boxmanager.cpp \
    clipboardmanagement.cpp \
    configfilemanagement.cpp \
    dataapimanagement.cpp \
    dbmanagement.cpp \
    itemmanager.cpp \
    jobmanagement.cpp \
    logmanagement.cpp \
    main.cpp \
    mainwindow.cpp \
    modbusmanagement.cpp \
    mylineedit.cpp \
    mynumberdialog.cpp \
    realsencemanager.cpp

HEADERS += \
    boxmanager.h \
    clipboardmanagement.h \
    common.h \
    configfilemanagement.h \
    dataapimanagement.h \
    dbmanagement.h \
    errors.h \
    itemmanager.h \
    jobmanagement.h \
    logmanagement.h \
    mainwindow.h \
    modbusmanagement.h \
    mylineedit.h \
    mynumberdialog.h \
    realsencemanager.h

FORMS += \
    mainwindow.ui \
    mynumberdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/'../../../../../Program Files (x86)/Intel RealSense SDK 2.0/lib/x64/' -lrealsense2

INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/Intel RealSense SDK 2.0/lib/x64'
DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/Intel RealSense SDK 2.0/lib/x64'

INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/Intel RealSense SDK 2.0/include'

unix|win32: LIBS += -L$$PWD/../../../../../msys64/mingw64/lib/ -llibmodbus.dll

INCLUDEPATH += $$PWD/../../../../../msys64/mingw64/include
DEPENDPATH += $$PWD/../../../../../msys64/mingw64/include

unix|win32: LIBS += -L$$PWD/../../../../../msys64/mingw64/lib/ -lcrypto

INCLUDEPATH += $$PWD/../../../../../msys64/mingw64/include
DEPENDPATH += $$PWD/../../../../../msys64/mingw64/include

unix|win32: LIBS += -L$$PWD/../../../../../msys64/mingw64/lib/ -lssl

INCLUDEPATH += $$PWD/../../../../../msys64/mingw64/include
DEPENDPATH += $$PWD/../../../../../msys64/mingw64/include

DISTFILES += \
    pics/customerTake_off.png \
    pics/customerTake_on.png \
    pics/multiStore_off.png \
    pics/multiStore_on.png \
    pics/multistore_store.png \
    pics/pick-1.png \
    pics/sevenday_off.png \
    pics/sevenday_on.png \
    pics/takepage_cancel.png \
    pics/takepage_finish.png \
    pics/takepage_tips.png \
    pics/tip_boxMoving.png

RESOURCES += \
    myImage.qrc
