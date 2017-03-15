TEMPLATE = app

QT += qml quick
CONFIG += c++11
CONFIG += console

SOURCES += main.cpp \
    algorithms/readgml.cpp \
    algorithms/PageRank.cpp \
    algorithms/FastUnfolding.cpp \
    algorithms/GitGraph.cpp \
    algorithms/GN.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    vsproject/test.VC.db \
    vsproject/test.vcxproj.filters \
    vsproject/x64/Release/test.tlog/CL.command.1.tlog \
    vsproject/x64/Release/test.tlog/CL.read.1.tlog \
    vsproject/x64/Release/test.tlog/CL.write.1.tlog \
    vsproject/x64/Release/test.tlog/link.command.1.tlog \
    vsproject/x64/Release/test.tlog/link.read.1.tlog \
    vsproject/x64/Release/test.tlog/link.write.1.tlog \
    vsproject/x64/Release/test.tlog/test.lastbuildstate \
    vsproject/x64/Release/test.tlog/unsuccessfulbuild \
    vsproject/test.sln \
    vsproject/test.vcxproj \
    vsproject/x64/Release/test.Build.CppClean.log \
    vsproject/x64/Release/test.log

HEADERS += \
    algorithms/FastUnfolding.h \
    algorithms/GitGraph.h \
    algorithms/GN.h \
    algorithms/network.h \
    algorithms/PageRank.h \
    algorithms/readgml.h \
    algorithms/qgraph.h
