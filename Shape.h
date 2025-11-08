//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#ifndef ARCHIVIZ_SHAPE_H
#define ARCHIVIZ_SHAPE_H
#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>

class Shape : protected QOpenGLFunctions {
public:
    Shape() = default;
    virtual ~Shape();

    virtual void init();
    virtual void draw(QOpenGLShaderProgram *program, const QMatrix4x4 &matrix);

protected:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
    QVector<QVector3D> m_vertices;
    QVector<unsigned int> m_indices;
};

#endif //ARCHIVIZ_SHAPE_H