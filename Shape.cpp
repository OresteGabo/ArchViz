//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "Shape.h"

#include <QOpenGLShaderProgram>
#include <QDebug>

Shape::Shape(const QVector3D &position)
    : m_position(position)
{
    // NO GL CALLS HERE
}

Shape::~Shape()
{
    // Destroy GL resources (requires context to be current)
    if (m_vao.isCreated()) m_vao.destroy();
    if (m_vbo.isCreated()) m_vbo.destroy();
    if (m_ebo.isCreated()) m_ebo.destroy();
}

void Shape::init()
{
    initializeOpenGLFunctions();

    if (m_geometryVertices.isEmpty() || m_indices.isEmpty() || m_geometryNormals.isEmpty()) {
        qWarning() << "Shape::init() - Missing geometry or normals!";
        return;
    }

    // 1. Interleave Position (3 floats) and Normal (3 floats) data
    m_vertices.clear();
    for (int i = 0; i < m_geometryVertices.size(); ++i) {
        // Position (Location 0)
        m_vertices.append(m_geometryVertices[i].x());
        m_vertices.append(m_geometryVertices[i].y());
        m_vertices.append(m_geometryVertices[i].z());

        // Normal (Location 1)
        m_vertices.append(m_geometryNormals[i].x());
        m_vertices.append(m_geometryNormals[i].y());
        m_vertices.append(m_geometryNormals[i].z());
    }

    // Create and bind VAO
    m_vao.create();
    m_vao.bind();

    // VBO (allocating interleaved data)
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));

    // EBO (Index data)
    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(m_indices.constData(), m_indices.size() * sizeof(GLuint));

    // Calculate stride and offset
    const int POSITION_COUNT = 3;
    const int NORMAL_COUNT = 3;
    const int STRIDE = (POSITION_COUNT + NORMAL_COUNT) * sizeof(float); // 6 * sizeof(float)

    // Attribute 0 = Position
    glEnableVertexAttribArray(0);
    // Location 0, 3 components, type FLOAT, Stride, Offset = 0
    glVertexAttribPointer(0, POSITION_COUNT, GL_FLOAT, GL_FALSE, STRIDE, reinterpret_cast<void*>(0));

    // Attribute 1 = Normal
    glEnableVertexAttribArray(1);
    // Location 1, 3 components, type FLOAT, Stride, Offset = 3*sizeof(float)
    glVertexAttribPointer(1, NORMAL_COUNT, GL_FLOAT, GL_FALSE, STRIDE, reinterpret_cast<void*>(POSITION_COUNT * sizeof(float)));

    // unbind
    m_vao.release();
    m_vbo.release();
    m_ebo.release();
}

void Shape::drawGeometry()
{
    if (!m_vao.isCreated()) return;

    m_vao.bind();
    // draw using EBO
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.release();
}
