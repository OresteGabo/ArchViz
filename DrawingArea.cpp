#include "DrawingArea.h"
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector4D>
#include <QDebug>

// Simple shaders (position + uniform color)
static const char *vertexShaderSource =
    "attribute vec3 position;\n"
    "uniform mat4 matrix;\n"
    "void main() {\n"
    "  gl_Position = matrix * vec4(position, 1.0);\n"
    "}\n";

static const char *fragmentShaderSource =
    "uniform vec4 color;\n"
    "void main() {\n"
    "  gl_FragColor = color;\n"
    "}\n";

DrawingArea::DrawingArea(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

DrawingArea::~DrawingArea()
{
    // Must make the context current before destroying GL resources
    makeCurrent();

    if (m_testCube) {
        delete m_testCube;
        m_testCube = nullptr;
    }

    if (m_logger) {
        delete m_logger;
        m_logger = nullptr;
    }

    doneCurrent();
}

void DrawingArea::initializeGL()
{
    // initialize functions for current context
    initializeOpenGLFunctions();

    // optional: setup debug logger (if driver supports it)
    m_logger = new QOpenGLDebugLogger(this);
    if (m_logger->initialize()) {
        connect(m_logger, &QOpenGLDebugLogger::messageLogged, this,
                [](const QOpenGLDebugMessage &msg){
                    qDebug() << "GL Debug:" << msg;
                });
        m_logger->startLogging();
    }

    // compile & link shaders
    initializeShaders();

    glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // create test cube object and initialize its GL buffers (context is current)
    m_testCube = new Cube(QVector3D(0.0f, 10.0f, 0.0f), 20.0f);
    m_testCube->init(); // safe: current context
}

void DrawingArea::initializeShaders()
{
    // Compile
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qCritical() << "Vertex shader compile error:" << m_program.log();
        return;
    }
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qCritical() << "Fragment shader compile error:" << m_program.log();
        return;
    }

    // Bind attribute location for "position" to 0 so VAO in Cube can rely on 0
    m_program.bindAttributeLocation("position", 0);

    if (!m_program.link()) {
        qCritical() << "Shader link error:" << m_program.log();
        return;
    }

    // Cache locations
    m_posAttr = m_program.attributeLocation("position"); // should be 0
    m_matrixUniform = m_program.uniformLocation("matrix");
    m_colorUniform = m_program.uniformLocation("color");
}

void DrawingArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projection.setToIdentity();
    if (h == 0) h = 1;
    m_projection.perspective(45.0f, float(w) / float(h), 0.1f, 2000.0f);
}

void DrawingArea::paintGL()
{
    if (!m_program.isLinked()) {
        // shaders not ready
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }

    // camera/view matrix
    m_view.setToIdentity();
    m_view.translate(0.0f, 0.0f, m_zoomDistance);
    m_view.rotate(m_rotationX, 1.0f, 0.0f, 0.0f);
    m_view.rotate(m_rotationY, 0.0f, 1.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // draw grid, axes and cube
    drawGrid();
    drawAxes();
    drawCube();
}

void DrawingArea::drawGrid()
{
    // grid parameters
    QVector<QVector3D> gridData;
    const int GRID_COUNT = 40;          // number of lines each side
    const float GRID_SPACING = 10.0f;
    const float HALF_EXTENT = (GRID_COUNT / 2.0f) * GRID_SPACING;
    const float Y_OFFSET = 0.001f;      // avoid z-fight

    for (int i = -GRID_COUNT/2; i <= GRID_COUNT/2; ++i) {
        float x = i * GRID_SPACING;
        gridData.append(QVector3D(x, Y_OFFSET, -HALF_EXTENT));
        gridData.append(QVector3D(x, Y_OFFSET, HALF_EXTENT));
    }
    for (int i = -GRID_COUNT/2; i <= GRID_COUNT/2; ++i) {
        float z = i * GRID_SPACING;
        gridData.append(QVector3D(-HALF_EXTENT, Y_OFFSET, z));
        gridData.append(QVector3D(HALF_EXTENT, Y_OFFSET, z));
    }

    // create VBO on the fly (fast enough for small grid)
    QOpenGLBuffer gridVbo(QOpenGLBuffer::VertexBuffer);
    gridVbo.create();
    gridVbo.bind();
    gridVbo.allocate(gridData.constData(), gridData.size() * sizeof(QVector3D));

    m_program.bind();
    QMatrix4x4 combined = m_projection * m_view;
    m_program.setUniformValue(m_matrixUniform, combined);

    m_program.enableAttributeArray(m_posAttr);
    m_program.setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 0);

    glLineWidth(1.0f);
    // grid color
    m_program.setUniformValue(m_colorUniform, QVector4D(0.32f, 0.32f, 0.36f, 1.0f));
    glDrawArrays(GL_LINES, 0, gridData.size());

    m_program.disableAttributeArray(m_posAttr);
    gridVbo.release();
    gridVbo.destroy();
    m_program.release();
}

