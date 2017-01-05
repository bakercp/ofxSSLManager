//
// Copyright (c) 2009 Christopher Baker <http://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"
#include "ofSSLManager.h"


void ofApp::setup()
{
    auto ctx = ofSSLManager::getDefaultClientContext();

    // etc.  Usually used internally w/ ofxSMTP, ofxHTTP, etc.

    std::cout << "Nothing to see here." << std::endl;
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);
    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
