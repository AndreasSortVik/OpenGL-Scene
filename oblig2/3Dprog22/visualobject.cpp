#include "visualobject.h"


VisualObject::VisualObject()
{

}
VisualObject::~VisualObject()
{
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

void VisualObject::setRenderStyle(int input)
{
    renderValue = input;
}

void VisualObject::rotate(GLfloat degrees)
{
    GLfloat rad = degrees * (M_PI/180.0f);

    QMatrix4x4 temp =
    {
        std::cos(rad), 0.f, -std::sin(rad), 0.f,
        0.f,       1.f,      0.f,       0.f,
        std::sin(rad), 0.f,  std::cos(rad), 0.f,
        0.f,       0.f,      0.f,       1.f
    };

    mMatrix = mMatrix*temp;
}

void VisualObject::setPosition3D(QVector3D inPos)
{
    auto V4D = mMatrix.column(3);
    V4D.setX(inPos.x());
    V4D.setY(inPos.y());
    V4D.setZ(inPos.z());
    mMatrix.setColumn(3, V4D);
}

QVector3D VisualObject::getVectorNormal(Vertex v0, Vertex v1, Vertex v2)
{
    QVector3D p0 = {v0.m_xyz[0], v0.m_xyz[1], v0.m_xyz[2]};
    QVector3D p1 = {v1.m_xyz[0], v1.m_xyz[1], v1.m_xyz[2]};
    QVector3D p2 = {v2.m_xyz[0], v2.m_xyz[1], v2.m_xyz[2]};

    QVector3D p01 = p1 - p0;
    QVector3D p02 = p2 - p0;
    QVector3D normal = QVector3D::crossProduct(p01, p02).normalized();

    return normal;
}
