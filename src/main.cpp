#include "ofMain.h"
#include "ofApp.h"
#include "Consts.h"

#include "ofAppGLFWWindow.h"
#include "ofEvents.h"

#include "ofBaseApp.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"
#include "ofFileUtils.h"
#include "ofEvents.h"
#include "ofPixels.h"

//#define GLFW_INCLUDE_NONE
//#include "GLFW/glfw3.h"


//========================================================================
int main( ){
	
    
    //#ifdef TARGET_LINUX_ARM
        // ofGLESWindowSettings settings;
        // settings.width = 1024;
        // settings.height = 768;
        // settings.glesVersion = 2;
        // ofCreateWindow(settings);
        //ofSetupOpenGL(1280,720, OF_WINDOW);
    // #elif defined(TARGET_OSX)
    //     ofSetupOpenGL(640,480,OF_WINDOW);
    //     ofGLWindowSettings settings;
    //     settings.setGLVersion(3,2);
    
    // #endif
    // int screenWidth = ofGetScreenWidth(); 
    // int screenHeight = ofGetScreenHeight();

    // ofLogNotice() << "Width:" << screenWidth << "Height:  " << screenHeight;
    // ofSetupOpenGL(1920,1080, OF_FULLSCREEN);	
    ofSetupOpenGL(640,480, OF_WINDOW);	

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
