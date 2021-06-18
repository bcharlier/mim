#ifndef WINDOW_H
#define WINDOW_H


#include <QMainWindow>
#include "Viewer.h"

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window();

private:
    Viewer *viewer;
    QDockWidget *mesherDockWidget;

    void initActions();

    void initFileActions();

    void initMenus();

    void initToolBars();

    QActionGroup *fileActionGroup;
    QToolBar *fileToolBar;

    void initDisplayDockWidgets();

    QGroupBox *displayGroupBox;

    QTabWidget *contents;

private
    slots:

            void saveMesh();

    void open();

    void openMeshModel();

    void saveCamera();

    void openCamera();

};


#endif // WINDOW_H