void DrawingArea::drawAxes()
{
    QVector<QVector3D> axesData = {
        {0.0f, 0.0f, 0.0f}, {60.0f, 0.0f, 0.0f},   // X
        {0.0f, 0.0f, 0.0f}, {0.0f, 60.0f, 0.0f},   // Y
        {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 60.0f}    // Z
    };

    QOpenGLBuffer axesVbo(QOpenGLBuffer::VertexBuffer);
    axesVbo.create();
    axesVbo.bind();
    axesVbo.allocate(axesData.constData(), axesData.size() * sizeof(QVector3D));

    m_program.bind();
    QMatrix4x4 combined = m_projection * m_view;
    m_program.setUniformValue(m_matrixUniform, combined);

    m_program.enableAttributeArray(m_posAttr);
    m_program.setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 0);

    glLineWidth(3.0f);
    // X red
    m_program.setUniformValue(m_colorUniform, QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 0, 2);
    // Y green
    m_program.setUniformValue(m_colorUniform, QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 2, 2);
    // Z blue
    m_program.setUniformValue(m_colorUniform, QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINES, 4, 2);

    m_program.disableAttributeArray(m_posAttr);
    axesVbo.release();
    axesVbo.destroy();
    m_program.release();
}

void DrawingArea::ensureCubeInitialized()
{
    if (!m_testCube) {
        m_testCube = new Cube(QVector3D(0.0f, 10.0f, 0.0f), 20.0f);
        m_testCube->init();
    }
}

void DrawingArea::drawCube()
{
    ensureCubeInitialized();

    if (!m_testCube) return;

    m_program.bind();
    // combined P*V
    QMatrix4x4 combinedPV = m_projection * m_view;

    // model matrix for cube
    QMatrix4x4 model;
    model.translate(m_testCube->getPosition());
    QMatrix4x4 finalMatrix = combinedPV * model;

    m_program.setUniformValue(m_matrixUniform, finalMatrix);
    m_program.setUniformValue(m_colorUniform, QVector4D(1.0f, 0.85f, 0.0f, 1.0f)); // yellow

    // Cube::draw assumes program is bound and VAO ready
    m_testCube->draw(&m_program);

    m_program.release();
}

// ---------- Input ----------

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();
    QWidget::mousePressEvent(event);
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    const float dx = float(p.x() - m_lastMousePos.x());
    const float dy = float(p.y() - m_lastMousePos.y());

    if (event->buttons() & Qt::RightButton) {
        m_rotationX += dy * 0.5f;
        m_rotationY += dx * 0.5f;
        update();
    } else if (event->buttons() & Qt::MiddleButton) {
        // pan the view matrix directly (simple)
        const float PAN = 0.2f;
        m_view.translate(dx * PAN, -dy * PAN, 0.0f);
        update();
    }

    m_lastMousePos = p;
    QWidget::mouseMoveEvent(event);
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}

void DrawingArea::wheelEvent(QWheelEvent *event)
{
    qreal numSteps = event->angleDelta().y() / 120.0;
    const float ZOOM_INC = 15.0f;
    m_zoomDistance += numSteps * ZOOM_INC;

    if (m_zoomDistance > -50.0f) m_zoomDistance = -50.0f;
    if (m_zoomDistance < -2000.0f) m_zoomDistance = -2000.0f;

    update();
    event->accept();
}
