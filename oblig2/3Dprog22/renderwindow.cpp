    #include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>

#include "curve.h"
#include "light.h"
#include "line.h"
#include "npc.h"
#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "interactiveobject.h"
#include "tobject.h"
#include "plane.h"
#include "house.h"
#include "door.h"
#include "heightmap.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //This is the matrix used to transform (rotate) the triangle
    //You could do without, but then you have to simplify the shader and shader setup
//    mMVPmatrix = new QMatrix4x4{};
//    mMVPmatrix->setToIdentity();    //1, 1, 1, 1 in the diagonal of the matrix
    mPmatrix = new QMatrix4x4{};
    mPmatrix->setToIdentity();
    mVmatrix = new QMatrix4x4{};
    mVmatrix->setToIdentity();

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

//    mObjects.push_back(new XYZ());
    mia = new InteractiveObject;
    mObjects.push_back(mia);

    //plane = new Plane("plane.txt");
    //mObjects.push_back(plane);

    house = new House;
    mObjects.push_back(house);

    door = new Door;
    mObjects.push_back(door);
    door->setPosition3D(QVector3D{0.0f, 0.0f, 0.0f});

    light = new Light;
    light->setPosition3D(QVector3D{0.0f, 2.5f, 0.0f});
    mObjects.push_back(light);

    curve = new Curve;
    mObjects.push_back(curve);

    line = new Line;
    mObjects.push_back(line);

    npc = new NPC;
    mObjects.push_back(npc);
    npc_Curve = false;

    heightMap = new HeightMap((char*)("../3Dprog22/heightmap.png"));
    //heightMap = new HeightMap((char*)("C:/Users/kmstr/Documents/GitHub/OpenGL-Scene/oblig2/3Dprog22/heightmap.png")); // Obs! Write your own directory to the image
    //heightMap = new HeightMap((char*)("C:/Users/wohal/source/repos/OpenGL-Scene/oblig2/3Dprog22/heightmap.png"));
    heightMap->setPosition3D(QVector3D{0.0f, 0.0f,0.0f});
    mObjects.push_back(heightMap);


    // Trophies: tObject(x, y, z, radius)
    trophies.push_back(new tObject(0.5f, 0.0f, -0.5f, 0.2));
    trophies.push_back(new tObject(4.5f, 0.0f, 2.3f, 0.2));
    trophies.push_back(new tObject(-2.5f, 0.0f, 1.0f, 0.2));
    trophies.push_back(new tObject(3.2f, 0.0f, -2.5f, 0.2));
    trophies.push_back(new tObject(-2.0f, 0.0f, 2.0f, 0.2));
    trophies.push_back(new tObject(1.5f, 0.0f, 3.5f, 0.2));
    trophies.push_back(new tObject(-3.0f, 0.0f, -2.3f, 0.2));
    trophies.push_back(new tObject(-7.0f, 0.0f, -8.0f, 0.2));

    // Add trophy objects into mObjects, and sets render style to visible
    for (int i = 0; i < trophies.size(); i++) {
        mObjects.push_back(trophies[i]);
        trophies[i]->setRenderStyle(0);
    }

    cameraEye = QVector3D{6.0f, 2.5f, 6.0f};
    //cameraEye = QVector3D{-7.0f, 2.5f, 6.0f};
    cameraAt = QVector3D{0.0f, 0.0f, 0.0f};
    //cameraAt = QVector3D{-7.0f, 0.0f, -5.0f};
    cameraUp = QVector3D{0.0f, 1.0f, 0.0f};
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //s only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram[0] = new Shader("../3Dprog22/plainshader.vert", "../3Dprog22/plainshader.frag");
    // For me (Andreas) "../3Dprog22/[filename]" does not work
    mShaderProgram[1] = new Shader("../3Dprog22/textureshader.vert", "../3Dprog22/textureshader.frag");
    //mShaderProgram[1] = new Shader("C:/Users/wohal/source/repos/OpenGL-Scene/oblig2/3Dprog22/textureshader.vert", "C:/Users/wohal/source/repos/OpenGL-Scene/oblig2/3Dprog22/textureshader.frag");
    mShaderProgram[2] = new Shader("../3Dprog22/phongshader.vert", "../3Dprog22/phongshader.frag");
    //mShaderProgram[2] = new Shader("C:/Users/wohal/source/repos/OpenGL-Scene/oblig2/3Dprog22/phongshader.vert", "C:/Users/wohal/source/repos/OpenGL-Scene/oblig2/3Dprog22/phongshader.frag");

    // Setups up different matrices for the different shaders
    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);

    // Initilizes texture
    //dogTexture = new Texture((char*)("C:/Users/wohal/source/repos/OpenGL-Scene/oblig2/3Dprog22/dog.jpg"));
    dogTexture = new Texture((char*)("../3Dprog22/dog.jpg"));
    dogTexture->LoadTexture();

    // Size of points
    glPointSize(0.1);

    for (auto it = mObjects.begin(); it != mObjects.end(); it++)
    {
        if ((*it) == door)
            (*it)->init(mMatrixUniform1);
        else if ((*it) == house)
            (*it)->init(mMatrixUniform2);
        else
            (*it)->init(mMatrixUniform0);
    }

    // AI movement parametres
    npc_x = -1.0f;
    npc_h = 0.1f;
    npc_swap = true;

    glBindVertexArray(0);
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    mCamera.init();
    mCamera.perspective(60, 4.0/3.0, 0.1, 1000.0);
    //mCamera.lookAt(QVector3D{6.0f, 2.5f, 6.0f}, QVector3D{0.0f, 0.0f, 0.0f}, QVector3D{0.0f, 1.0f, 0.0f});
    //mCamera.lookAt(cameraEye, mia->getPosition(), cameraUp);
    mCamera.lookAt(cameraEye, QVector3D(-7.5, 2.0f, -10), cameraUp);

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
        // What shader to use (plainshader)
        glUseProgram(mShaderProgram[0]->getProgram());
        //send data to shaders
        mCamera.update(mPmatrixUniform0, mVmatrixUniform0);

        // Draw
        for (auto it = mObjects.begin(); it != mObjects.end(); it++)
        {
            if ((*it) == door || (*it) == house) // Does not render ground nor door objects with plainshader
                continue;

            (*it)->draw();
        }

        // Draws all objects using phong shading
        // What shader to use (texture & phong shader)
        glUseProgram(mShaderProgram[2]->getProgram());
        //send data to shaders
        mCamera.update(mPmatrixUniform2, mVmatrixUniform2);
        checkForGLerrors();

        //Additional parameters for light shader:
        glUniform3f(mLightPositionUniform, light->getPosition().x(), light->getPosition().y(), light->getPosition().z());
        glUniform3f(mCameraPositionUniform, cameraEye.x(), cameraEye.y(), cameraEye.z());
        glUniform3f(mLightColorUniform, light->mLightColor.x(), light->mLightColor.y(), light->mLightColor.z());
        glUniform1f(mSpecularStrengthUniform, light->mSpecularStrength);
        //Texture
        //glUniform1i(mTextureUniform2, 0);

