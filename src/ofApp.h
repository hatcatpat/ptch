#pragma once

#include "ofMain.h"

#include "Component.h"
#include "Block.h"
#include "BlockManager.h"
#include "ComponentManager.h"
#include "Cursor.h"

#include "AudioManager.h"
#include "AudioBlockManager.h"

class ofApp : public ofBaseApp, public ConnectorListener
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void nodeConnectionMade() override;
	void nodeConnectionFailed(int, int) override;

	void audioOut(float *output, int buffer_size, int n_chan) override;
	void audioIn(float *input, int buffer_size, int n_chan) override;

	void cursorMoved(int, int);
	void cursorDragged(int, int, int);
	void cursorPressed(int, int, int);
	void cursorReleased(int, int, int);

	void paint();

	bool b_needs_repaint = true;
	ofFbo fbo;

	Cursor cursor;
	int currently_held_key = -1;

	float *ptr;

	ComponentManager component_manager;
	BlockManager block_manager;
	AudioManager audio_manager;
	AudioBlockManager audio_block_manager;
};
