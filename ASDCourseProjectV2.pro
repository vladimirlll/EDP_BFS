QT       += core gui widgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Demo/demo.cpp \
    Demo/demo.cpp \
    Demo/edge.cpp \
    Demo/edge.cpp \
    Demo/graph.cpp \
    Demo/graph.cpp \
    Demo/node.cpp \
    Demo/node.cpp \
    Exceptions/filehandleexception.cpp \
    Testing/question.cpp \
    Testing/test.cpp \
    Testing/testing.cpp \
    Testing/testresult.cpp \
    Testing/testwgt.cpp \
    Theory/theory.cpp \
    Theory/theorypage.cpp \
    Theory/theorywidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Demo/demo.h \
    Demo/demo.h \
    Demo/edge.h \
    Demo/edge.h \
    Demo/graph.h \
    Demo/graph.h \
    Demo/node.h \
    Demo/node.h \
    Exceptions/filehandleexception.h \
    Testing/question.h \
    Testing/test.h \
    Testing/testing.h \
    Testing/testresult.h \
    Testing/testwgt.h \
    Theory/theory.h \
    Theory/theorypage.h \
    Theory/theorywidget.h \
    mainwindow.h

FORMS += \
    Demo/demo.ui \
    Demo/demo.ui \
    Testing/testing.ui \
    Testing/testwgt.ui \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
