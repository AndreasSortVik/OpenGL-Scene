#ifndef CAMERA_H
#define CAMERA_H

#include <qvectornd.h>
#include <QMatrix4x4>
#include <QOpenGLFunctions_4_1_Core>

class Camera : public QOpenGLFunctions_4_1_Core
{
private:
   GLint  mPmatrixUniform;        //OpenGL reference to the Uniform in the shader program
   GLint  mVmatrixUniform;        //OpenGL reference to the Uniform in the shader program
   QVector3D mEye;
public:
   Camera();
   ~Camera() { }
   void init();
   void perspective(int degrees, double aspect, double nearplane, double farplane);
   void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);
   void update(GLint pMatrixUniform, GLint vMatrixUniform);

   QMatrix4x4 mPmatrix{};         // Projection matrix
   QMatrix4x4 mVmatrix{};         // View matrix
};

#endif // CAMERA_H
