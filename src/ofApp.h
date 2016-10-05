#pragma once

#include <vector>

#include "ofMain.h"


class ofApp : public ofBaseApp{
private:
	ofPoint pos, target;
	float base_heading, top_angle, target_heading;
	float left, right;
	bool target_run;

	ofPoint rotation_center;

	std::vector<ofPoint> pos_log;

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseReleased(int x, int y, int button);
};
