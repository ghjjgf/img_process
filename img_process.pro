QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# 禁用OpenCV并行计算
DEFINES += CV_DISABLE_OPENMP
DEFINES += CV_DISABLE_TBB
DEFINES += CV_DISABLE_ONETBB

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algo/config.cpp \
    algo/function.cpp \
    algo/thread.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    algo/config.h \
    algo/function.h \
    algo/thread.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

CONFIG += lrelease
CONFIG += embed_translations

LIBS += -luser32 -lgdi32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 设置库搜索路径
QMAKE_LIBDIR += G:/enlishpath/opencv-4.8.0/opencv/build/x64/vc16/lib

win32:CONFIG(release, debug|release): LIBS += -lopencv_world480
else:win32:CONFIG(debug, debug|release): LIBS += -lopencv_world480d

# 移除错误的unix平台配置
# else:unix: LIBS += -LG:/enlishpath/opencv-4.8.0/opencv/build/x64/vc16/lib/ -lopencv_world480

INCLUDEPATH += G:/enlishpath/opencv-4.8.0/opencv/build/include
DEPENDPATH += G:/enlishpath/opencv-4.8.0/opencv/build/include

