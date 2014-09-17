#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
    // Main Rendering Loops
		void setup();
		void update();
		void draw();

    // Event Handlers
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    // create 3 children in place of a single one
    // void generate_children(WavePart* a, std::vector<WavePart*> &particles);
    ofTrueTypeFont myfont;
    
		
};
