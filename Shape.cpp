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
    // CRITICAL: Initialize GL functions
    initializeOpenGLFunctions();

    // 1. Convert m_geometryVertices to interleaved buffer data (m_vertices)
    m_vertices.clear();
    for (const auto &v : m_geometryVertices) {
        m_vertices.append(v.x());
        m_vertices.append(v.y());
        m_vertices.append(v.z());
    }

    if (m_vertices.isEmpty() || m_indices.isEmpty()) {
        qWarning() << "Shape::init() - No geometry to initialize!";
        return;
    }

    // Create and bind VAO
    m_vao.create();
    m_vao.bind();

    // VBO (Vertex Buffer Object)
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));

    // EBO (Element Buffer Object)
    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(m_indices.constData(), m_indices.size() * sizeof(GLuint));

    // Attribute 0 = position (3 floats per vertex)
    glEnableVertexAttribArray(0);
    // The stride is 3 * sizeof(float) because we only store x, y, z
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

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
