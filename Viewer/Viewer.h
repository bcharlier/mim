/****************************************************************************


*****************************************************************************/

#ifndef MESHERVIEWER_H
#define MESHERVIEWER_H

#include "OpenGLIncludeQtComp.h"

#include "Objects/Mesh.h"
#include "Objects/Curve.h"

enum Mode {
    DIM2D = 0, DIM3D = 1
};

enum DisplayMode {
    WIRE = 0, SOLID = 1, LIGHTED_WIRE = 2, LIGHTED = 3
};

#include <QColor>
#include <QOpenGLFunctions>
#include <QGLViewer/qglviewer.h>
#include <QString>


#include "PCATools.h"
#include "Manipulator.h"
#include "RectangleSelection.h"
#include "Utils/Vec3D.h"

#include "GLUtilityMethods.h"

using namespace qglviewer;

class Viewer : public QGLViewer, public QOpenGLFunctions {
    Q_OBJECT
public :
    Viewer(QWidget *parent);

    ~Viewer();


    void open(const QString &fileName);

    void openModel(const QString &filename);

    void saveOFF(const QString &filename);

    void openCamera(const QString &filename);

    void saveCamera(const QString &filename);


protected :
    virtual void init();

    virtual void draw();

    virtual void keyPressEvent(QKeyEvent *e);

    virtual void mousePressEvent(QMouseEvent *e);

    virtual void mouseMoveEvent(QMouseEvent *e);

    virtual void mouseReleaseEvent(QMouseEvent *e);

    virtual QString helpString() const;

    void initLightsAndMaterials();

    void drawNormals();

    void updateViewer();

    void clear();

    void updateCamera(const Vec3Df &center, float radius);

    void changeDisplayMode();

    void restaureLastState();

    DisplayMode displayMode;

    SimpleManipulator *manipulator;
    RectangleSelection *rselection;

    std::deque <std::vector<Vec3Df>> Q;

    Mesh mesh;
    Curve curve;

    Mesh model_mesh;
    Curve model_curve;

    double sphereScale;
    double manipulatorScale;

    Mode dimension;

    bool camera_motion_activated;
    QPoint previous_mouse_position;
    qglviewer::Vec camera_position;
    qglviewer::Vec int_mouse_w_position;



public slots :
            void manipulatorReleased();

    void addToSelection(QRectF const &, bool);

    void removeFromSelection(QRectF const &);

    void computeManipulatorForDeformation();

    void saveCurrentState();

    void setSphereScale(double _sphereScale) {
        sphereScale = _sphereScale;
        update();
    }

    void setManipulatorScale(double _mScale) {
        manipulatorScale = _mScale;
        update();
    }

    void invertNormals() {
        mesh.invertNormal();
        update();
    }

    void setMode( Mode dim );

    void reset();

};


#endif // MESHERVIEWER_H
