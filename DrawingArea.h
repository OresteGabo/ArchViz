
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
    int m_normalAttr = 1;

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;

    QPoint m_lastMousePos;
    float m_rotationX = 20.0f;
    float m_rotationY = -30.0f;
    float m_zoomDistance = -200.0f;

    QVector<Shape*> m_shapes;

    QOpenGLDebugLogger *m_logger = nullptr;
    QOpenGLBuffer m_gridVbo;
    int m_modelMatrixUniform = -1;
    int m_viewMatrixUniform = -1;
    int m_projectionMatrixUniform = -1;

    // Lighting uniforms
    int m_lightPosUniform = -1;
    int m_viewPosUniform = -1;
    int m_objectColorUniform = -1;

    // Lighting properties (constants for the scene)
    QVector3D m_lightPosition = QVector3D(100.0f, 200.0f, 100.0f); // Overhead light
    QVector3D m_viewPosition; // Camera position (calculated in paintGL)

    // Material/Lighting constants
    const QVector3D m_ambientColor = QVector3D(0.2f, 0.2f, 0.2f);
    const QVector3D m_diffuseColor = QVector3D(0.7f, 0.7f, 0.7f);
    const QVector3D m_specularColor = QVector3D(1.0f, 1.0f, 1.0f);
    const float m_shininess = 32.0f; // Specular exponent
};


#endif // ARCHIVIZ_DRAWINGAREA_H
