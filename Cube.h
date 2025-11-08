#ifndef ARCHIVIZ_CUBE_H
#define ARCHIVIZ_CUBE_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

class Cube : protected QOpenGLFunctions
{
public:
    Cube(const QVector3D &position = QVector3D(0,0,0), float size = 5.0f);
    ~Cube();

    void init();                 // create VAO/VBO/EBO (call with current context)
    void draw(QOpenGLShaderProgram *program); // draw using currently bound shader program

    void setPosition(const QVector3D &p) { m_position = p; }
    QVector3D getPosition() const { return m_position; }

private:
    void generateGeometry();

private:
    QVector3D m_position;
    float m_size;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo { QOpenGLBuffer::VertexBuffer };
    QOpenGLBuffer m_ebo { QOpenGLBuffer::IndexBuffer };

    QVector<float> m_vertices;      // interleaved x,y,z
    QVector<GLuint> m_indices;
};

#endif // ARCHIVIZ_CUBE_H
