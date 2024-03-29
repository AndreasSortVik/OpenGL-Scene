#include "heightmap.h"

#include "stb_image.h"
#include "logger.h"
#include <cmath>

HeightMap::HeightMap(const char* ImagePath)
{
    unsigned char* data = stbi_load(ImagePath, &width, &height, &bitDepth, 1);

    float res = 1;
    float x{};
    float y{};
    float z{};
    float heightMultiplier{ 0.05f }; // 0.05f
    float yPlacement{ -7.0f }; // -5.0f
    int vert{};

    for (x = -height / 2; x < (height / 2); x += res) {
        for (z = -height / 2; z < (width / 2); z += res) {

            y = (float)data[vert];
            mVertices.push_back(Vertex{x, y * heightMultiplier + yPlacement, z, 0.0f, y, 1.0f});

            y = (float)data[vert+width];
            mVertices.push_back(Vertex{ x + res, y * heightMultiplier + yPlacement, z, 0.0f, y, 1.0f});

            y = (float)data[vert+1];
            mVertices.push_back(Vertex{ x, y * heightMultiplier + yPlacement, z + res, 0.0f, y, 1.0f});
            mVertices.push_back(Vertex{ x, y * heightMultiplier + yPlacement, z + res, 0.0f, y, 1.0f});

            y = (float)data[vert+width];
            mVertices.push_back(Vertex{ x + res, y * heightMultiplier + yPlacement, z, 0.0f, y, 1.0f});

            y = (float)data[vert+1+width];
            mVertices.push_back(Vertex{ x + res, y * heightMultiplier + yPlacement, z + res, 0.0f, y, 1.0f});

            vert++;
            // Get tex elvation for (i, j) tex coordinate)
            //unsigned char* texEl = data + (j + width * i) * channels;
            // Raw height at coordinate
            //unsigned char y = texEl[0]; // Needs correct path to file, gives nullptr if file is not found

        /*	vertices.push_back(Vertex{
                (-height / 2.0f + height * i/(float)height),
                ((int)y * yScale - yShift),
                (-width / 2.0f + width * j / (float)width)
            });*/
        }
    }

    stbi_image_free(data);

}

void HeightMap::init(GLint matrixUniform)
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

void HeightMap::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

float HeightMap::getHeight(QVector2D p) // Returns barycentric coordinates when point is inside triangle
{
    mLogger = Logger::getInstance();

    //QVector2D p = {tempP.x(), tempP.z()};
    QVector2D v0;
    QVector2D v1;
    QVector2D v2;
    //QVector3D v0;
    //QVector3D v1;
    //QVector3D v2;
    QVector3D baryc3D{};

    // Loops through all triangles to find correct
    for (int i = 0; i < mVertices.size() - 2; i += 3)
    {
        // Converts position of vertex in triangle to QVector3D
        v0 = {mVertices[i].m_xyz[0], mVertices[i].m_xyz[1]};
        v1 = {mVertices[i + 1].m_xyz[0], mVertices[i + 1].m_xyz[1]};
        v2 = {mVertices[i + 2].m_xyz[0], mVertices[i + 2].m_xyz[1]};

//        v0 = {mVertices[i].m_xyz[0], mVertices[i].m_xyz[1], mVertices[i].m_xyz[2]};
//        v1 = {mVertices[i + 1].m_xyz[0], mVertices[i + 1].m_xyz[1], mVertices[i + 1].m_xyz[2]};
//        v2 = {mVertices[i + 2].m_xyz[0], mVertices[i + 2].m_xyz[1], mVertices[i + 2].m_xyz[2]};

        // Finds barycentric coordinates
        baryc3D = getBarycCoordinate(v0, v1, v2, p);

        //mLogger->logText("u:" + std::to_string(baryc3D.x()));
        //mLogger->logText("v:" + std::to_string(baryc3D.y()));
        //mLogger->logText("w:" + std::to_string(baryc3D.z()));

        if (baryc3D.x() >= 0 && baryc3D.y() >= 0 && baryc3D.z() >= 0)
        {
            //mLogger->logText("u:" + std::to_string(baryc3D.x()));
            //mLogger->logText("v:" + std::to_string(baryc3D.y()));
            //mLogger->logText("w:" + std::to_string(baryc3D.z()));
            mLogger->logText("Found at triangle number: " + std::to_string(i / 3));
            break;
        }
    }

    //mLogger->logText("u:" + std::to_string(baryc3D.x()));
    //mLogger->logText("v:" + std::to_string(baryc3D.y()));
    //mLogger->logText("w:" + std::to_string(baryc3D.z()));

    float height = v0.y() * baryc3D.x() + v1.y() * baryc3D.y() + v2.y() * baryc3D.z();
    //mLogger->logText("Height value:" + std::to_string(height));
    return height;
}

QVector3D HeightMap::getBarycCoordinate(QVector2D v0, QVector2D v1, QVector2D v2, QVector2D p)
{
    mLogger = Logger::getInstance();

    QVector2D v10 = v1 - v0;
    QVector2D v21 = v2 - v1;

    float area = QVector3D::crossProduct(QVector3D(v10, 0.0f), QVector3D(v21, 0.0f)).z();

    QVector2D v0p = v0 - p;
    QVector2D v1p = v1 - p;
    QVector2D v2p = v2 - p;

    float u = QVector3D::crossProduct(QVector3D(v0p, 0.0f), QVector3D(v1p, 0.0f)).z() / area;
    float v = QVector3D::crossProduct(QVector3D(v1p, 0.0f), QVector3D(v2p, 0.0f)).z() / area;
    float w = QVector3D::crossProduct(QVector3D(v2p, 0.0f), QVector3D(v0p, 0.0f)).z() / area;

    QVector3D baryc3D = {u, v, w};

    return baryc3D;

    /*
    // Vector between triangle vertices and vertex p
    QVector2D v0p = v0 - p;
    QVector2D v1p = v1 - p;
    QVector2D v2p = v2 - p;

//    QVector3D v0p = v0 - p;
//    QVector3D v1p = v1 - p;
//    QVector3D v2p = v2 - p;

    //mLogger->logText("v0p:" + std::to_string(v0p));
    //mLogger->logText("v1p:" + std::to_string(v1p));
    //mLogger->logText("v2p:" + std::to_string(v2p));

    // Area for each triangle with relation to vertex p


    float A = QVector3D::crossProduct(QVector3D{v0p, 0.0f}, QVector3D{v1p, 0.0f}).length();
    float B = QVector3D::crossProduct(QVector3D{v1p, 0.0f}, QVector3D{v2p, 0.0f}).length();
    float C = QVector3D::crossProduct(QVector3D{v2p, 0.0f}, QVector3D{v0p, 0.0f}).length();

    mLogger->logText("A: " + std::to_string(A));
    mLogger->logText("B: " + std::to_string(B));
    mLogger->logText("C: " + std::to_string(C));

    // Area of triangle
    //float area = QVector3D::crossProduct(v1 - v0, v2 - v0).length();
    float area = QVector3D::crossProduct(QVector3D(v1 - v0, 0.0), QVector3D(v2 - v0, 0.0)).length();

    // Barycentric coordinates
    float u = A / area;
    float v = B / area;
    float w = C / area;

    //mLogger->logText("u:" + std::to_string(u));
    //mLogger->logText("v:" + std::to_string(v));
    //mLogger->logText("w:" + std::to_string(w));

    // Vector with barycentric coordinates
    QVector3D baryc3D = {u, v, w};

    return baryc3D;
    */
}
