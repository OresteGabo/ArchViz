//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "CompositeObject.h"

void CompositeObject::addShape(Shape* shape, const QMatrix4x4 &transform) {
    m_shapes.append({shape, transform});
}

void CompositeObject::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &baseMatrix) {
    for (const auto &entry : m_shapes) {
        entry.shape->draw(program, baseMatrix * entry.transform);
    }
}
