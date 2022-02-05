//
//  vidLayer.cpp
//  VideoDelayTest
//
//  Created by Ali Momeni on 5/16/17.
//
//

#include "ofApp.h"

void MidiProcessor::setup(){
    ofLog(OF_LOG_NOTICE,"Reading MIDI XML file for initialization.....");
    XML.loadFile("myMidiSettings.xml");

    //MIDI Controller Numbers= 
    MIDI_DRAW = XML.getValue("MIDI_DRAW", 0);
    MIDI_PORT = XML.getValue("MIDI_PORT", 0);
    MIDI_SCALE = XML.getValue("MIDI_SCALE", 0);
    MIDI_XPOS = XML.getValue("MIDI_XPOS", 0);
    MIDI_YPOS = XML.getValue("MIDI_YPOS", 0);
    MIDI_REC_CC = XML.getValue("MIDI_REC_CC", 64);


    //MIDI Note numbers

    MIDI_REC_01 = XML.getValue("MIDI_REC_01", 0);
    MIDI_REC_02 = XML.getValue("MIDI_REC_02", 0);
    MIDI_REC_03 = XML.getValue("MIDI_REC_03", 0);
    MIDI_REC_04 = XML.getValue("MIDI_REC_04", 0);
    MIDI_REC_05 = XML.getValue("MIDI_REC_05", 0);
    MIDI_REC_06 = XML.getValue("MIDI_REC_06", 0);
    MIDI_REC_07 = XML.getValue("MIDI_REC_07", 0);
    MIDI_REC_08 = XML.getValue("MIDI_REC_08", 0);
    
    MIDI_SHADER_BYPASS = XML.getValue("MIDI_SHADER_BYPASS", 0);
    MIDI_SHADER_THRESH = XML.getValue("MIDI_SHADER_THRESH", 0);
    MIDI_SHADER_SOFT = XML.getValue("MIDI_SHADER_SOFT", 0);
    MIDI_SHADER_INVERT_ON = XML.getValue("MIDI_SHADER_INVERT_ON", 0);
    MIDI_SHADER_INVERT_OFF = XML.getValue("MIDI_SHADER_INVERT_OFF", 0);
    MIDI_SHADER_OPACITY = XML.getValue("MIDI_SHADER_OPACITY", 0);
    MIDI_SHADER_CAM_OPACITY = XML.getValue("MIDI_SHADER_CAM_OPACITY", 0);
    MIDI_SHADER_CAM_THRESH = XML.getValue("MIDI_SHADER_CAM_THRESH", 0);
    MIDI_SHADER_CAM_SOFT = XML.getValue("MIDI_SHADER_CAM_SOFT", 0);
    MIDI_SHADER_CAM_INVERT_ON = XML.getValue("MIDI_SHADER_CAM_INVERT_ON", 0);
    MIDI_SHADER_CAM_INVERT_OFF = XML.getValue("MIDI_SHADER_CAM_INVERT_OFF", 0);


    MIDI_MASK_NEXT = XML.getValue("MIDI_MASK_NEXT", 0);
    MIDI_MASK_PREV = XML.getValue("MIDI_MASK_PREV", 0);
    
    // ofLog(OF_LOG_NOTICE,"MIDI_SHADER_CAM_OPACITY: " + ofToString(MIDI_SHADER_CAM_OPACITY));


    ofLog(OF_LOG_NOTICE,"Setting MIDI port to " + ofToString(MIDI_PORT));

    midiIn.listInPorts(); 
    midiIn.openPort(MIDI_PORT);
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
    midiIn.setVerbose(true);

}

void MidiProcessor::draw(ofxMidiMessage& msg){
    // make a copy of the latest message
    midiMessage = msg;
    int layNum;

    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 400);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 410);
    text.str(""); // clear
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 440);
    text.str(""); // clear
    //ofDrawRectangle(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 460);
    text.str(""); // clear


    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 480);
    text.str(""); // clear

    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 500);
    text.str(""); // clear


}

