#include "Cube.h"
#include <QOpenGLShaderProgram>
#include <QDebug>

Cube::Cube(const QVector3D &position, float size)
    : m_position(position), m_size(size)
{
    generateGeometry();
}

Cube::~Cube()
{
    // Caller should have made the GL context current before destructor runs.
    if (m_vao.isCreated()) m_vao.destroy();
    if (m_vbo.isCreated()) m_vbo.destroy();
    if (m_ebo.isCreated()) m_ebo.destroy();
}

void Cube::generateGeometry()
{
    // Build a simple cube centered at origin with size m_size
    float hs = m_size * 0.5f;
    // 8 vertices
    QVector<QVector3D> verts = {
        {-hs, -hs, -hs}, { hs, -hs, -hs}, { hs,  hs, -hs}, {-hs,  hs, -hs}, // back face
        {-hs, -hs,  hs}, { hs, -hs,  hs}, { hs,  hs,  hs}, {-hs,  hs,  hs}  // front face
    };

    m_vertices.clear();
    for (const auto &v : verts) {
        m_vertices.append(v.x());
        m_vertices.append(v.y());
        m_vertices.append(v.z());
    }

    // 12 triangles (36 indices)
    m_indices = {
        0,1,2,  2,3,0,   // back
        4,5,6,  6,7,4,   // front
        0,4,7,  7,3,0,   // left
        1,5,6,  6,2,1,   // right
        3,2,6,  6,7,3,   // top
        0,1,5,  5,4,0    // bottom
    };
}

void Cube::init()
{
    initializeOpenGLFunctions();

    if (m_vertices.isEmpty() || m_indices.isEmpty()) {
        qWarning() << "Cube::init() - no geometry!";
        return;
    }

    // Create and bind VAO
    m_vao.create();
    m_vao.bind();

    // VBO
    if (m_vbo.isCreated()) m_vbo.destroy();
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));

    // EBO
    if (m_ebo.isCreated()) m_ebo.destroy();
    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(m_indices.constData(), m_indices.size() * sizeof(GLuint));

    // attribute 0 = position (we bound "position" to 0 in the shader)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    // unbind
    m_vao.release();
    m_vbo.release();
    m_ebo.release();
}

void Cube::draw(QOpenGLShaderProgram *program)
{
    Q_UNUSED(program);
    if (!m_vao.isCreated()) return;

    m_vao.bind();
    // draw using EBO
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.release();
}
