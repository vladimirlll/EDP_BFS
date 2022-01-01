QT       += core gui widgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    demo.cpp \
    edge.cpp \
    graph.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    testing.cpp \
    testresult.cpp \
    testwgt.cpp \
    theory.cpp

HEADERS += \
    demo.h \
    edge.h \
    graph.h \
    mainwindow.h \
    node.h \
    testing.h \
    testresult.h \
    testwgt.h \
    theory.h

FORMS += \
    demo.ui \
    mainwindow.ui \
    testing.ui \
    testwgt.ui \
    theory.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
