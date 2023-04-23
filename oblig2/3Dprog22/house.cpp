#include "house.h"

House::House()
{

    //     v  x    y    z     r g b     mVertices

    // Back wall
    Vertex v0{-10.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v0);
    Vertex v1{-5.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v1);
    Vertex v2{-5.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v2);

    Vertex v3{-10.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v3);
    Vertex v4{-10.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v4);
    Vertex v5{-5.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v5);

    // Left wall
    Vertex v6{-10.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v6);
    Vertex v7{-10.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v7);
    Vertex v8{-10.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v8);

    Vertex v9{-10.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v9);
    Vertex v10{-10.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v10);
    Vertex v11{-10.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v11);

    // Right wall
    Vertex v12{-5.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v12);   // Bottom-left
    Vertex v13{-5.0f, 0.0f, -5.0f,  0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v13);    // Top-left
    Vertex v14{-5.0f, 5.0f, -5.0f,  0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v14);    // Top-right

    Vertex v15{-5.0f, 0.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v15);   // Bottom-left
    Vertex v16{-5.0f, 5.0f, -10.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v16);   // Bottom-right
    Vertex v17{-5.0f, 5.0f, -5.0f,  0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v17);    // Top-right

    // Front wall-left
    Vertex v18{-10.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v18);
    Vertex v19{-8.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v19);
    Vertex v20{-8.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v20);

    Vertex v21{-10.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v21);
    Vertex v22{-10.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v22);
    Vertex v23{-8.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v23);

    // Front wall-right
    Vertex v24{-7.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v24);
    Vertex v25{-5.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v25);
    Vertex v26{-5.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v26);

    Vertex v27{-7.0f, 0.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v27);
    Vertex v28{-7.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v28);
    Vertex v29{-5.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v29);

    // Front wall-middle (above door)
    Vertex v30{-8.0f, 4.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v30);
    Vertex v31{-7.0f, 4.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v31);
    Vertex v32{-7.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v32);

    Vertex v33{-8.0f, 4.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v33);
    Vertex v34{-8.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v34);
    Vertex v35{-7.0f, 5.0f, -5.0f, 0, 1, 0, 0.0f, 0.0f};   mVertices.push_back(v35);

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
