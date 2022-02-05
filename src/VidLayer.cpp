//
//  vidLayer.cpp
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/16/17.
//
//

#include "ofApp.h"
#include "VidLayer.h"



void VidLayer::setup(int thisID, int bufSize){
    //array of textures
    //vidFrames.resize(bufSize);
    
    //array of FBOs
    vidFrames.resize(bufSize);
    
    myID = thisID;
    state = 0;                //0 = stop, 1 = record, 2 = play; stop all playheads
    playHead = 0;             //set all playheads to 0
    playDir = 1;              // 1 for forward, -1 for reverse
    recHead = 0;              //set all playheads to 0
    recCount = 0;             //set all record framecounts to 0
    recMax = bufSize;
    opacity = 1.;
    scale = LAYER_SCALE;
    thresh = SHADER_THRESH;
    invert = SHADER_INVERT;
    softness = SHADER_SOFT;
    opacity = SHADER_OPACITY;
    shaderActive = SHADER_ACTIVE;
    //x = THUMB_W * scale * myID +THUMB_W * scale/2;
    //y = THUMB_H * scale + THUMB_H/2;
    x = WINDOW_W/2;
    y = WINDOW_H/2;
    
    fboSettings.internalformat = GL_R8;
    fboSettings.width = CAM_W;
    fboSettings.height = CAM_H;

    //does this allocate the FBO?
    for (int k = 0;k<bufSize;k++){
        // vidFrames[k].allocate(CAM_W, CAM_H);
        vidFrames[k].allocate(fboSettings);
        vidFrames[k].setAnchorPercent(0.5, 0.5);
    }

    
    // setup Shader
    #ifdef TARGET_OPENGLES
        shader.load("shadersES2/livedraw");
    #else
        if(ofIsGLProgrammableRenderer()){
            shader.load("shadersGL3/livedraw");
        }else{
            shader.load("shadersGL2/livedraw");
        }
    #endif
    
    imageMask.load("img_mask_1080c.png");
    imageMask.resize(CAM_W,CAM_H);
    

}

void VidLayer::draw(ofTexture thisTexture){

    if (state==2 && recCount>0) {
        //ofLog(OF_LOG_NOTICE, "playing at " + ofToString(myID)+ " at "+ ofToString(playHead));
        
        if (shaderActive == 1) {
            shader.begin();
            shader.setUniformTexture("maskTex", imageMask.getTexture(), 1);
            shader.setUniform1f("thresh", thresh);
            shader.setUniform1f("softness", softness);
            shader.setUniform1f("invert", invert);
            shader.setUniform1f("opacity", opacity);

            vidFrames[playHead].getTexture().draw(x, y , CAM_W * scale * VID_SCALE, CAM_H * scale * VID_SCALE);
            //vidFrames[playHead].draw(x, y , CAM_W * scale * VID_SCALE, CAM_H * scale * VID_SCALE);
     
            shader.end();
        } else {
            vidFrames[playHead].draw(x, y , CAM_W * scale * VID_SCALE, CAM_H * scale * VID_SCALE);
        }

        
         } else if (state == 1) {
        
        //while recording, draw live feed
             ofTexture livefeed;
             livefeed = thisTexture;
             livefeed.setAnchorPercent(0.5, 0.5);
         if (shaderActive == 1) {
             shader.begin();
             shader.setUniformTexture("maskTex", imageMask.getTexture(), 1);
             shader.setUniform1f("thresh", thresh);
             shader.setUniform1f("softness", softness);
             shader.setUniform1f("invert", invert);
             shader.setUniform1f("opacity", opacity);
             livefeed.draw(x, y, CAM_W * scale * VID_SCALE, CAM_H * scale * VID_SCALE);
             shader.end();
         } else {

             livefeed.draw(x, y , CAM_W * scale * VID_SCALE, CAM_H * scale * VID_SCALE);
         }
     
             
             
    }
    

}


void VidLayer::update(ofTexture theTexture){
    if (state == 1) {
        //ofLog(OF_LOG_NOTICE, "recording update " + ofToString(myID)+ " at "+ ofToString(recHead));
        
        
        //draw texture onto FBO
        vidFrames[recHead].begin();
        theTexture.draw(0,0);
        vidFrames[recHead].end();
        
        //advance record head
        recHead = (recHead + 1 ) % recMax;
        
    } else if (state == 2) {
        //advance playhead
        playHead = (playHead + playDir * 1);
        if (playHead >= recCount ) {playDir = -1; playHead = recCount - 1;}
        if (playHead <= 0 ) {playDir = 1; playHead = 0;}
    
    }
}

void VidLayer::setState(int thisState){
    //if we're not already recording, start recording
    
    switch (thisState) {
        //stop
        case 0:
        //record
        case 1:
            if (state != thisState) {
                
                //match shader params to camera input's shader
                
                //try extern
                
                thresh = camThresh;
                softness = camSoftness;
                invert = camInvert;
                
                
                
                //ofLog(OF_LOG_NOTICE, "RECORD: State for layer " + ofToString(myID) + " is " + ofToString(state));
                //ofLog(OF_LOG_NOTICE, "Starting recording on layer " + ofToString(myID));
                state=1;            //set state to 1
                recHead = 0;        //reset record head
            }
        //play
        case 2:
            if (state != thisState) {
                ofLog(OF_LOG_NOTICE, "PLAY: State for layer " + ofToString(myID) + " is " + ofToString(state));
                ofLog(OF_LOG_NOTICE, "Starting playing on layer " + ofToString(myID));
                state=2;
                recCount = recHead;     //set record frame count to last frame recorded
                playHead = recCount - 1;           //set playhead to 0
                playDir = -1;
            }
    }
    
}

/*
int vidLayer::setFrame(int thisFrame, ofPixels thisPixels) {
    vidFrames[thisFrame].allocate( thisPixels);
}
 */

int VidLayer::getState() {
    return(state);
}

void VidLayer::setSpeed(float speed){
    
}

void VidLayer::setShaderParams(float thisThresh, float thisSoftness, float thisInvert){
    thresh = thisThresh;
    softness = thisSoftness;
    invert = thisInvert;
}

void VidLayer::setScale(float thisScale){
    scale =thisScale;
}

void VidLayer::setOpacity(float thisOpacity){
    // ofLog(OF_LOG_NOTICE, "VidLayer: Setting opacity for layer " + ofToString(myID)  + " to " + ofToString(thisOpacity));
    opacity =thisOpacity;
}

void VidLayer::setThresh(float thisThresh){
    thresh =thisThresh;
}

void VidLayer::setInvert(float thisInvert){
    invert =thisInvert;
}

void VidLayer::setPos(int positionX, int positionY){

    x = positionX ;
    y = positionY;
    //ofLog(OF_LOG_NOTICE, "---setting position of " + ofToString(myID) + " to  " + ofToString(x) + " " + ofToString(y));

}

void VidLayer::setXPos(int positionX){
    x = positionX ;
    //ofLog(OF_LOG_NOTICE, "---setting X position of " + ofToString(myID) + " to  " + ofToString(x));


}

void VidLayer::setYPos(int positionY){

    y = positionY;
    //ofLog(OF_LOG_NOTICE, "---setting X position of " + ofToString(myID) + " to  " + ofToString(y));

}
