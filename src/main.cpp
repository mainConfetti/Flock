#include <iostream>
#include <stdlib.h>
#include <boid.h>
#include <flock.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>

#include "OpenGLWindow.h"

#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"



int main(int argc, char **argv)
{
    /*
    /// -----------------------------------------------------------------------------------------------------
    // CLASS TESTS and debug
    std::cout << "Hello World!" << std::endl;
    // Test Boid
    Boid bigBird;
    std::cout << "boid pos: ( " << bigBird.getXPos() << ", " << bigBird.getYPos() << ", " << bigBird.getZPos() << " )" << std::endl;
    bigBird.setPos(1.0, 2.0, 3.0);
    std::cout << "boid new pos: ( " << bigBird.getXPos() << ", " << bigBird.getYPos() << ", " << bigBird.getZPos() << " )" << std::endl;
    // Test Flock
    std::cout << "\n" << "\n";
    Flock flock(10);
    flock.addBoid();
    flock.addBoid();
    flock.printBoid(8);
    flock.printBoid(9);
    flock.setNeighbours(6);
    flock.queryNeighbours(6);
    flock.flock[6].Info();
    ----------------------------------------------------------------------------------------------------------*/
    // opengl stuff
    QGuiApplication app(argc, argv);
    // create an OpenGL format specifier
    QSurfaceFormat format;
    // set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples(4);
    #if defined( DARWIN)
      // at present mac osx Mountain Lion only supports GL3.2
      // the new mavericks will have GL 4.x so can change
      format.setMajorVersion(3);
      format.setMinorVersion(2);
    #else
      // with luck we have the latest GL version so set to this
      format.setMajorVersion(4);
      format.setMinorVersion(3);
    #endif
    // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
    format.setProfile(QSurfaceFormat::CoreProfile);
    // now set the depth buffer to 24 bits
    format.setDepthBufferSize(24);
    // now we are going to create our scene window
    NGLScene window;
    // and set the OpenGL format
    window.setFormat(format);
    // we can now query the version to see if it worked
    std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
    // set the window size
    window.resize(1024, 720);
    // and finally show
    window.show();

    return app.exec();





    return 0;
}

