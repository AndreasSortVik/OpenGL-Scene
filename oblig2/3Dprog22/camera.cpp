#include "camera.h"

Camera::Camera()
{
//    mPmatrix.setToIdentity();
//    mVmatrix.setToIdentity();
}

void Camera::init()
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();

}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{
    mVmatrix.lookAt(eye, at, up);
}

void Camera::update(GLint pMatrixUniform, GLint vMatrixUniform)
{
    mPmatrixUniform = pMatrixUniform;
    mVmatrixUniform = vMatrixUniform;

    initializeOpenGLFunctions();
    glUniformMatrix4fv(mPmatrixUniform, 1, GL_FALSE, mPmatrix.constData());
    glUniformMatrix4fv(mVmatrixUniform, 1, GL_FALSE, mVmatrix.constData());
}
