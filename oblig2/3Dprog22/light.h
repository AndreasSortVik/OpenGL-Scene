#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

///Simple Light class that has a small mesh and some light parameters
class Light : public VisualObject
{
public:
    ///Light class with mesh and light parameters
    Light();
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

    GLfloat mAmbientStrength{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f};  //Grey

    GLfloat mLightStrength{0.7f};
    QVector3D mLightColor{0.9f, 0.9f, 0.3f};    //Yellow

    GLfloat mSpecularStrength{0.9f};
    GLint mSpecularExponent{4};

    QVector3D getPosition() override;

private:
    float mx, my, mz, mr;
};

#endif // LIGHT_H
