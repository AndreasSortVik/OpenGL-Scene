#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>

#include "camera.h"
#include "controller.h"
#include "visualobject.h"
#include "texture.h"

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    bool mRotate{false};     //Check if triangle should rotate

private slots:
    void render();          //the actual render - function

private:
//    Vertex m_v;

    //unsigned int texture;

    std::vector<VisualObject*> mObjects;
    std::vector<VisualObject*> trophies;
    VisualObject* mia;      //member interactive object
    VisualObject* plane;
    VisualObject* house;
    VisualObject* door;
    class Light* light{nullptr};
    VisualObject* curve;
    VisualObject* line;
    VisualObject* npc;
    VisualObject* heightMap;
    Camera mCamera;
    Controller controller;

    Texture* dogTexture;

    void init();            //initialize things we need before rendering

    bool npc_Curve, npc_swap;
    float npc_x, npc_h;

    QVector3D cameraEye;
    QVector3D cameraAt;
    QVector3D cameraUp;

    bool keyPressed = false;
    bool cameraSwitched = false;
    bool spacePressed = false;

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    //Shader *mShaderProgram{nullptr};    //holds pointer the GLSL shader program
    Shader *mTexShaderProgram{nullptr};
    Shader *mShaderProgram[4];

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QMatrix4x4 *mMVPmatrix{nullptr};         //The matrix with the transform for the object we draw
    QMatrix4x4 *mPmatrix{nullptr};
    QMatrix4x4 *mVmatrix{nullptr};

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

    void setupPlainShader(int shaderIndex);
    GLint  mMatrixUniform0;              //OpenGL reference to the Uniform in the shader program
    GLint  mPmatrixUniform0;
    GLint  mVmatrixUniform0;

    void setupTextureShader(int shaderIndex);
    GLint  mMatrixUniform1;
    GLint  mPmatrixUniform1;
    GLint  mVmatrixUniform1;
    GLint  mTextureUniform1;

    void setupPhongShader(int shaderIndex);
    GLint mMatrixUniform2;
    GLint mPmatrixUniform2;
    GLint mVmatrixUniform2;

    GLint mLightColorUniform;
    GLint mObjectColorUniform;
    GLint mAmbientLightStrengthUniform;
    GLint mLightPositionUniform;
    GLint mCameraPositionUniform;
    GLint mSpecularStrengthUniform;
    GLint mSpecularExponentUniform;
    GLint mLightStrengthUniform;
//    GLint mTextureUniform2;

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;              //the only one we use now
    void keyReleaseEvent(QKeyEvent *event) override;            //not the only one we use now
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
