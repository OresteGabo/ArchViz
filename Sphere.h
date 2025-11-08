//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#ifndef ARCHIVIZ_SPHERE_H
#define ARCHIVIZ_SPHERE_H


#pragma once
#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere(float radius = 1.0f, int slices = 32, int stacks = 16);
    void generateGeometry() override;
private:
    float m_radius;
    int m_slices, m_stacks;
};



#endif //ARCHIVIZ_SPHERE_H