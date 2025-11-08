//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "Sphere.h"
#include <cmath>

Sphere::Sphere(float radius, int slices, int stacks)
    : m_radius(radius), m_slices(slices), m_stacks(stacks) {
    generateGeometry();
    init();
}

// Sphere.cpp (Modified generateGeometry)
void Sphere::generateGeometry() {
    m_geometryVertices.clear();
    m_geometryNormals.clear();
    m_indices.clear();

    // ... (Stack/Slice calculation code remains the same) ...

    for (int i = 0; i <= m_stacks; ++i) {
        float phi = M_PI * i / m_stacks;
        for (int j = 0; j <= m_slices; ++j) {
            float theta = 2.0f * M_PI * j / m_slices;
            float x = m_radius * sinf(phi) * cosf(theta);
            float y = m_radius * cosf(phi);
            float z = m_radius * sinf(phi) * sinf(theta);

            QVector3D vertex = QVector3D(x, y, z);

            // Sphere Normal: Normalized position vector (since it's a smooth surface)
            m_geometryVertices.append(vertex);
            m_geometryNormals.append(vertex.normalized()); // Normal is just the normalized position
        }
    }

    // ... (Index calculation code remains the same) ...
    for (int i = 0; i < m_stacks; ++i) {
        for (int j = 0; j < m_slices; ++j) {
            int first = i * (m_slices + 1) + j;
            int second = first + m_slices + 1;

            m_indices << first << second << first + 1;
            m_indices << second << second + 1 << first + 1;
        }
    }
}
