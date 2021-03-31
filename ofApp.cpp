#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->cap.open("D:\\MP4\\Aerial Shot Of City.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(cv::CAP_PROP_POS_FRAMES, 0);
		return;
	}

	cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
	cv::resize(src, this->frame, this->cap_size);
	cv::flip(this->frame, this->frame, 0);

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofTranslate(0, -320);

	for (int x = -640; x < 640; x += 40) {

		ofMesh face, line;
		line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

		for (int y = 0; y <= ofGetHeight(); y += 1) {

			auto noise_value = ofNoise(x * 0.005, y * 0.001 + ofGetFrameNum() * 0.005);
			float angle = 0;
			if (noise_value > 0.75) { angle = ofMap(noise_value, 0.75, 1.0, 0, PI * 0.3); }
			if (noise_value < 0.25) { angle = ofMap(noise_value, 0.25, 0.0, 0, PI * -0.3); }
			auto rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));

			face.addVertex(glm::vec4(x - 20, y, 0, 0) * rotation);
			face.addVertex(glm::vec4(x + 20, y, 0, 0) * rotation);

			face.addTexCoord(glm::vec3(x + 640, y, 0));
			face.addTexCoord(glm::vec3(x + 640 + 40, y, 0));

			line.addVertex(glm::vec4(x - 20, y, 0, 0) * rotation);
			line.addVertex(glm::vec4(x + 20, y, 0, 0) * rotation);

			if (face.getNumVertices() > 2) {

				face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 4);
				face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

				line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 3);
				line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 4);
			}
		}

		line.addIndex(0); line.addIndex(1);
		line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 2);

		ofSetColor(255);
		this->image.bind();
		face.draw();
		this->image.unbind();
		
		ofSetColor(39);
		line.drawWireframe();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}