//        // Draw (is able to draw all objects)
//        for (auto it = mObjects.begin(); it != mObjects.end(); it++)
//        {
//            if ((*it) == door) // Does not render door objects with phongshader
//                continue;

//            (*it)->draw();
//        }

        // Draw
        house->draw();
        //heightMap->draw();

        // What shader to use (textureshader)
        glUseProgram(mShaderProgram[1]->getProgram());
        mCamera.update(mPmatrixUniform1, mVmatrixUniform1);
        glUniform1i(mTextureUniform1, 0);
        dogTexture->UseTexture();

        door->draw();
    }
    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader
    //mMatrixUniform0 = glGetUniformLocation( mShaderProgram[0]->getProgram(), "matrix" );

    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);

    //Movement
    if (mia)
    {
        float playerHeight = 0.0f;

        QVector2D playerPos = {mia->getPosition().x(), mia->getPosition().z()};
        if (controller.moveLeft)
        {
            playerHeight = heightMap->getHeight(playerPos);
            mLogger->logText("Player height" + std::to_string(playerHeight));

            mia->move(-0.1f, 0.0f, 0.0f); // (-0.1f, 0.0f, 0.0f)
            mia->setPosition3D(QVector3D(mia->getPosition().x(), playerHeight, mia->getPosition().z()));
        }
        if (controller.moveRight)
        {
            playerHeight = heightMap->getHeight(playerPos);
            mLogger->logText("Player height" + std::to_string(playerHeight));

            mia->move(0.1f, 0.0f, 0.0f); // (0.1f, 0.0f, 0.0f)
            mia->setPosition3D(QVector3D(mia->getPosition().x(), playerHeight, mia->getPosition().z()));
        }
        if (controller.moveUp)
        {
            playerHeight = heightMap->getHeight(playerPos);
            mLogger->logText("Player height" + std::to_string(playerHeight));

            mia->move(0.0f, 0.0f, -0.1f); // (0.0f, 0.0f, -0.1f)
            mia->setPosition3D(QVector3D(mia->getPosition().x(), playerHeight, mia->getPosition().z()));
        }
        if (controller.moveDown)
        {
            playerHeight = heightMap->getHeight(playerPos);
            mLogger->logText("Player height" + std::to_string(playerHeight));

            mia->move(0.0f, 0.0f, 0.1f); // (0.0f, 0.0f, 0.1f)
            mia->setPosition3D(QVector3D(mia->getPosition().x(), playerHeight, mia->getPosition().z()));
        }

        // Light source follows player
        light->setPosition3D(QVector3D(mia->getPosition().x(), mia->getPosition().y() + 2.5f, mia->getPosition().z()));

        // Checks for collisions
        for (int i = 0; i < trophies.size(); i++)
        {
            float distance = trophies[i]->getPosition().distanceToPoint(mia->getPosition());
            //float distance = mia->getPosition().distanceToPoint(trophies[i]->getPosition());
            //mLogger->logText("Object " + std::to_string(i) + ": " + std::to_string(distance));

            if (distance < mia->getRadius() + trophies[i]->getRadius())
            {
                //mLogger->logText("Collisions!!!");
                trophies[i]->setRenderStyle(1);
            }
        }

    }

    // Light source movement
