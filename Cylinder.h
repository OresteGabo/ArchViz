//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#ifndef ARCHIVIZ_CYLINDER_H
#define ARCHIVIZ_CYLINDER_H

#pragma once
#include "Shape.h"

class Cylinder : public Shape {
public:
    Cylinder(float radius = 1.0f, float height = 2.0f, int segments = 32);
    void generateGeometry();
private:
    float m_radius, m_height;
    int m_segments;
};



#endif //ARCHIVIZ_CYLINDER_H