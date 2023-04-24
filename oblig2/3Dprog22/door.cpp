#include "door.h"

Door::Door()
{
    //     v  x    y    z     r g b     mVertices
    // Door
    Vertex v1{-8.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    Vertex v2{-7.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    Vertex v3{-7.0f, 4.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    Vertex v4{-8.0f, 4.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    mVertices.push_back(v1); // Bottom left
    mVertices.push_back(v2); // Bottom right
    mVertices.push_back(v3); // Top right
    mVertices.push_back(v1); // Bottom left
    mVertices.push_back(v4); // Top left
    mVertices.push_back(v3); // Top right

    mMatrix.setToIdentity();
}

Door::~Door()
{

}

void Door::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER,
                 mVertices.size() * sizeof(Vertex),
                 mVertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6* sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Door::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

float Door::getRadius()
{
    return 0.0f;
}

QVector3D Door::getPosition()
{
    return position;
}
