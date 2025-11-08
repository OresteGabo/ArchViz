
#ifndef ARCHIVIZ_DRAWINGAREA_H
#define ARCHIVIZ_DRAWINGAREA_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QPoint>
#include <QVector3D>
#include <QOpenGLDebugLogger>

#include "Cube.h"
#include "CompositeObject.h"


class DrawingArea : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);
    ~DrawingArea() override;


protected:
    // Qt OpenGL lifecycle
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // User input
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    // helpers
    void initializeShaders();
    void drawGrid();
    void drawAxes();
    void drawCube();
    void ensureCubeInitialized();
    void drawShapes();

private:
    QOpenGLShaderProgram m_program;
    int m_posAttr = 0;
    int m_matrixUniform = -1;
    int m_colorUniform = -1;

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;

    QPoint m_lastMousePos;
    float m_rotationX = 20.0f;
    float m_rotationY = -30.0f;
    float m_zoomDistance = -200.0f;

    QVector<Shape*> m_shapes;

    QOpenGLDebugLogger *m_logger = nullptr;
};


#endif // ARCHIVIZ_DRAWINGAREA_H
