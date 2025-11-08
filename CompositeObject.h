//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#ifndef ARCHIVIZ_COMPOSITEOBJECT_H
#define ARCHIVIZ_COMPOSITEOBJECT_H


// CompositeObject.h
#pragma once
#include "Shape.h"
#include <QList>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
class CompositeObject {
public:
    void addShape(Shape* shape, const QMatrix4x4 &transform);
    void draw(QOpenGLShaderProgram *program, QMatrix4x4 baseMatrix);
private:
    struct Entry { Shape* shape; QMatrix4x4 transform; };
    QList<Entry> m_shapes;
};



#endif //ARCHIVIZ_COMPOSITEOBJECT_H