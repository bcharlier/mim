/****************************************************************************


*****************************************************************************/

#ifndef MESHERVIEWER_H
#define MESHERVIEWER_H
#include "OpenGLIncludeQtComp.h"

#include "Mesh.h"


enum Mode {MESHING, MESH_DEFORMATION, IMAGE_DEFORMATION};

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };

#include <QColor>
#include <QOpenGLFunctions>
#include <QGLViewer/qglviewer.h>
#include <QString>


#include "PCATools.h"
#include "Manipulator.h"
#include "RectangleSelection.h"
#include "Vec3D.h"

#include "GLUtilityMethods.h"

using namespace qglviewer;

class Viewer : public QGLViewer, public QOpenGLFunctions
{
    Q_OBJECT
public :
    Viewer(QWidget *parent);
    ~Viewer();


    void open(const QString & fileName);
    void openModel (const QString & filename);
    void saveOFF (const QString & filename);

    void openCamera (const QString & filename);
    void saveCamera(const QString & filename);


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

    void updateCamera(const Vec3Df & center, float radius);

    void changeDisplayMode();

    void restaureLastState();

    DisplayMode displayMode;

    SimpleManipulator * manipulator;
    RectangleSelection * rselection;

    std::deque< std::vector<Vec3Df> > Q;

    Mesh mesh;

    Mesh model_mesh;
    double sphereScale;
    double manipulatorScale;

public slots :
    void manipulatorReleased();
    void addToSelection(QRectF const &, bool);
    void removeFromSelection(QRectF const &);
    void computeManipulatorForDeformation();
    void saveCurrentState();
    void setSphereScale(double _sphereScale){sphereScale = _sphereScale; update();}
    void setManipulatorScale(double _mScale){manipulatorScale = _mScale; update();}

    void invertNormals(){ mesh.invertNormal(); update(); }
    void reset();

};


#endif // MESHERVIEWER_H
