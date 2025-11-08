//
// Created by muhirwa gabo Oreste on 08/11/2025.
//


#ifndef ARCHIVIZ_SHAPE_H
#define ARCHIVIZ_SHAPE_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

class Shape : protected QOpenGLFunctions
{
public:
    Shape(const QVector3D &position = QVector3D(0,0,0));
    virtual ~Shape();

    // Pure virtual function: Must be implemented by derived classes
    virtual void generateGeometry() = 0;

    // Common OpenGL setup function
    void init();

    // Common drawing function
    void drawGeometry();

    // Common properties
    void setPosition(const QVector3D &p) { m_position = p; }
    QVector3D getPosition() const { return m_position; }
    void setColor(const QVector4D &c) { m_color = c; }
    QVector4D getColor() const { return m_color; }

protected:
    // Common data members
    QVector3D m_position;
    QVector4D m_color = QVector4D(1.0f, 1.0f, 1.0f, 1.0f); // default white

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo { QOpenGLBuffer::VertexBuffer };
    QOpenGLBuffer m_ebo { QOpenGLBuffer::IndexBuffer };

    // FIX: Using QVector3D directly is safer for geometry generation
    QVector<QVector3D> m_geometryVertices; // Geometry data container (temporary)
    QVector<float> m_vertices;             // Final interleaved buffer data
    QVector<GLuint> m_indices;             // Index data
    QVector<QVector3D> m_geometryNormals;  // NEW: Store normals here
};

#endif // ARCHIVIZ_SHAPE_H
