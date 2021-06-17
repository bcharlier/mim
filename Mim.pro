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

INCLUDEPATH = ./Manipulator
HEADERS += Window.h \
    DeformableObject.h \
    Viewer.h \
    Manipulator/PCATools.h \
    Manipulator/Manipulator.h \
    Vec3D.h \
    GLUtilityMethods.h \
    Manipulator/RectangleSelection.h \
    Triangle.h \
    Edge.h \
    Mesh.h \
    OpenGLIncludeQtComp.h
SOURCES += Window.cpp \
    Viewer.cpp \
    Main.cpp \
    GLUtilityMethods.cpp \
    Mesh.cpp
LIBS += -L/usr/lib/x86_64-linux-gnu \
    -lQGLViewer-qt5


