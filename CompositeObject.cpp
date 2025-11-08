//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "CompositeObject.h"

void CompositeObject::addShape(Shape* shape, const QMatrix4x4 &transform) {
    //m_shapes.append({shape, transform});

    Entry entry = {shape, transform};
    m_shapes.append(entry);
}

// CompositeObject.cpp (Modified draw function)
// Assuming CompositeObject::draw now requires the shader program and the uniform location

void CompositeObject::draw(QOpenGLShaderProgram *program, QMatrix4x4 baseMatrix)
{
    // The CompositeObject must bind the program and set the matrix uniforms
    program->bind(); // Assuming the program uniform locations are visible here

    // We need the uniform location from the DrawingArea, let's assume it's set
    // in the calling scope, or we pass it here. For simplicity, let's assume
    // we use the same matrix uniform name as DrawingArea.

    int matrixUniform = program->uniformLocation("matrix");
    int colorUniform = program->uniformLocation("color");

    for (const auto &entry : m_shapes) {
        if (!entry.shape) continue;

        // 1. Calculate the final model matrix for this shape
        QMatrix4x4 finalMatrix = baseMatrix * entry.transform;

        // 2. Set the matrix uniform
        program->setUniformValue(matrixUniform, finalMatrix);

        // 3. Set the color uniform (using the shape's defined color)
        program->setUniformValue(colorUniform, entry.shape->getColor());

        // 4. Draw the geometry (one argument call)
        entry.shape->drawGeometry(); // FIX: Call the 0-argument draw function
    }

    program->release();
}
