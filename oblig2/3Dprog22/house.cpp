#include "house.h"

House::House()
{

    // Vertices
    //     v    x     y      z      r  g  b  u     v
    Vertex v0{-10.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   // Back-bottom-left Vertex
    Vertex v1{ -5.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   // Back-bottom-right Vertex
    Vertex v2{ -5.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   // Back-top-right Vertex
    Vertex v3{-10.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   // Back-top-left Vertex
    Vertex v4{-10.0f, 0.0f,  -5.0f, 0, 1, 0, 0.0f, 0.0f};   // Front-bottom-left Vertex
    Vertex v5{-10.0f, 5.0f,  -5.0f, 0, 1, 0, 0.0f, 0.0f};   // Front-top-left Vertex
    Vertex v6{-5.0f, 0.0f,  -5.0f, 0, 1, 0, 0.0f, 0.0f};   // Front-bottom-right Vertex
    Vertex v7{-5.0f, 5.0f,  -5.0f, 0, 1, 0, 0.0f, 0.0f};   // Front-top-right Vertex
    //OBS! Required Front Wall Vertices
    //     v    x     y      z     r  g  b  u     v
    Vertex v8{-8.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};    // Middle-bottom-left Vertex
    Vertex v9{-8.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};    // Middle-top-left Vertex
    Vertex v10{-7.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};    // Middle-bottom-right Vertex
    Vertex v11{-7.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};    // Middle-top-right Vertex
    Vertex v12{-8.0f, 4.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};    // Middle-middle-left Vertex
    Vertex v13{-7.0f, 4.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};    // Middle-middle-right Vertex

    // Back Wall            mVertices[0-5]
    mVertices.push_back(v0);    // Bottom-left
    mVertices.push_back(v2);    // Top-right
    mVertices.push_back(v1);    // Top-left
    mVertices.push_back(v0);    // Bottom-left
    mVertices.push_back(v2);    // Top-right
    mVertices.push_back(v3);    // Bottom-right

    // Left Wall            mVertices[6-11]
    mVertices.push_back(v0);    // Bottom-left
    mVertices.push_back(v4);    // Top-left
    mVertices.push_back(v5);    // Top-right
    mVertices.push_back(v0);    // Bottom-left
    mVertices.push_back(v3);    // Bottom-right
    mVertices.push_back(v5);    // Top-right

    // Right Wall           mVertices[12-17]
    mVertices.push_back(v1);    // Bottom-left
    mVertices.push_back(v7);   // Top-right
    mVertices.push_back(v6);   // Top-left
    mVertices.push_back(v1);    // Bottom-left
    mVertices.push_back(v7);   // Top-right
    mVertices.push_back(v2);    // Bottom-right

    // Front wall-left      mVertices[18-23]
    mVertices.push_back(v4);    // Bottom-left
    mVertices.push_back(v8);   // Top-left
    mVertices.push_back(v9);   // Top-right
    mVertices.push_back(v4);    // Bottom-left
    mVertices.push_back(v5);    // Bottom-right
    mVertices.push_back(v9);   // Top-right

    // Front wall-right     mVertices[24-29]
    mVertices.push_back(v10);   // Bottom-left
    mVertices.push_back(v6);   // Top-left
    mVertices.push_back(v7);   // Top-right
    mVertices.push_back(v10);   // Bottom-left
    mVertices.push_back(v11);   // Bottom-right
    mVertices.push_back(v7);   // Top-right

    // Front wall-middle    mVertices[30-35]
    mVertices.push_back(v12);
    mVertices.push_back(v13);
    mVertices.push_back(v11);
    mVertices.push_back(v12);
    mVertices.push_back(v9);
    mVertices.push_back(v11);

    // Finds normal for each triangle (smooth shading)
    for(int i = 0; i <= mVertices.size() - 2; i += 3)
    {
        QVector3D normal;

        if (i % 6 != 0 && i != 0)
            normal = VisualObject::getVectorNormal(mVertices[i], mVertices[i + 2], mVertices[i + 1]);
        else
            normal = VisualObject::getVectorNormal(mVertices[i], mVertices[i + 1], mVertices[i + 2]);



        mVertices[i].m_normal[0] = normal.x();
        mVertices[i].m_normal[1] = normal.y();
        mVertices[i].m_normal[2] = normal.z();

        mVertices[i + 1].m_normal[0] = normal.x();
        mVertices[i + 1].m_normal[1] = normal.y();
        mVertices[i + 1].m_normal[2] = normal.z();

        mVertices[i + 2].m_normal[0] = normal.x();
        mVertices[i + 2].m_normal[1] = normal.y();
        mVertices[i + 2].m_normal[2] = normal.z();
    }


    mMatrix.setToIdentity();
}

House::~House()
{

}

void House::init(GLint matrixUniform)
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void House::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

float House::getRadius()
{
    return 0.0f;
}

QVector3D House::getPosition()
{
    return position;
}
