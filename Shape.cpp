//
// Created by muhirwa gabo Oreste on 08/11/2025.
//

#include "Shape.h"

Shape::~Shape() {
    initializeOpenGLFunctions();
    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();
}

void Shape::init() {
    initializeOpenGLFunctions();

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.constData(), m_vertices.size() * sizeof(QVector3D));

    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    m_vao.release();
    m_vbo.release();
    m_ebo.release();
}

void Shape::draw(QOpenGLShaderProgram *program, const QMatrix4x4 &matrix) {
    program->setUniformValue("matrix", matrix);
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.release();
}
