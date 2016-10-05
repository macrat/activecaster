#include "ofApp.h"

#define STEP       1

#define WHEEL_DISTANCE   64
#define CASTER_DISTANCE  32
#define WHEEL_SIZE       4


void ofApp::setup(){
	pos.set(ofGetWidth()/2, ofGetHeight()/2);
	base_heading = top_angle = 0;
	left = right = 0;
	target_run = false;

	ofSetBackgroundColor(ofColor::black);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
}


void ofApp::update(){
	ofPoint center_pos{
		pos.x + sin(-base_heading + PI) * CASTER_DISTANCE,
		pos.y + cos(-base_heading + PI) * CASTER_DISTANCE
	};
	pos_log.push_back(center_pos);
	while(pos_log.size() > 255){
		pos_log.erase(pos_log.begin());
	}

	if(target_run){
		float target_angle = -atan2(center_pos.x - target.x, center_pos.y - target.y) - base_heading + PI/2;
		float target_distance = center_pos.distance(target);
		ofPoint local_target(
			sin(target_angle) * target_distance,
			cos(target_angle) * target_distance
		);

		right = (local_target.x / WHEEL_SIZE) + (WHEEL_DISTANCE * local_target.y) / (2 * WHEEL_SIZE * CASTER_DISTANCE);
		left = (local_target.x / WHEEL_SIZE) + (-WHEEL_DISTANCE * local_target.y) / (2 * WHEEL_SIZE * CASTER_DISTANCE);

		right /= 50;
		left /= 50;

		top_angle = target_heading - base_heading;
	}

	if(left == right){
		pos.x -= cos(PI/2 + base_heading) * left;
		pos.y -= sin(PI/2 + base_heading) * right;

		rotation_center.set(-100, -100);
	}else{
		const double rate = (double)left / (double)right;
		const double center_distance = (-1 - 1/(rate-1)) * WHEEL_DISTANCE + WHEEL_DISTANCE/2.0;

		if(center_distance == 0){
			const double run_angle = left / (WHEEL_DISTANCE/2.0);

			base_heading += run_angle;

			rotation_center.set(pos);
		}else{
			const double run_distance = (left + right) / 2.0;
			const double run_angle = run_distance / center_distance;

			rotation_center.set(
				pos.x - cos(base_heading)*center_distance,
				pos.y - sin(base_heading)*center_distance
			);

			pos.set(
				rotation_center.x - cos(PI + base_heading - run_angle)*center_distance,
				rotation_center.y - sin(PI + base_heading - run_angle)*center_distance
			);
			base_heading -= run_angle;
		}
	}
}


void ofApp::draw(){
	ofSetColor(ofColor::lightYellow, 64);
	for(const ofPoint& x: pos_log){
		ofDrawCircle(x, 4);
	}

	ofPushMatrix();
	ofTranslate(pos);
	ofRotate(base_heading * 180 / PI);

	ofPushMatrix();
	ofTranslate(0, -CASTER_DISTANCE);
	ofRotate(top_angle * 180 / PI);
	ofSetColor(ofColor::lightCyan);
	ofDrawRectangle(-WHEEL_DISTANCE, -WHEEL_DISTANCE, WHEEL_DISTANCE*2, WHEEL_DISTANCE*2);
	ofDrawLine(-WHEEL_DISTANCE, -WHEEL_DISTANCE, WHEEL_DISTANCE, WHEEL_DISTANCE);
	ofDrawLine(-WHEEL_DISTANCE, WHEEL_DISTANCE, WHEEL_DISTANCE, -WHEEL_DISTANCE);
	ofDrawLine(0, -WHEEL_DISTANCE*0.7, 0, -WHEEL_DISTANCE);
	ofPopMatrix();

	ofNoFill();
	ofSetColor(ofColor::white);

	ofDrawLine(0, -CASTER_DISTANCE, 0, 0);
	ofDrawLine(-WHEEL_DISTANCE/2-10, 0, WHEEL_DISTANCE/2+10, 0);

	ofSetColor(ofColor::yellow);
	ofDrawCircle(0, -CASTER_DISTANCE, 8);

	ofSetColor(ofColor::skyBlue);
	ofDrawRectangle(-WHEEL_DISTANCE/2-10, -20, 20, 40);

	ofSetColor(ofColor::pink);
	ofDrawRectangle(WHEEL_DISTANCE/2-10, -20, 20, 40);

	ofPopMatrix();

	ofSetColor(ofColor::red);
	ofDrawCircle(rotation_center, 8);

	if(target_run){
		ofSetColor(ofColor::green);
		ofDrawCircle(target, 8);
	}
}


void ofApp::keyPressed(int key){
	target_run = false;

	switch(key) {
	case 'a':
		left += STEP;
		break;
	case 'z':
		left -= STEP;
		break;
	case 'd':
		right += STEP;
		break;
	case 'c':
		right -= STEP;
		break;
	case 's':
		left += STEP;
		right += STEP;
		break;
	case 'x':
		left -= STEP;
		right -= STEP;
		break;
	case 'q':
		top_angle -= 0.1;
		break;
	case 'e':
		top_angle += 0.1;
		break;
	case ' ':
		setup();
		break;
	}
}


void ofApp::mouseReleased(int x, int y, int button) {
	target.set(x, y);
	target_run = true;
	if(!target_run){
		target_heading = base_heading + top_angle;
	}
}
