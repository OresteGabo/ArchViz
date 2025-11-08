#include "Cube.h"
#include <QOpenGLShaderProgram>
#include <QDebug>

Cube::Cube(const QVector3D &position, float size)
    : Shape(position), m_size(size) // Call Shape constructor
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
    float hs = m_size * 0.5f;

    // Clear all containers
    m_geometryVertices.clear();
    m_geometryNormals.clear();
    m_indices.clear();

    // Vertices and Normals must be generated together in the same order.
    // Index count will track where the next vertex is added.
    GLuint index_count = 0;

    // Helper macro for adding a quad (two triangles)
    auto add_face = [&](QVector3D p0, QVector3D p1, QVector3D p2, QVector3D p3, QVector3D normal) {
        // Vertices (4)
        m_geometryVertices.append(p0); m_geometryNormals.append(normal);
        m_geometryVertices.append(p1); m_geometryNormals.append(normal);
        m_geometryVertices.append(p2); m_geometryNormals.append(normal);
        m_geometryVertices.append(p3); m_geometryNormals.append(normal);

        // Indices (6)
        m_indices.append(index_count + 0); m_indices.append(index_count + 1); m_indices.append(index_count + 2);
        m_indices.append(index_count + 2); m_indices.append(index_count + 3); m_indices.append(index_count + 0);

        index_count += 4;
    };

    // Front Face (+Z)
    add_face(
        QVector3D(-hs, -hs, hs), QVector3D( hs, -hs, hs), QVector3D( hs,  hs, hs), QVector3D(-hs,  hs, hs),
        QVector3D(0, 0, 1)
    );

    // Back Face (-Z)
    add_face(
        QVector3D( hs, -hs, -hs), QVector3D(-hs, -hs, -hs), QVector3D(-hs,  hs, -hs), QVector3D( hs,  hs, -hs),
        QVector3D(0, 0, -1)
    );

    // Right Face (+X)
    add_face(
        QVector3D( hs, -hs,  hs), QVector3D( hs, -hs, -hs), QVector3D( hs,  hs, -hs), QVector3D( hs,  hs,  hs),
        QVector3D(1, 0, 0)
    );

    // Left Face (-X)
    add_face(
        QVector3D(-hs, -hs, -hs), QVector3D(-hs, -hs,  hs), QVector3D(-hs,  hs,  hs), QVector3D(-hs,  hs, -hs),
        QVector3D(-1, 0, 0)
    );

    // Top Face (+Y)
    add_face(
        QVector3D(-hs,  hs,  hs), QVector3D( hs,  hs,  hs), QVector3D( hs,  hs, -hs), QVector3D(-hs,  hs, -hs),
        QVector3D(0, 1, 0)
    );

    // Bottom Face (-Y)
    add_face(
        QVector3D(-hs, -hs, -hs), QVector3D( hs, -hs, -hs), QVector3D( hs, -hs,  hs), QVector3D(-hs, -hs,  hs),
        QVector3D(0, -1, 0)
    );

    // Total vertices: 24 (index_count = 24)
    // Total indices: 36 (6 faces * 6 indices/face)
}

