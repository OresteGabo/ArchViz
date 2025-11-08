//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "Cylinder.h"
#include <cmath>

Cylinder::Cylinder(float radius, float height, int segments)
    : m_radius(radius), m_height(height), m_segments(segments) {
    generateGeometry();
    init();
}

void Cylinder::generateGeometry() {
    float halfH = m_height / 2.0f;

    // Generate circle vertices
    for (int i = 0; i <= m_segments; ++i) {
        float theta = (2 * M_PI * i) / m_segments;
        float x = m_radius * cosf(theta);
        float z = m_radius * sinf(theta);
        m_vertices.append(QVector3D(x, halfH, z));   // top
        m_vertices.append(QVector3D(x, -halfH, z));  // bottom
    }

    // Side faces
    for (int i = 0; i < m_segments; ++i) {
        int top1 = i * 2;
        int bot1 = top1 + 1;
        int top2 = (i + 1) * 2;
        int bot2 = top2 + 1;

        m_indices << top1 << bot1 << top2;
        m_indices << bot1 << bot2 << top2;
    }
}
