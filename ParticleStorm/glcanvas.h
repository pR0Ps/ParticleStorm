#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QGLWidget>
#include <QtOpenGL>

class GLCanvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLCanvas(QWidget *parent = 0);

private:
    int coords[6];
    int vels[6];
    int max_screen;

protected:
    void resizeGL(int w, int h);
    void paintGL();

signals:

public slots:
    void update();

};

#endif // GLCANVAS_H
