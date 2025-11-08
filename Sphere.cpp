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

void Sphere::generateGeometry() {
    for (int i = 0; i <= m_stacks; ++i) {
        float phi = M_PI * i / m_stacks;
        for (int j = 0; j <= m_slices; ++j) {
            float theta = 2.0f * M_PI * j / m_slices;
            float x = m_radius * sinf(phi) * cosf(theta);
            float y = m_radius * cosf(phi);
            float z = m_radius * sinf(phi) * sinf(theta);
            m_vertices.append(QVector3D(x, y, z));
        }
    }

    for (int i = 0; i < m_stacks; ++i) {
        for (int j = 0; j < m_slices; ++j) {
            int first = i * (m_slices + 1) + j;
            int second = first + m_slices + 1;

            m_indices << first << second << first + 1;
            m_indices << second << second + 1 << first + 1;
        }
    }
}
