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

// Cylinder.cpp (Modified generateGeometry)
void Cylinder::generateGeometry() {
    m_geometryVertices.clear();
    m_geometryNormals.clear();
    m_indices.clear();

    float halfH = m_height / 2.0f;
    int indexOffset = 0;

    // --- 1. Top and Bottom Caps (Flat Normals) ---
    QVector3D topCenter(0.0f, halfH, 0.0f);
    QVector3D bottomCenter(0.0f, -halfH, 0.0f);

    // Top Center Vertex (Index 0)
    m_geometryVertices.append(topCenter); m_geometryNormals.append(QVector3D(0.0f, 1.0f, 0.0f));
    // Bottom Center Vertex (Index 1)
    m_geometryVertices.append(bottomCenter); m_geometryNormals.append(QVector3D(0.0f, -1.0f, 0.0f));
    indexOffset = 2; // Start ring vertices from index 2

    // --- 2. Side Ring Vertices (Radial Normals) ---
    for (int i = 0; i <= m_segments; ++i) {
        float theta = (2 * M_PI * i) / m_segments;
        float x = m_radius * cosf(theta);
        float z = m_radius * sinf(theta);

        QVector3D radialNormal = QVector3D(x, 0.0f, z).normalized();

        // Top Ring Vertex
        m_geometryVertices.append(QVector3D(x, halfH, z));
        m_geometryNormals.append(radialNormal); // Radial normal

        // Bottom Ring Vertex
        m_geometryVertices.append(QVector3D(x, -halfH, z));
        m_geometryNormals.append(radialNormal); // Radial normal
    }

    // --- 3. Indices ---

    // Side Faces
    for (int i = 0; i < m_segments; ++i) {
        int top1 = indexOffset + i * 2;
        int bot1 = top1 + 1;
        int top2 = indexOffset + (i + 1) * 2;
        int bot2 = top2 + 1;

        m_indices << top1 << bot1 << top2;
        m_indices << bot1 << bot2 << top2;
    }

    // Top Cap (Triangles connecting to topCenter at index 0)
    for (int i = 0; i < m_segments; ++i) {
        m_indices << 0 << (indexOffset + (i + 1) * 2) << (indexOffset + i * 2);
    }

    // Bottom Cap (Triangles connecting to bottomCenter at index 1)
    for (int i = 0; i < m_segments; ++i) {
        m_indices << 1 << (indexOffset + i * 2 + 1) << (indexOffset + (i + 1) * 2 + 1);
    }
}
