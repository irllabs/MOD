#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "VidLayer.h"
#include "ofxCv.h"
//#include "ofxCvPiCam.h"
#include "Utils.h"
#include "ofxXmlSettings.h"
#include "ConsoleListener.h"
#include "MidiProcessor.h"

//for Raspberry Pi


// #ifdef TARGET_LINUX_ARM

//     //#include "TerminalListener.h"
//     #include "ofxRPiCameraVideoGrabber.h"

// #endif

//use extern variables to have access across files

extern float camThresh;
extern float camSoftness;
extern float camInvert;
extern float camOpacity;

extern int CAM_DEV_ID;
extern int CAM_W;
extern int CAM_H;
extern float VID_SCALE;
extern int THUMB_W;
extern int THUMB_H;
extern int CAM_FPS;

//recording layers
extern int LAYER_COUNT;
extern int FRAME_COUNT;

//render window
extern int WINDOW_W;
extern int WINDOW_H;

//MIDI Controller Numbers
extern int MIDI_PORT;
extern int MIDI_REC;
extern int MIDI_SCALE;
extern int MIDI_XPOS;
extern int MIDI_YPOS;


//default layer settings
extern float LAYER_SCALE;
extern float SHADER_THRESH;
extern float SHADER_INVERT;
extern float SHADER_SOFT;
extern float SHADER_OPACITY;
extern int SHADER_ACTIVE;
//extern string SHADER_MASK;

// Video layers
extern vector < VidLayer > vidLayers;


extern int USE_OFXCVPI;


class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
        void draw();
        //void drawMIDI();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofTexture nowTexture;
        ofPixels nowPixels;
    
        int counter;

        // for GStreamer video capture
        # ifdef TARGET_LINUX_ARM
        ofGstVideoUtils gst;
        ofTexture gsttex;
        #endif
    
        // for MIDI
        // void newMidiMessage(ofxMidiMessage& eventArgs);
        // stringstream text;
        // ofxMidiIn midiIn;
        // ofxMidiMessage midiMessage;
        MidiProcessor midiProcessor;

        // for shader params
        ofShader camShader;
        ofImage camShaderMask;
        ofTexture camShaderMaskTex;

        float camShaderActive;
        


        // // Video layers
        // vector < VidLayer > vidLayers;
    
        // Live Camera
    
        // #ifdef TARGET_LINUX_ARM
    
        //     /*
        //     void onCharacterReceived(KeyListenerEventData& e)
        //     {
        //         keyPressed((int)e.character);
        //     };
        //     */
        //     //TerminalListener consoleListener;
            
        //     OMXCameraSettings omxCameraSettings;
        //     ofxRPiCameraVideoGrabber videoGrabber;
        //     ofxCvPiCam videoGrabber2;

    
        # ifdef TARGET_OSX
        ofVideoGrabber videoGrabber;
        # endif

        # ifdef TARGET_LINUX
        ofVideoGrabber videoGrabber;
        # endif

    
        //XML settings related
        ofxXmlSettings XML;

    
    
    };
