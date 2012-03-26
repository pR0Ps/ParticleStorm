#include <QtGui/QApplication>
#include <QtOpenGL>
#include "mainwindow.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    if (!QGLFormat::hasOpenGL() || !QGLFramebufferObject::hasOpenGLFramebufferObjects()) {
        QMessageBox::information(0, "OpenGL framebuffer objects 2", "This system does not support OpenGL/framebuffer objects.");
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
