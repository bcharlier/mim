/****************************************************************************
*****************************************************************************/

#include "Viewer.h"

#include <QOpenGLContext>

#if QT_VERSION >= 0x040000
# include <QKeyEvent>
#endif


using namespace qglviewer;

static unsigned int max_operation_saved = 10;

Viewer::Viewer(QWidget *parent) : QGLViewer(parent) {}

Viewer::~Viewer() {}

void Viewer::init() {
    makeCurrent();

    initializeOpenGLFunctions();

    // Absolutely needed for MouseGrabber
    setMouseTracking(true);

    setBackgroundColor(QColor(255, 255, 255));

    //restoreStateFromFile();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    manipulator = new SimpleManipulator;

    //connect(manipulator , SIGNAL(moved()) , this , SLOT(updateFromCMInterface())); //TODO if interactive motion of selected points
    connect(manipulator, SIGNAL(mouseReleased()), this, SLOT(manipulatorReleased()));

    rselection = new RectangleSelection;
    connect(rselection, SIGNAL(add(QRectF, bool)), this, SLOT(addToSelection(QRectF, bool)));
    connect(rselection, SIGNAL(remove(QRectF)), this, SLOT(removeFromSelection(QRectF)));
    connect(rselection, SIGNAL(apply()), this, SLOT(computeManipulatorForDeformation()));

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    initLightsAndMaterials();

    setKeyDescription(Qt::Key_D, "Change display mode");

    displayMode = LIGHTED;

    sphereScale = 0.5;
    manipulatorScale = 1.;

    dimension = DIM3D;

    camera_motion_activated = false;

}

void Viewer::initLightsAndMaterials() {

    GLTools::initLights();
    GLTools::setDefaultMaterial();
    GLTools::setSunriseLight();

}


