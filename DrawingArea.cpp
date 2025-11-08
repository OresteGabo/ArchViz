#include "DrawingArea.h"
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector4D>
#include <QDebug>
#include "Shape.h"
#include "Pyramid.h"
#include "Cube.h"
#include "Sphere.h"
// Simple shaders (position + uniform color)
static const char *vertexShaderSource =
    "attribute vec3 position;\n"
    "attribute vec3 normal;\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "varying vec3 FragPos;\n"
    "varying vec3 Normal;\n"
    "void main() {\n"
    "  FragPos = vec3(modelMatrix * vec4(position, 1.0));\n"

    // FIX for GLSL 1.10: Manually extract the 3x3 matrix components for the Normal transform
    "  mat3 normalMatrix = mat3(\n"
    "    modelMatrix[0].xyz, \n"
    "    modelMatrix[1].xyz, \n"
    "    modelMatrix[2].xyz\n"
    "  );\n"
    "  Normal = normalMatrix * normal;\n"

    "  gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);\n"
    "}\n";
static const char *fragmentShaderSource =
    "uniform vec3 lightPos;\n"
    "uniform vec3 viewPos;\n"
    "uniform vec4 objectColor;\n"

    // Lighting components
    "uniform vec3 ambientColor;\n"
    "uniform vec3 diffuseColor;\n"
    "uniform vec3 specularColor;\n"
    "uniform float shininess;\n" // Specular exponent

    "varying vec3 FragPos;\n"
    "varying vec3 Normal;\n"

    "void main() {\n"
    // --- 1. Ambient component ---
    "  vec3 ambient = ambientColor * vec3(objectColor);\n"

    // --- 2. Diffuse component ---
    "  vec3 norm = normalize(Normal);\n"
    "  vec3 lightDir = normalize(lightPos - FragPos);\n"
    "  float diff = max(dot(norm, lightDir), 0.0);\n"
    "  vec3 diffuse = diffuseColor * diff * vec3(objectColor);\n"

    // --- 3. Specular component ---
    "  vec3 viewDir = normalize(viewPos - FragPos);\n"
    "  vec3 reflectDir = reflect(-lightDir, norm);\n"
    "  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);\n"
    "  vec3 specular = specularColor * spec;\n"

    // --- Final color ---
    "  vec3 result = ambient + diffuse + specular;\n"
    "  gl_FragColor = vec4(result, objectColor.a);\n"
    "}\n";

DrawingArea::DrawingArea(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

// DrawingArea.cpp

// ... (other DrawingArea functions)

void DrawingArea::paintGL()
{
    if (!m_program.isLinked()) {
        // shaders not ready
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }

    // Camera/View matrix setup
    m_view.setToIdentity();
    m_view.translate(0.0f, 0.0f, m_zoomDistance);
    m_view.rotate(m_rotationX, 1.0f, 0.0f, 0.0f);
    m_view.rotate(m_rotationY, 0.0f, 1.0f, 0.0f);

    // CRITICAL: Calculate Camera/View Position (Inverse of View Matrix translation)
    QMatrix4x4 viewInverse = m_view.inverted();
    m_viewPosition = viewInverse.column(3).toVector3D();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Draw the scene elements
    drawGrid();
    drawAxes();
    drawShapes();
}

// ... (rest of DrawingArea functions)
DrawingArea::~DrawingArea()
{
    makeCurrent();

    // Delete all shapes
    qDeleteAll(m_shapes);
    m_shapes.clear();

    if (m_logger) delete m_logger;
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

    // 3. Create and Initialize ALL shapes (Context is current)

    // 3.1. Create the Cube
    Cube *cube = new Cube(QVector3D(30.0f, 10.0f, 0.0f), 20.0f);
    cube->setColor(QVector4D(1.0f, 0.85f, 0.0f, 1.0f)); // yellow
    cube->init();
    m_shapes.append(cube);

    // 3.2. Create a Sphere
    Shape *sphere = new Sphere(15.0f, 32, 16);
    sphere->setPosition(QVector3D(-30.0f, 15.0f, 0.0f));
    sphere->setColor(QVector4D(0.0f, 0.5f, 1.0f, 1.0f)); // blue
    sphere->init();
    m_shapes.append(sphere);

    // 3.3. Create a Pyramid
    Pyramid *pyramid = new Pyramid(20.0f, 30.0f);
    pyramid->setPosition(QVector3D(0.0f, 15.0f, 30.0f));
    pyramid->setColor(QVector4D(0.8f, 0.2f, 0.8f, 1.0f)); // magenta
    pyramid->init();
    m_shapes.append(pyramid);
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

    // Bind attribute locations (MUST match gl_Position and Normal buffer order)
    m_program.bindAttributeLocation("position", 0);
    m_program.bindAttributeLocation("normal", 1); // NEW: Bind normal attribute to location 1

    if (!m_program.link()) {
        qCritical() << "Shader link error:" << m_program.log();
        return;
    }

    // Cache NEW matrix locations
    m_posAttr = m_program.attributeLocation("position");
    m_normalAttr = m_program.attributeLocation("normal");

    m_modelMatrixUniform = m_program.uniformLocation("modelMatrix");
    m_viewMatrixUniform = m_program.uniformLocation("viewMatrix");
    m_projectionMatrixUniform = m_program.uniformLocation("projectionMatrix");
    m_objectColorUniform = m_program.uniformLocation("objectColor");

    // Cache lighting uniforms
    m_lightPosUniform = m_program.uniformLocation("lightPos");
    m_viewPosUniform = m_program.uniformLocation("viewPos");

    // Set fixed lighting uniforms once (Ambient, Diffuse, Specular properties)
    m_program.bind();
    m_program.setUniformValue(m_program.uniformLocation("ambientColor"), m_ambientColor);
    m_program.setUniformValue(m_program.uniformLocation("diffuseColor"), m_diffuseColor);
    m_program.setUniformValue(m_program.uniformLocation("specularColor"), m_specularColor);
    m_program.setUniformValue(m_program.uniformLocation("shininess"), m_shininess);
    m_program.release();
}

void DrawingArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projection.setToIdentity();
    if (h == 0) h = 1;
    m_projection.perspective(45.0f, float(w) / float(h), 0.1f, 2000.0f);
}



