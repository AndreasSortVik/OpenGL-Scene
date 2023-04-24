#include "plane.h"

Plane::Plane()
{
    //Vertices
    //     v    x     y      z     r  g  b
    Vertex v0(-15.0f, 0.0f, 15.0f, 0, 0, 1);    // Front-left
    Vertex v1(-15.0f, 0.0f,-15.0f, 0, 0, 1);    // Back-left
    Vertex v2( 15.0f, 0.0f, 15.0f, 0, 0, 1);    // Front-right
    Vertex v3( 15.0f, 0.0f,-15.0f, 0, 0, 1);    // Back-right

    // Triangle 1
    mVertices.push_back(v0);    // Front-left
    mVertices.push_back(v1);    // Back-left
    mVertices.push_back(v2);    // Front-right
    // Triangle 2
    mVertices.push_back(v0);    // Front-left
    mVertices.push_back(v1);    // Back-left
    mVertices.push_back(v3);    // Back-right

    mMatrix.setToIdentity();
}

Plane::Plane(std::string filnavn) : VisualObject()
{
   readFile(filnavn);
   mMatrix.setToIdentity();
}

Plane::~Plane()
{

}

void Plane::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {

        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}

void Plane::readFile(std::string filnavn) {
   std::ifstream inn;
   inn.open(filnavn.c_str());


   if (inn.is_open()) {
       int n;
       Vertex vertex;
       inn >> n;
       mVertices.reserve(n);
       for (int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
       }
       inn.close();
   }
}

void Plane::init(GLint matrixUniform)
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

    glBindVertexArray(0);
}

void Plane::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Plane::place(float dx, float dy, float dz, float dr)
{
    mx = dx;
    my = dy;
    mz = dz;
    mr = dr;

//    qDebug() << "move";
    mMatrix.translate(dx, dy, dz);
}

float Plane::getRadius()
{
    return mr;
}

QVector3D Plane::getPosition()
{
    return mMatrix.column(3).toVector3D();
}
