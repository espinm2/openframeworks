#include "ofApp.h"
#include "wave.h"
#include <vector>
#include <iostream>
#include <math.h>    
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

// Constant variables
const bool    BOUNDED = true;
const double  MIN_AMP = 0.001;
const int     PART_RADIUS = 4;
const int     CLUSTER_SIZE = 10;
const int     RADIUS_CLUSTER = 8;
const int     VELOCITY = 340; // speed of sound m/s or 1000Hz
const double  INITAL_AMP = 10;
unsigned int  TOTAL_PARTICLES= 0;
const double PI_CONST  = 3.1415926535;

// Physics
double timestep = .005; // seconds

// Where we keep the particles in an oranized fasion
std::vector<std::list<WavePart*> > particleRings;


//--------------------------------------------------------------
void ofApp::setup(){
  myfont.loadFont("arial.ttf", 12);

}

//--------------------------------------------------------------
void ofApp::update(){
  // TODO Check if we need to plit
  // TODO Check if we need to kill off

  // Update the posititon of all the particles in a ring
  for(unsigned int i=0; i < particleRings.size(); i++){

    // pointer to linked list
    std::list<WavePart*> * curRing = &particleRings[i]; 
    // std::cout << "ring num: " << i << " ring size: " << curRing->size() << std::endl;

    // Get iterator for this list
    std::list<WavePart*>::iterator itr;

    // For each particle
    for (itr = curRing->begin(); itr != curRing->end(); ++itr){

      WavePart* curPart = *itr;

      // Vector along x axis
      ofVec2f pos = curPart->getPos();
      ofVec2f dir = curPart->getDir();
      ofVec2f axis(1,0);
      double radianAngle = (double)(axis.angleRad(dir));

      double vx = VELOCITY * cos(radianAngle);
      double vy = VELOCITY * sin(radianAngle);

      ofVec2f newPos( pos.x + (vx * timestep) , pos.y + (vy * timestep) );


      // Are we below a threhold
      if(curPart->getAmp() < MIN_AMP){

        // I have the element after
        itr = curRing->erase(itr);
        itr--;
        delete curPart;
        TOTAL_PARTICLES--;
        continue;
      }

      // Do we bounce, depends if we assume open space or closed space
      if(BOUNDED){
        // hit a vertical wall
        if(newPos.x < 0 || ofGetWindowWidth() < newPos.x ){
          ofVec2f newDir(-1*dir.x, dir.y);
          curPart->setDir(newDir);
        }

        // Hit a horizontal wall
        if(newPos.y < 0 || ofGetWindowHeight() < newPos.y ){
          ofVec2f newDir(dir.x, -1*dir.y);
          curPart->setDir(newDir);
        }
      }//bounded

      curPart->setPos(newPos);

      // Are we far away from other points enough to split?
      bool rightOutOfRange = false;
      bool leftOutOfRange  = false;
      
      // Check to the right if not at head
      if(itr != curRing->begin()){
        std::list<WavePart*>::iterator itrRight = itr; itrRight--;
        WavePart* rightPart = *itrRight;
        rightOutOfRange = 5 * PART_RADIUS  < curPart->getPos().distance(rightPart->getPos());
      }

      // Check to the left if not at the tail
      std::list<WavePart*>::iterator itrLeft = itr; itrLeft++;
      if(itrLeft != curRing->end()){
        WavePart* leftPart = *itrLeft;
        leftOutOfRange = 5 * PART_RADIUS  < curPart->getPos().distance(leftPart->getPos());
      }

      // If either left or right is to far, split mofo!
      if(leftOutOfRange || rightOutOfRange){
        //std::cout << "Generating Children" << std::endl;

        // Get offset
        double angleBetweenParts = (2*PI_CONST)/CLUSTER_SIZE;
        double offsetAngle = angleBetweenParts/4.0;


        // Generate left particle
        double radianAngleRight = (double)(axis.angleRad(dir) - offsetAngle);
        double vxRight = VELOCITY * cos(radianAngleRight);
        double vyRight = VELOCITY * sin(radianAngleRight);
        ofVec2f newPosRight( pos.x + (vxRight * timestep) , pos.y + (vyRight * timestep) );
        ofVec2f dirRight(newPosRight - pos);
        dirRight = dirRight.normalized();
        WavePart * newRightPart = new WavePart(newPosRight,curPart->getAmp()/3.0);
        newRightPart->setDir(dirRight);
        
        
        // Generate right particle
        double radianAngleLeft  = (double)(axis.angleRad(dir) + offsetAngle);
        double vxLeft = VELOCITY * cos(radianAngleLeft);
        double vyLeft = VELOCITY * sin(radianAngleLeft);
        ofVec2f newPosLeft( pos.x + (vxLeft * timestep) , pos.y + (vyLeft * timestep) );
        ofVec2f dirLeft(newPosLeft - pos);
        dirLeft = dirLeft.normalized();
        WavePart * newLeftPart = new WavePart(newPosLeft,curPart->getAmp()/3.0);
        newLeftPart->setDir(dirLeft);

        // Reduce amplage of middle particle
        curPart->setAmp(curPart->getAmp()/3.0);

        // Erease from list the old (middle entry) and add to Ring Structure
        // I have the element after
        itr = curRing->erase(itr);

        // Pushes the other values back
        curRing->insert(itr,newRightPart);
        curRing->insert(itr,curPart);
        curRing->insert(itr,newLeftPart);
        TOTAL_PARTICLES += 2;

        // Move back one so im at left newest
        itr--;

      }// Genereate child optinal
    
    }//ringloop for each particle

  }//vectorloop for each ring
  
  

}

//--------------------------------------------------------------
// See above
void ofApp::draw(){

  // Set the background
  ofBackground(30,10,30);


  // Draw the total number of particles in the system
  std::stringstream str;
  str << TOTAL_PARTICLES;
  std::string total = "Total Particles: " + str.str();
  myfont.drawString(total, 10,20);


  // Draw all the particles by iterating through the linked list
  // Update the posititon of all the particles in a ring
  for(unsigned int i=0; i < particleRings.size(); i++){

    std::list<WavePart*> curRing = particleRings[i]; 
    //std::cout << "ring num: " << i << " ring size: " << curRing.size() << std::endl;


    // Get iterator for this list
    std::list<WavePart*>::iterator itr;

    // For each particle
    for (itr = curRing.begin(); itr != curRing.end(); ++itr){

      WavePart* curPart = *itr;

      ofVec2f pos = curPart->getPos();

      ofSetColor(ofColor::blue);
      ofFill();
      ofCircle(pos.x, pos.y, PART_RADIUS);
    }
  }

}//draw

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  mousePressed(x,y,button);
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

  // Location Pressed / Center of cluster
  ofVec2f center(x,y);

  // Create a cluster of wave particles when you click
  double radianAngle = ( 2 * PI_CONST ) / CLUSTER_SIZE;

  // Create the dslist that will hold this list
  std::list<WavePart*> ring;


  for(int i = 0; i < CLUSTER_SIZE; i++){

    // Get changes in postiton
    int dx = (int)(cos(radianAngle * i) * RADIUS_CLUSTER);
    int dy = (int)(sin(radianAngle * i) * RADIUS_CLUSTER);
    ofVec2f pos(x + dx, y + dy);

    // Create this new particle
    WavePart * newPart = new WavePart(pos,INITAL_AMP);

    // Set Direction
    ofVec3f dir(pos-center);
    newPart->setDir(dir.getNormalized());

    // Add to collection
    ring.push_back(newPart);
    TOTAL_PARTICLES++;
  
  }

  particleRings.push_back(ring);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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


