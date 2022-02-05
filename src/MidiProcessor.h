//
//  vidLayer.hpp
//  MidiProcessor: Class for processing midi input
//
//  Created by Ali Momeni on 2/4/22.
//
//

// #ifndef MidiProcessor_hpp
// #define MidiProcessor_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxMidi.h"

class MidiProcessor: public ofxMidiListener {
public:
    //MIDI Controller Numbers
    int MIDI_DRAW;
    int MIDI_PORT;
    int MIDI_REC_CC;
    int MIDI_REC_01;
    int MIDI_REC_02;
    int MIDI_REC_03;
    int MIDI_REC_04;
    int MIDI_REC_05;
    int MIDI_REC_06;
    int MIDI_REC_07;
    int MIDI_REC_08;

    int MIDI_SCALE;
    int MIDI_XPOS;
    int MIDI_YPOS;

    int MIDI_SHADER_BYPASS;
    int MIDI_SHADER_THRESH;
    int MIDI_SHADER_SOFT;
    int MIDI_SHADER_INVERT_ON;
    int MIDI_SHADER_INVERT_OFF;
    int MIDI_SHADER_OPACITY;
    int MIDI_SHADER_CAM_OPACITY;
    int MIDI_SHADER_CAM_THRESH;
    int MIDI_SHADER_CAM_SOFT;
    int MIDI_SHADER_CAM_INVERT_ON;
    int MIDI_SHADER_CAM_INVERT_OFF;
    int MIDI_MASK_NEXT;
    int MIDI_MASK_PREV;
    int MIDI_SHADER_SOFTNESS_MODIFIER;
    int MIDI_SHADER_THRESH_MODIFIER;

    //state variables
    int knobMode;  //0 (default) = knob for shader threshold, 1 = knob for shader smoothing

    //XML settings related
    ofxXmlSettings XML;

    // for midi messages
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    stringstream text;

    //methods
    void setup();
    void draw(ofxMidiMessage& msg);
    // void update(ofxMidiMessage& msg;
    void newMidiMessage(ofxMidiMessage& eventArgs);


};

// #endif /* vidLayer_hpp */