//    if (light)
//    {
//        long secElapsed = mTimeStart.nsecsElapsed();

//        float light_x = 5.5f * sin(secElapsed);
//        float light_y = 2.5f;
//        float light_z = 5.5f * cos(secElapsed);

//        light->setPosition3D(QVector3D(light_x, light_y, light_z));
//    }

    //AI movement
    if (npc) {
        float npc_y = 0.0f;
        float npc_z;
        long secElapsed = mTimeStart.nsecsElapsed() / 1000000;
        if (npc_swap)
        {
            npc_x += npc_h / secElapsed;
            if (npc_x >= 1.0f)
                npc_swap = false;
        }
        else
        {
            npc_x -= npc_h / secElapsed;
            if (npc_x <= -1.0f)
                npc_swap = true;
        }

        if (npc_Curve)
            npc_z = pow(npc_x, 2);
        else
            npc_z = -npc_x;

        npc->move(npc_x, npc_y, npc_z);
        //mLogger->logText(std::to_string(secElapsed));
        //mLogger->logText(std::to_string(npc_x) + ", " + std::to_string(npc_y) + ", " + std::to_string(npc_z));
    }
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mShaderProgram[shaderIndex]->use();
    mPmatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pmatrix");
    mVmatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vmatrix");
    mMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "matrix");
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mShaderProgram[shaderIndex]->use();
    mPmatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pmatrix");
    mVmatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vmatrix");
    mMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "matrix");
    mTextureUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
    mShaderProgram[shaderIndex]->use();
    mMatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "matrix" );
    mVmatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vmatrix" );
    mPmatrixUniform2 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pmatrix" );

    mLightColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColour" );
    mObjectColorUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColour" );
    mAmbientLightStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrength" );
    mLightPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightStrengthUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightStrength" );
    mCameraPositionUniform = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );
//    mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }

    //You get the keyboard input like this
    //movement
    if (event->key() == Qt::Key_A) controller.moveLeft = true;
    if (event->key() == Qt::Key_D) controller.moveRight = true;
    if (event->key() == Qt::Key_W) controller.moveUp = true;
    if (event->key() == Qt::Key_S) controller.moveDown = true;

    if (event->key() == Qt::Key_E)
    {
        if (!keyPressed)
        {
            door->rotate(90);
            door->setPosition3D(QVector3D{-12.0f, 0.0f, 2.0f});
            keyPressed = true;
        }
        else
        {
            door->rotate(270);
            door->setPosition3D(QVector3D{0.0f, 0.0f, 0.0f});
            keyPressed = false;
        }
    }

    if (event->key() == Qt::Key_R)
    {
        if (!cameraSwitched)
        {
            //cameraEye = QVector3D{-9.9f, 2.5, -9.9f};
            cameraEye = QVector3D{-7.5, 3.0f, -5.1};
            cameraAt = QVector3D{-7.5f, 0.0f, -7.5f};
        }

        if (cameraSwitched)
        {
            cameraEye = QVector3D{6.0f, 2.5f, 6.0f};
            cameraAt = QVector3D{0.0f, 0.0f, 0.0f};
            cameraUp = QVector3D{0.0f, 1.0f, 0.0f};
        }

        cameraSwitched = !cameraSwitched;
    }

    //toggle npc movement graph
    if (event->key() == Qt::Key_F)  npc_Curve = !npc_Curve;

    if (event->key() == Qt::Key_Space)
    {
        if (!spacePressed) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (spacePressed) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        spacePressed = !spacePressed;
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A) controller.moveLeft = false;
    if (event->key() == Qt::Key_D) controller.moveRight = false;
    if (event->key() == Qt::Key_W) controller.moveUp = false;
    if (event->key() == Qt::Key_S) controller.moveDown = false;
}