void MidiProcessor::newMidiMessage(ofxMidiMessage& msg){

    // make a copy of the latest message
    midiMessage = msg;
    int layNum;
    //update midi messages
    
    //record on/off messages
    if (midiMessage.status == MIDI_CONTROL_CHANGE) {
        //ofLog(OF_LOG_NOTICE, "CC " + ofToString(midiMessage.control) + " " + ofToString(midiMessage.value) );

        //record on and off messages
        if (midiMessage.control >=MIDI_REC_CC && midiMessage.control <(MIDI_REC_CC+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_REC_CC;
            if (midiMessage.value==127) {vidLayers[layNum].setState(1);} else {vidLayers[layNum].setState(2);}
        }

        //opacity messages
        if (midiMessage.control >=MIDI_SHADER_OPACITY && midiMessage.control <(MIDI_SHADER_OPACITY+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_SHADER_OPACITY;
            float scaleVal = 1.;
            scaleVal = Utils::scale(midiMessage.value,0,127,0,1);            
            vidLayers[layNum].setOpacity(scaleVal);
        }

        //thresh messages
        if (midiMessage.control >=MIDI_SHADER_THRESH && midiMessage.control <=(MIDI_SHADER_THRESH+LAYER_COUNT) ) {
            layNum = midiMessage.control - MIDI_SHADER_THRESH;
            float scaleVal = 1.;
            scaleVal = Utils::scale(midiMessage.value,0,127,0,1);            
            vidLayers[layNum].setThresh(scaleVal);
        }

        //smoothing message modifier key



        //scale messages
        // if (midiMessage.control >=MIDI_SCALE && midiMessage.control <=(MIDI_SCALE+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_SCALE;
        //     float scaleVal = 1.;
        //     if (midiMessage.value < 64) {  scaleVal = Utils::scale(midiMessage.value,0,64,0,1);} else { scaleVal = Utils::scale(midiMessage.value,64,127,1,5);}
        //     //ofLog(OF_LOG_NOTICE, "Setting scale to " + ofToString(scaleVal));
            
        //     vidLayers[layNum].setScale(scaleVal);
        // }
        
        // //X position messages
        // if (midiMessage.control >=MIDI_XPOS && midiMessage.control <=(MIDI_XPOS+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_XPOS ;
        //     float xPosition =Utils::scale(midiMessage.value,0,127,0,WINDOW_W);
        //     vidLayers[layNum].setXPos(xPosition);
        // }
        
        // //Y position messages
        // if (midiMessage.control >=MIDI_YPOS && midiMessage.control <=(MIDI_YPOS+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_YPOS;
        //     float yPosition = Utils::scale(midiMessage.value,0,127,WINDOW_H,0);
        //     vidLayers[layNum].setYPos(yPosition);
        // }
        
        // //Shader messages
        
        
        // if (midiMessage.control ==MIDI_SHADER_BYPASS ) {
        //     camShaderActive = Utils::scale(midiMessage.value,0,127,0., 1.);
        // }
        
        if (midiMessage.control == MIDI_SHADER_CAM_OPACITY ) {
            float scaleVal = Utils::scale(midiMessage.value,0,127,0., 1.);

            ofLog(OF_LOG_NOTICE, "Setting camopacity to " + ofToString(scaleVal));

            camOpacity = scaleVal;
        }
        
        if (midiMessage.control ==MIDI_SHADER_CAM_THRESH ) {
            camThresh = Utils::scale(midiMessage.value,0,127,0., 1.);
        }

        if (midiMessage.control == MIDI_MASK_NEXT  && midiMessage.value == 127) {
            shaderMaskNumber = (shaderMaskNumber + 1 ) % shaderMaskCount;
        }

        if (midiMessage.control == MIDI_MASK_PREV && midiMessage.value == 127) {
            shaderMaskNumber = (shaderMaskNumber - 1  + shaderMaskCount ) % shaderMaskCount;
        }
        
        // if (midiMessage.control ==MIDI_SHADER_CAM_SOFT ) {
        //     camSoftness = Utils::scale(midiMessage.value,0,127,0., 1.);
        // }
        
        // if (midiMessage.control ==MIDI_SHADER_CAM_INVERT_ON ) {
        //     if (midiMessage.value == 127) {camInvert = 1.;}
        // }
        
        // if (midiMessage.control ==MIDI_SHADER_CAM_INVERT_OFF ) {
        //     if (midiMessage.value == 127) {camInvert = 0.;}
        // }

        // //invert messages

        // if (midiMessage.control >=MIDI_SHADER_INVERT_ON && midiMessage.control <=(MIDI_SHADER_INVERT_ON+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_SHADER_INVERT_ON;
        //     if (midiMessage.value == 127) {vidLayers[layNum].setInvert(1.);}
        // }
        
        // if (midiMessage.control >=MIDI_SHADER_INVERT_OFF && midiMessage.control <=(MIDI_SHADER_INVERT_OFF+LAYER_COUNT) ) {
        //     layNum = midiMessage.control - MIDI_SHADER_INVERT_ON;
        //     if (midiMessage.value == 127) {vidLayers[layNum].setInvert(0.);}
        // }
    
        
    } else if (midiMessage.status == MIDI_NOTE_ON || midiMessage.status == MIDI_NOTE_OFF) //
    {
        if (midiMessage.pitch == MIDI_REC_01) {layNum = 0; } else
        if (midiMessage.pitch == MIDI_REC_02) {layNum = 1; } else
        if (midiMessage.pitch == MIDI_REC_03) {layNum = 2; } else
        if (midiMessage.pitch == MIDI_REC_04) {layNum = 3; } else
        if (midiMessage.pitch == MIDI_REC_05) {layNum = 4; } else
        if (midiMessage.pitch == MIDI_REC_06) {layNum = 5; } else
        if (midiMessage.pitch == MIDI_REC_07) {layNum = 6; } else
        if (midiMessage.pitch == MIDI_REC_08) {layNum = 7; } 
        
        if (midiMessage.velocity==127) {vidLayers[layNum].setState(1);} else {vidLayers[layNum].setState(2);}
    }

    

}
   