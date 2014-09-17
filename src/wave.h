#ifndef _WAVE_H_
#define _WAVE_H_

#include "ofMain.h"
#include <iostream>
#include <vector>

class WavePart {
  public:

    // Constructor
    WavePart (ofVec2f pos, double a){
      postition = pos; amp = a;
    }

    // Setters
    void setPos(ofVec2f pos){ postition = pos; }
    void setDir(ofVec2f dir){ direction = dir; }
    void setAmp(double a) {amp = a;}

    // Getters
    ofVec2f getPos(){ return postition; }
    ofVec2f getDir(){ return direction; }
    double  getAmp(){ return amp; }

    // Debugger
    
  private:

    // Representation
    ofVec2f postition;
    ofVec2f direction;
    double  amp;
};

#endif
