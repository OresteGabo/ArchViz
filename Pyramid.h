//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#ifndef ARCHIVIZ_PYRAMID_H
#define ARCHIVIZ_PYRAMID_H

#pragma once
#include "Shape.h"

class Pyramid : public Shape {
public:
    Pyramid(float base = 1.0f, float height = 1.0f);
    void generateGeometry();
private:
    float m_base, m_height;
};



#endif //ARCHIVIZ_PYRAMID_H