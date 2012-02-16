#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QGLWidget>
#include <QtOpenGL>

class GLCanvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLCanvas(QWidget *parent = 0);
    void update();

private:

protected:
    void resizeGL(int w, int h);
    void paintGL();

signals:

public slots:

};

#endif // GLCANVAS_H