// DrawingArea.cpp

void DrawingArea::drawGrid()
{
    // The grid is centered at the origin (0, 0, 0) in world space, so its model matrix is identity.
    QMatrix4x4 model;
    model.setToIdentity();

    m_program.bind();

    // 1. Set NEW Matrix Uniforms for the current shader
    m_program.setUniformValue(m_projectionMatrixUniform, m_projection);
    m_program.setUniformValue(m_viewMatrixUniform, m_view);
    m_program.setUniformValue(m_modelMatrixUniform, model);

    // 2. Set the Color Uniform
    // This color will be affected by ambient light, but it should be visible.
    QVector4D gridColor(0.32f, 0.32f, 0.36f, 1.0f);
    m_program.setUniformValue(m_objectColorUniform, gridColor);

    // Grid parameters
    const int gridSize = 20;
    const int numLines = gridSize * 2 + 1;
    const float lineSpacing = 1.0f;

    // Create line data
    QVector<float> gridData;
    float start = -gridSize * lineSpacing;
    float end = gridSize * lineSpacing;
    float yOffset = 0.001f; // Slightly above the origin to prevent Z-fighting with the base plane

    for (int i = 0; i < numLines; ++i) {
        float pos = start + i * lineSpacing;

        // Vertical lines (along X-axis)
        gridData.append(pos); gridData.append(yOffset); gridData.append(start);
        gridData.append(pos); gridData.append(yOffset); gridData.append(end);

        // Horizontal lines (along Z-axis)
        gridData.append(start); gridData.append(yOffset); gridData.append(pos);
        gridData.append(end); gridData.append(yOffset); gridData.append(pos);
    }

    // VBO setup
    if (!m_gridVbo.isCreated()) {
        m_gridVbo.create();
    }
    m_gridVbo.bind();
    m_gridVbo.allocate(gridData.constData(), gridData.size() * sizeof(float));

    // 3. Configure Attributes
    // CRITICAL: ONLY enable the position attribute (Location 0).
    // The normal attribute (Location 1) MUST be left disabled so the shader doesn't try to read it.

    // Enable Position Attribute (Location 0)
    m_program.enableAttributeArray(m_posAttr);
    // Attribute pointer: (Location, Size=3, Type=float, Normalized=False, Stride=0, Offset=0)
    m_program.setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 0);

    // Draw the grid lines
    glDrawArrays(GL_LINES, 0, gridData.size() / 3);

    // 4. Cleanup
    m_program.disableAttributeArray(m_posAttr);
    m_gridVbo.release();
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

// DrawingArea.cpp (Modified drawShapes)

void DrawingArea::drawShapes()
{
    m_program.bind();

    // Set PROJECTION and VIEW matrix UNIFORMS (once per frame)
    m_program.setUniformValue(m_projectionMatrixUniform, m_projection);
    m_program.setUniformValue(m_viewMatrixUniform, m_view);

    // Set Light and View positions (once per frame)
    m_program.setUniformValue(m_lightPosUniform, m_lightPosition);
    m_program.setUniformValue(m_viewPosUniform, m_viewPosition);

    for (Shape *shape : m_shapes) {
        if (!shape) continue;

        // 1. Calculate Model Matrix
        QMatrix4x4 model;
        model.translate(shape->getPosition());

        // 2. Set uniforms specific to this shape (Model matrix & Color)
        m_program.setUniformValue(m_modelMatrixUniform, model);
        m_program.setUniformValue(m_objectColorUniform, shape->getColor());

        // 3. Draw
        shape->drawGeometry();
    }
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
