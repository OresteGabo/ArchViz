//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "Pyramid.h"

Pyramid::Pyramid(float base, float height) : m_base(base), m_height(height) {
    generateGeometry();
    init();
}

// Pyramid.cpp (Modified generateGeometry() function)

void Pyramid::generateGeometry() {
    float b = m_base / 2.0f;

    // FIX: Use m_geometryVertices for QVector3D data
    m_geometryVertices = {
        QVector3D(-b, 0, -b),
        QVector3D(b, 0, -b),
        QVector3D(b, 0, b),
        QVector3D(-b, 0, b),
        QVector3D(0, m_height, 0)
    };

    // m_indices (QVector<GLuint>) is correctly assigned, no change needed.
    m_indices = {
        0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4, // sides
        0, 1, 2, 0, 2, 3                    // base
    };
}
