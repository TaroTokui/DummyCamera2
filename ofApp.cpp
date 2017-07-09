#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetCircleResolution(128);

	// init fbo
	shadowFbo.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	shadowHeatmapFbo.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	prevFbo.allocate(CAMERA_WIDTH, CAMERA_HEIGHT);
	prevFbo.begin();
	ofBackground(0);
	prevFbo.end();

	// init spout
	spout1.init("shadow", CAMERA_WIDTH, CAMERA_HEIGHT);
	spout2.init("heatmap", CAMERA_WIDTH, CAMERA_HEIGHT);

	// load shaders
	reloadShader();

	// gui
	params.setName("settings");
	params.add(radius.set("rarius", 100, 1, 500));
	params.add(increaseParam.set("increase", 0.02, 0.0, 0.2));
	params.add(decreaseParam.set("decrease", 0.1, 0.0, 1.0));
	gui.setup(params);

	// init video
	player.load("movies/test.mov");
	player.play();
	player.setLoopState(OF_LOOP_NORMAL);
}

//--------------------------------------------------------------
void ofApp::update() {

	player.update();
	if (player.isFrameNew()) {
		tex.loadData(player.getPixels());
	}

	shadowFbo.begin();
	ofClear(0, 0, 0, 255);
	if (tex.isAllocated()) {
		tex.draw(0, 0);
	}
	ofDrawCircle(mouseX, mouseY, radius);
	shadowFbo.end();

	// update heatmap
	shadowHeatmapFbo.begin();
	heatmap.begin();
	heatmap.setUniformTexture("preTex", prevFbo.getTexture(), 1);
	heatmap.setUniform1f("increaseParam", increaseParam);
	heatmap.setUniform1f("decreaseParam", decreaseParam);
	heatmap.setUniform2f("iResolution", resolution.x, resolution.y);
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	shadowFbo.draw(0, 0);
	heatmap.end();
	shadowHeatmapFbo.end();

	prevFbo.begin();
	shadowHeatmapFbo.draw(0, 0);
	prevFbo.end();

	// send texture
	spout1.send(shadowFbo.getTexture());
	spout2.send(shadowHeatmapFbo.getTexture());

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	shadowFbo.draw(0, 0);
	gui.draw();

	ofDrawBitmapString("spout channel name: shadow", 20, ofGetWindowHeight() - 20);
}

void ofApp::exit() {
	spout1.release();
	spout2.release();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'r')
	{
		reloadShader();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


//--------------------------------------------------------------
void ofApp::reloadShader()
{
	heatmap.load("", "shaders/heatmap.frag");
	cout << "shader loaded." << endl;
}