void Viewer::mousePressEvent(QMouseEvent *e) {

    makeCurrent();

    if ((e->modifiers() & Qt::ShiftModifier)) {

        if (rselection->isInactive()) {
            rselection->activate();
        }
        rselection->mousePressEvent(e, camera());
        update();
        return;

    } else if ((e->button() == Qt::MidButton) && (e->modifiers() & Qt::ControlModifier)) {
        manipulator->clear();
        manipulator->setDisplayScale(manipulatorScale * camera()->sceneRadius() / 9.);

    } else if ((e->button() == Qt::LeftButton) && (e->modifiers() & Qt::ControlModifier)) {
        bool found;
        qglviewer::Vec point = camera()->pointUnderPixel(e->pos(), found);
        if (found) {
            manipulator->clear();
            manipulator->setDisplayScale(manipulatorScale * camera()->sceneRadius() / 9.);
            manipulator->setOrigine(point);
            manipulator->activate();
        }
    } else if ((e->button() == Qt::RightButton) && (e->modifiers() & Qt::ControlModifier)) {
        bool found;

        qglviewer::Vec point = camera()->pointUnderPixel(e->pos(), found);
        if (found) {
            manipulator->clear();
            manipulator->setDisplayScale(manipulatorScale * camera()->sceneRadius() / 9.);
        }
    }


    if( dimension == DIM2D ){

        if( (e->button() == Qt::RightButton) && !camera_motion_activated ){
            camera_motion_activated = true;
            previous_mouse_position = e->pos();
            bool found;
            int_mouse_w_position = camera()->pointUnderPixel(previous_mouse_position, found);
            if( !found ) std::cout << "DIM2D::Init pb" <<std::endl;
            camera_position = camera()->position();
        } else {
            manipulator->mousePressEvent(e, camera());
            update();
        }
    } else {
        QGLViewer::mousePressEvent(e);
    }
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {

    if (!rselection->isInactive()) {

        if (rselection->isInactive()) {
            rselection->activate();
        }
        rselection->mouseMoveEvent(e, camera());
        update();
        return;
    }

    if( dimension == DIM3D ){
        QGLViewer::mouseMoveEvent(e);
    } else {

        if( camera_motion_activated ){

            QPoint delta = previous_mouse_position - e->pos();
            qglviewer::Vec direction (delta.x(), -delta.y(), 0.);

            direction =  0.05*camera()->flySpeed()*direction ;

            bool found;
            qglviewer::Vec point = camera()->pointUnderPixel(e->pos(), found);
            if (found) {
                direction = camera()->pointUnderPixel(previous_mouse_position, found) - point;
            } else std::cout << "DIM2D::MouseMove pb" <<std::endl; //TODO fix


            qglviewer::Vec cam_pos = camera_position + direction;
            camera()->setPosition(cam_pos);
            camera()->lookAt( qglviewer::Vec ( cam_pos.x, cam_pos.y, 0. ) );
            update();
        } else {

            QGLViewer::mouseMoveEvent(e);
            update();
        }
    }
}

void Viewer::mouseReleaseEvent(QMouseEvent *e) {

    if (!rselection->isInactive()) {

        if (rselection->isInactive()) {
            rselection->activate();
        }
        rselection->mouseReleaseEvent(e, camera());
        rselection->deactivate();
        update();
        return;

    }

    if( camera_motion_activated ){
        camera_motion_activated = false;
    }

    QGLViewer::mouseReleaseEvent(e);
}

void Viewer::manipulatorReleased() {
    saveCurrentState();
}

void Viewer::saveCurrentState() {

    if (Q.size() == max_operation_saved)
        Q.pop_front();
    //TODO define state i.e. current vertices positions?

}


void Viewer::restaureLastState() {

    if (Q.size() > 0) {

        if (Q.size() > 1)
            Q.pop_back();

        //TODO define state to restore

        if (Q.size() > 1)
            Q.pop_back();
    }

}


void Viewer::addToSelection(QRectF const &zone, bool moving) {

    if (manipulator->getEtat()) manipulator->deactivate();

    float modelview[16];
    camera()->getModelViewMatrix(modelview);
    float projection[16];
    camera()->getProjectionMatrix(projection);

    //TODO
}

void Viewer::removeFromSelection(QRectF const &zone) {
    if (manipulator->getEtat()) manipulator->deactivate();

    float modelview[16];
    camera()->getModelViewMatrix(modelview);
    float projection[16];
    camera()->getProjectionMatrix(projection);

    //TODO remove from selection
}

void Viewer::computeManipulatorForDeformation() {
    //TODO
}

void Viewer::clear() {

    curve = Curve();
    mesh = Mesh();

    manipulator->deactivate();

    Q.clear();

}


void Viewer::open(const QString &filename) {

    //clear();


    if (filename.endsWith(".off")) {
        mesh.clear();
        std::vector <Vec3Df> &vertices = mesh.getVertices();
        std::vector < Triangle > &triangles = mesh.getTriangles();

        FileIO::openOFF(filename.toStdString(), vertices, triangles);
        mesh.update();

    } else if (filename.endsWith(".obj")) {
        mesh.clear();
        std::vector <Vec3Df> &vertices = mesh.getVertices();
        std::vector < Triangle > &triangles = mesh.getTriangles();

        FileIO::objLoader(filename.toStdString(), vertices, triangles);
        mesh.update();

    } else if (filename.endsWith(".cff")) {
        curve.clear();
        std::vector <Vec3Df> &vertices = curve.getVertices();
        std::vector < Edge > &edges = curve.getTriangles();

        FileIO::openCFF(filename.toStdString(), vertices, edges);
        curve.update();
    } else {
        std::cout << "Viewer::openMesh::Unsupported mesh file format " << std::endl;
    }

    updateViewer();

}

void Viewer::openModel(const QString &filename) {


    if (filename.endsWith(".off")) {
        model_mesh.clear();
        std::vector <Vec3Df> &vertices = model_mesh.getVertices();
        std::vector < Triangle > &triangles = model_mesh.getTriangles();

        FileIO::openOFF(filename.toStdString(), vertices, triangles);
        model_mesh.update();

    } else if (filename.endsWith(".obj")) {
        model_mesh.clear();
        std::vector <Vec3Df> &vertices = model_mesh.getVertices();
        std::vector < Triangle > &triangles = model_mesh.getTriangles();

        FileIO::objLoader(filename.toStdString(), vertices, triangles);
        model_mesh.update();

    } else if (filename.endsWith(".cff")) {
        model_curve.clear();
        std::vector <Vec3Df> &vertices = model_curve.getVertices();
        std::vector < Edge > &edges = model_curve.getTriangles();

        FileIO::openCFF(filename.toStdString(), vertices, edges);
        model_curve.update();
    } else {
        std::cout << "Viewer::openModel::Unsupported mesh file format " << std::endl;
    }


    update();
}


void Viewer::saveCamera(const QString &filename) {
    std::ofstream out(filename.toUtf8());
    if (!out)
        exit(EXIT_FAILURE);

    out << camera()->position() << " " <<
           camera()->viewDirection() << " " <<
           camera()->upVector() << " " <<
           camera()->fieldOfView();
    out << std::endl;
    out.close();
}

std::istream &operator>>(std::istream &stream, qglviewer::Vec &v) {
    stream >>
            v.x >>
            v.y >>
            v.z;

    return stream;
}

void Viewer::openCamera(const QString &filename) {

    std::ifstream file;
    file.open(filename.toStdString());

    qglviewer::Vec pos;
    qglviewer::Vec view;
    qglviewer::Vec up;
    float fov;

    file >> pos >>
            view >>
            up >>
            fov;

    camera()->setPosition(pos);
    camera()->setViewDirection(view);
    camera()->setUpVector(up);
    camera()->setFieldOfView(fov);

    camera()->computeModelViewMatrix();
    camera()->computeProjectionMatrix();

    update();
}

void Viewer::saveOFF(const QString &filename) {
    FileIO::saveOFF(filename.toStdString(), mesh.getVertices(), mesh.getTriangles());
}


void Viewer::updateViewer() {
    std::cout <<  " radius0 " << std::endl;
    curve.update();
    std::cout <<  " radius0 " << std::endl;
    mesh.update();

    std::vector <Vec3Df> &vertices_curve = curve.getVertices();
    //std::vector <Vec3Df> vertices_curve;
    std::vector <Vec3Df> &vertices = mesh.getVertices();

    std::cout <<  " radius " << std::endl;
    std::cout <<  " nb_vet_curve " << vertices_curve.size() <<  std::endl;
    std::cout <<  " nb_vert " << vertices.size() <<  std::endl;
    std::vector <Vec3Df> tmp = vertices_curve;
    tmp.insert(
                tmp.end(),
                std::make_move_iterator(vertices.begin()),
                std::make_move_iterator(vertices.end())
                );
    /*
    Vec3Df err; err[0] = 0.; err[1] = 0.; err[2] = 0.;
    for(auto i=0; i<  vertices_curve.size() + vertices.size(); ++i) {
        std::cout << (tmp[i] - tmp2[i]) *(tmp[i] - tmp2[i]) << std::endl;
        err += (tmp[i] - tmp2[i]) *(tmp[i] - tmp2[i])  ;
    }
    std:cout << err << " err " << std::endl;
*/
    Vec3Df center;
    double radius;
    MeshTools::computeAveragePosAndRadius(tmp, center, radius);


    //std::vector <Edge> &edges = curve.getTriangles();
    //std::vector <Triangle> &triangle = mesh.getTriangles();
    //std::cout << center << " radius " << radius << std::endl;
    //for( unsigned int i = 0 ; i < edges.size() ; i++ )
    //    std::cout << edges[i][0] << " ; " <<edges[i][1]<< std::endl;

    updateCamera(center, radius);

    manipulator->clear();
    manipulator->setDisplayScale(manipulatorScale * camera()->sceneRadius() / 9.);

    saveCurrentState();

    double average = radius / 10.;
    sphereScale = camera()->sceneRadius() * 0.01 / average;
    if (sphereScale > 1.) sphereScale = 1.;

}

void Viewer::updateCamera(const Vec3Df &center, float radius) {
    camera()->setSceneCenter(Vec(center[0], center[1], center[2]));
    camera()->setSceneRadius(radius * 2.);
    manipulator->setDisplayScale(manipulatorScale * radius / 3.);

    camera()->showEntireScene();
}

void Viewer::reset() {

    //TODO

}


void Viewer::draw() {

    if( dimension == DIM2D ){
        glDisable(GL_LIGHTING);
        glColor3f(1.f,1.f,1.f);
        glBegin(GL_QUADS);
        glVertex3f(-100.f, -100.f, -0.0001f);
        glVertex3f(-100.f, 100.f, -0.0001f);
        glVertex3f(100.f, 100.f, -0.0001f);
        glVertex3f(100.f, -100.f, -0.0001f);
        glEnd();
        glEnable(GL_LIGHTING);
    }

    if (displayMode == LIGHTED || displayMode == LIGHTED_WIRE) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    } else if (displayMode == WIRE) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);

    } else if (displayMode == SOLID) {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

    glLineWidth(5.f);
    glColor3f(0., 0., 0.);
    glDisable(GL_LIGHTING);
    curve.draw();
    glColor3f(0.37, 0.82, 0.55);
    model_curve.draw();

    glEnable(GL_LIGHTING);
    glColor3f(0.37,0.55,0.82);
    mesh.draw();
    glColor3f(0.37, 0.82, 0.55);
    model_mesh.draw();

    if (displayMode == SOLID || displayMode == LIGHTED_WIRE) {
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glPolygonOffset(-1.0, 1.0);

        glColor3f(0., 0., 0.);
        curve.draw();
        mesh.draw();
        model_curve.draw();
        model_mesh.draw();

        glDisable(GL_POLYGON_OFFSET_LINE);
        glEnable(GL_LIGHTING);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    manipulator->draw();
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    rselection->draw();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

}

void Viewer::changeDisplayMode() {
    if (displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if (displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if (displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
    update();
}


void Viewer::setMode( Mode dim ){
    dimension = dim;
    if( dimension == DIM3D ) {
        camera()->setType(Camera::PERSPECTIVE);
        manipulator->setDim3D();
        updateViewer();
    }
    else {
        camera()->setPosition(qglviewer::Vec(0.,0.,1));
        camera()->lookAt(qglviewer::Vec(0.,0.,0.));
        camera()->setUpVector(qglviewer::Vec(0.,1.,0.));
        camera()->setType(Camera::ORTHOGRAPHIC);
        manipulator->setDim2D();
        updateViewer();
    }

    update();
}

void Viewer::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_D :
        changeDisplayMode();
        break;
    case Qt::Key_Z :
        if (e->modifiers() & Qt::ControlModifier) restaureLastState();
        update();
        break;
    default :
        QGLViewer::keyPressEvent(e);
    }
}

QString Viewer::helpString() const {
    QString text("<h2>KidPocket Framework</h2>");

    text += "This application allows to deform a mesh using </br> <b>ARAP*</b>.";

    text += "<p>";

    text += "<h3>Interaction</h3>";
    text += "<p>";
    text += "<h4>Basics</h4>";
    text += "<ul>";
    text += "<li><b>H</b>   :   make this help appear.</li>";
    text += "<li><b>D</b>   :   change display mode.</li>";
    text += "<li><b>Ctrl + Q</b>   :   close the application.</li>";
    text += "</ul>";
    text += "<h4>Open</h4>";
    text += "<ul>";
    text += "<li><b>Ctrl + O</b>    :    open a file containing a surface mesh (*.off, *.obj).</li>";
    text += "</ul>";
    text += "<h4>Save</h4>";
    text += "<ul>";
    text += "<li><b>Ctrl + S</b>    :    save deformed surface mesh (*.off).</li>";
    text += "</ul>";
    text += "<h4>Deformation</h4>";
    text += "<ul>";
    text += "<li><b>Ctrl + mouse right</b>    :    select/unselect as moving handles cage vertices</li>";
    text += "<li><b>Ctrl + mouse left</b>    :    change vertices handle type: fixed to moving or moving to fixed</li>";
    text += "<li><b>Ctrl + mouse middle</b>    :    change all moving handle vertices to fixed</li>";
    text += "<li><b>Ctrl + A</b>    :    set all free vertices to moving handles</li>";
    text += "<li><b>Ctrl + Shift + A</b>    :    set all free vertices to fixed handles</li>";
    text += "<li><b>Ctrl + U</b>    :    unselect all moving handles</li>";
    text += "<li><b>Ctrl + Shift + U</b>    :    unselect all fixed handles</li>";
    text += "<li><b>Shift + mouse right click</b>    :    manipulate the selected vertices</li>";
    text += "<li><b>Mouse middle on active manipulator</b>    :    change manipulator type</li>";
    text += "<li><b>Ctrl + Z</b>    :    cancel last deformation</li>";
    text += "</ul>";
    text += "</p>";

    text += "<h3>User guide</h3>";
    text += "<p>";
    text += "<h4>Pipeline</h4>";
    text += "<ul>";
    text += "<li>Load a mesh (*.off).</li>";
    text += "<li>Activate deformation tools.</li>";
    text += "<li>Select cage vertices. <ul><li>Fixed handles: red spheres.</li> <li>Moving handles: green spheres.</li> </ul></li>";
    text += "<li>Compute manipulator (Shift+right click).</li>";
    text += "<li>Operate deformations and iterate last 2 steps until getting the desired result. <b>WARNING:</b> Do not forget to unselect cage vertices before deforming a different area of the model.</li>";
    text += "<li>Save model.</li>";
    text += "</ul>";
    text += "<h4>Remarks</h4>";
    text += "<ul>";
    text += "<li>When you open a file there will be a latence time due to the initialization of ARAP.</li>";
    text += "<li>To deform the model, use the 'Rectangle' selection tool to select the moving handles of cage vertices, by using the mouse while keeping 'Shift' pressed (discussed before).</li>";
    text += "<li>To unselect vertices, use the 'Rectangle' selection tool by using the mouse while keeping 'Ctrl + Shift' pressed.</li>";
    text += "<li>To disable the manipulation tool, right click on it.</li>";
    text += "</ul>";
    text += "</p>";

    text += "<h3>Biblio</h3>";
    text += "<p>";
    text += "(*) : M. Alexa, D. Cohen-Or, D. Levin, As-rigid-as-possible shape 457 interpolation, Proceedings of SIGGRAPH 2000 (2000) 157458 164ISBN 1-58113-208-5.";
    text += "</p>";

    text += "</p>";
    return text;
}
