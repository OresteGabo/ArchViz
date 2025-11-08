#ifndef ARCHIVIZ_CUBE_H
#define ARCHIVIZ_CUBE_H

#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector>
#include <QOpenGLFunctions>
#include "Shape.h"

class QOpenGLShaderProgram;

class Cube : public Shape
{
public:
    Cube(const QVector3D &position = QVector3D(0,0,0), float size = 5.0f);
    ~Cube() override; // Add override

    void generateGeometry() override;

    void setPosition(const QVector3D &p) { m_position = p; }
    QVector3D getPosition() const { return m_position; }

private:

    float m_size;


};

#endif // ARCHIVIZ_CUBE_H
