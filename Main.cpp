/****************************************************************************

*****************************************************************************/
#include "Viewer/OpenGLIncludeQtComp.h"
#include "Viewer/Window.h"
#include <qapplication.h>

int main(int argc, char **argv) {
    QApplication application(argc, argv);

    Window vi;

    vi.show();

    return application.exec();
}
