#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H


#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>
#include <QMatrix4x4>
#include <vector>
#include <QOpenGLTexture>
#include "stb_image.h"
#include "texture.h"
#include "vertex.h"
#include "logger.h"


class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
    VisualObject();
    ~VisualObject();

    virtual void init(GLint matrixUniform)=0;
    virtual void draw()=0;
    // move for interaktive objekter
    virtual void move(float x, float y, float z) {  }
    // move for simulerende objekter
    virtual void move(float dt) {}
    virtual float getRadius() {}
    virtual QVector3D getPosition() {}
    virtual float getHeight(QVector2D p) {}
    void setRenderStyle(int input);
    bool mRotate;

    void rotate(GLfloat degrees);
    void setPosition3D(QVector3D inPos);
    QVector3D getVectorNormal(Vertex p0, Vertex p1, Vertex p2);

    QMatrix4x4 mMatrix;
    QMatrix4x4 mPmatrix;
    QMatrix4x4 mVmatrix;

protected:
    class Logger *mLogger{nullptr};

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLint mMatrixUniform{0};

    // Velger å lagre posisjon, rotasjon og translasjon
    // I hver sin 4x4 matrise
    QMatrix4x4 mPosition;
    QMatrix4x4 mRotation;
    QMatrix4x4 mScale;
    // Legger til rette for simulering
    QVector3D mVelocity;

    int renderValue;
 };
 #endif // VISUALOBJECT_H
