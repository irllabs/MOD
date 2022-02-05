#include "ofApp.h"
// #include <opencv2/opencv.hpp>

//using namespace ofxCv;
//using namespace cv;

// Mat frame;
ofImage thisImage;

//definied as extern in ofApp.h
float camThresh;
float camSoftness;
float camInvert;
float camOpacity;
int camShaderActive;

int shaderMaskNumber;
int shaderMaskCount;

int CAM_DEV_ID;
int CAM_W;
int CAM_H;
float VID_SCALE;
int THUMB_W;
int THUMB_H;
int CAM_FPS;

//recording layers
int LAYER_COUNT;
int FRAME_COUNT;
int LAYER_DRAW;

//render window
int WINDOW_W;
int WINDOW_H;

//default layer settings
float LAYER_SCALE;
float SHADER_THRESH;
float SHADER_INVERT;
float SHADER_SOFT;
float SHADER_OPACITY;
int SHADER_ACTIVE;
string SHADER_MASK;

//debugging
int USE_OFXCVPI = 1;

// Video layers
vector < VidLayer > vidLayers;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_NOTICE);
    //-------------------------------------
    //--XML-settings-----------------------
    //-------------------------------------
    //read all initial values into extern variables from XML settings file
    
    ofLog(OF_LOG_NOTICE,"--------Reading XML file for initialization--------");
    XML.loadFile("mySettings.xml");
 
    //render window
    WINDOW_W = XML.getValue("WINDOW_W", 0);
    WINDOW_H = XML.getValue("WINDOW_H", 0);
    

    CAM_DEV_ID = XML.getValue("CAM_DEV_ID", 0);
    CAM_W = XML.getValue("CAM_W", 0);
    CAM_H = XML.getValue("CAM_H", 0);
    CAM_FPS = XML.getValue("CAM_FPS", 30);
    VID_SCALE = float(WINDOW_W) / float(CAM_W);
    ofLog(OF_LOG_NOTICE, "VID_SCALE is: " + ofToString(WINDOW_W) + " divided by " + ofToString(CAM_W) + " so: " + ofToString(VID_SCALE));

    THUMB_W = XML.getValue("THUMB_W", 0);
    THUMB_H = XML.getValue("THUMB_H", 0);
    
    //recording layers
    LAYER_COUNT = XML.getValue("LAYER_COUNT", 0);
    FRAME_COUNT = XML.getValue("FRAME_COUNT", 0);
    LAYER_DRAW = XML.getValue("LAYER_DRAW", 1);

    //default layer settings
    LAYER_SCALE = XML.getValue("LAYER_SCALE", 0.0);
    SHADER_THRESH = XML.getValue("SHADER_THRESH", 0.0);
    SHADER_INVERT = XML.getValue("SHADER_INVERT", 0.0);
    SHADER_SOFT = XML.getValue("SHADER_SOFT", 0.0);
    SHADER_OPACITY = XML.getValue("SHADER_OPACITY", 0.0);
    SHADER_ACTIVE = XML.getValue("SHADER_ACTIVE", 0);

    // //ofSetLogLevel(OF_LOG_VERBOSE);

    //-------------------------------------
    //--SETUP Shader-----------------------
    //-------------------------------------

    ofSetFrameRate(CAM_FPS);


    ofLog(OF_LOG_NOTICE,"--------setting up shaders--------");
    // setup Shader

    ofDirectory masksDir("masks");
    //populate the directory object
    masksDir.listDir();
    masksDir.allowExt("png");
    shaderMaskCount = masksDir.size();
    //go through and print out all the paths
    for(int i = 0; i < shaderMaskCount; i++){
        ofLogNotice(masksDir.getPath(i));
    }

    #ifdef TARGET_OPENGLES
        camShader.load("shadersES2/livedraw");
    #else
        if(ofIsGLProgrammableRenderer()){
            camShader.load("shadersGL3/livedraw");
        }else{
            camShader.load("shadersGL2/livedraw");
        }
    #endif
    shaderMaskNumber = 0;
    camShaderMasks.resize(shaderMaskCount);
    for(int i = 0; i < shaderMaskCount; i++){
        camShaderMasks[i].load(masksDir.getPath(i));
        camShaderMasks[i].resize(CAM_W,CAM_H);
    }

    // camShaderMasks.load("img_mask_1080c.png");
    // camShaderMask.resize(CAM_W,CAM_H);
    camShaderMaskTex = camShaderMasks[shaderMaskNumber].getTexture();
    camThresh = SHADER_THRESH;
    camSoftness = SHADER_SOFT;
    camInvert = SHADER_INVERT;
    camOpacity = SHADER_OPACITY;
    camShaderActive = SHADER_ACTIVE;
        
    //---------------------------------------
    //--SETUP CAMERA-------------------------
    //---------------------------------------

    ofLog(OF_LOG_NOTICE,"--------setting up camera--------");
    
    # ifdef TARGET_LINUX_ARM
    // When setting up a gstreamer pipeline manually:
    gst.setPipeline("v4l2src device=/dev/video0 ! image/jpeg,framerate=30/1,width=1920,height=1080 ! jpegparse ! jpegdec ! video/x-raw ! videoconvert ! video/x-raw,width=1920,height=1080,format=BGRx,framerate=30/1 ! videoconvert  ! videorate" , OF_PIXELS_RGB, true, CAM_W,CAM_H);
    gst.startPipeline();
    gst.play();
    gsttex.allocate(CAM_W,CAM_H,GL_RGB);
    # endif

    # ifdef TARGET_LINUX
    // When using the videoGrabber component
    vector<ofVideoDevice> devices = videoGrabber.listDevices();
    
    for(unsigned int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    videoGrabber.setDeviceID(CAM_DEV_ID);
    videoGrabber.setDesiredFrameRate(CAM_FPS);
    videoGrabber.initGrabber(CAM_W, CAM_H);
    ofSetVerticalSync(true);    
    # endif

    #ifdef TARGET_OSX
    // When using the videoGrabber component
    vector<ofVideoDevice> devices = videoGrabber.listDevices();
    
    for(unsigned int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    ofLogNotice() << ">> Selecting: " << devices[CAM_DEV_ID].deviceName << " @ " <<  ofToString (CAM_W) << "x" << ofToString(CAM_H) << " @ " << ofToString(CAM_FPS) << " fps";

    videoGrabber.setDeviceID(CAM_DEV_ID);
    videoGrabber.setDesiredFrameRate(CAM_FPS);
    videoGrabber.initGrabber(CAM_W, CAM_H);
    ofSetVerticalSync(true); 
    #endif      

    //-------------------------------------
    //--SETUP MIDI-------------------------
    //-------------------------------------
    ofLog(OF_LOG_NOTICE,"--------setting up MIDI--------");
    midiProcessor.setup();
    
    //-------------------------------------------
    //--SETUP VID Layers-------------------------
    //-------------------------------------------
    ofLog(OF_LOG_NOTICE,"--------setting up VidLayers--------");

    //initialize video layers
    ofLog(OF_LOG_NOTICE, "Initializing layers vector to size "+ ofToString(LAYER_COUNT));
    vidLayers.resize(LAYER_COUNT);
    
    for (int k=0;k<LAYER_COUNT;k++){
        ofLog(OF_LOG_NOTICE, "Initializing layer " + ofToString(k)+ " with frame-count "+ ofToString(FRAME_COUNT));
        vidLayers[k].setup(k,FRAME_COUNT);
    }
    
    //------------------------------------------
    ofLog(OF_LOG_NOTICE,"--------DONE with setup--------");
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0, 0, 0);
    // gate latest camera frame, assign it to a texture
    
    # ifdef TARGET_LINUX_ARM
    gst.update();
        if(gst.isFrameNew()){
           nowTexture.loadData(gst.getPixels(), GL_RGB);
    }
    # endif      

    # ifdef TARGET_LINUX
    videoGrabber.update();  
    if(videoGrabber.isFrameNew()){
        nowTexture = videoGrabber.getTexture();
    }
    # endif      

    # ifdef TARGET_OSX
    videoGrabber.update();  
    if(videoGrabber.isFrameNew()){
        nowTexture = videoGrabber.getTexture();
    }
    # endif      

    // update video layers
    for (int k=0;k<LAYER_COUNT;k++){
        //ofLog(OF_LOG_NOTICE,"...update layer "+ ofToString(k));
        //first method: use textures
        //nowPixels = videoGrabber.getPixels();
        //vidLayers[k].update(nowPixels);
    
        //second method: use FBOs
        vidLayers[k].update(nowTexture);
        }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    
    //draw processed cam
    if (SHADER_ACTIVE == 1) {
        //ofLog(OF_LOG_NOTICE, "---------drawing up with Shader");

        camShader.begin();
        camShader.setUniformTexture("maskTex", camShaderMasks[shaderMaskNumber].getTexture(), 1);
        camShader.setUniform1f("thresh", camThresh);
        camShader.setUniform1f("softness", camSoftness);
        camShader.setUniform1f("invert", camInvert);
        camShader.setUniform1f("opacity", camOpacity);
        //videoGrabber.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
        nowTexture.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);

        camShader.end();
    } else {
        //ofLog(OF_LOG_NOTICE, "---------drawing up with NO Shader");
        //videoGrabber.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
        nowTexture.draw(0,0,CAM_W * VID_SCALE,CAM_H * VID_SCALE);
    }

    
    //itereate through all recording buffers
    
    if (LAYER_DRAW == 1) {
        for (int k=0;k<LAYER_COUNT;k++){
            //ofLog(OF_LOG_NOTICE,"...drawing layer "+ ofToString(k));
            vidLayers[k].draw(nowTexture);
        }
    }
    
    // if (MIDI_DRAW == 1) { 
        // midiProcessor.draw();
        // }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '1'){
        vidLayers[0].setState(1);
    }
    if(key == '2'){
        vidLayers[1].setState(1);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '1'){
        vidLayers[0].setState(2);
    }
    if(key == '2'){
        vidLayers[1].setState(2);
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // clean up
    midiProcessor.midiIn.closePort();
    //midiProcessor.midiIn.removeListener(this);
}