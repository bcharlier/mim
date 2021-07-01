TEMPLATE = app
TARGET = mim
MOC_DIR = ./moc
OBJECTS_DIR = ./obj
DEPENDPATH += ./Manipulator
DISTFILES += *.png
QT *= xml \
    opengl
CONFIG -= debug \
    debug_and_release
CONFIG += release \
    qt \
    opengl \
    warn_on \
    thread \
    rtti \
    console \
    embed_manifest_exe

INCLUDEPATH += ./Manipulator \
    Objects/ \
    Viewer/ \
    Utils/
HEADERS += Viewer/Window.h \
    Objects/Curve.h \
    Objects/DeformableObject.h \
    Objects/DeformableObjectManager.h \
    Viewer/Viewer.h \
    Manipulator/PCATools.h \
    Manipulator/Manipulator.h \
    Utils/Vec3D.h \
    Viewer/GLUtilityMethods.h \
    Manipulator/RectangleSelection.h \
    Objects/Triangle.h \
    Objects/Edge.h \
    Objects/Mesh.h \
    Viewer/OpenGLIncludeQtComp.h
SOURCES += Viewer/Window.cpp \
    Objects/DeformableObjectManager.cpp \
    Objects/Curve.cpp \
    Viewer/Viewer.cpp \
    Main.cpp \
    Viewer/GLUtilityMethods.cpp \
    Objects/Mesh.cpp
LIBS += -L/usr/lib/x86_64-linux-gnu \
    -lQGLViewer-qt5


