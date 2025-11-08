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
    // Use m_geometryVertices (from Shape) for geometry storage
    m_geometryVertices = {
        {-hs, -hs, -hs}, { hs, -hs, -hs}, { hs,  hs, -hs}, {-hs,  hs, -hs}, // back face
        {-hs, -hs,  hs}, { hs, -hs,  hs}, { hs,  hs,  hs}, {-hs,  hs,  hs}  // front face
    };

    // 12 triangles (36 indices) - use m_indices (from Shape)
    m_indices = {
        0,1,2,  2,3,0,   // back
        4,5,6,  6,7,4,   // front
        0,4,7,  7,3,0,   // left
        1,5,6,  6,2,1,   // right
        3,2,6,  6,7,3,   // top
        0,1,5,  5,4,0    // bottom
    };
}

