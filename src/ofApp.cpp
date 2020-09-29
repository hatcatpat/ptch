#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    audio_manager.setup();

    ofBackground(0);
    ofSetBackgroundAuto(false);

    cursor.setup(64, 64, 10, 10);

    block_manager.setup(&component_manager, this);

    audio_block_manager.setup(&audio_manager, &block_manager);

    audio_block_manager.addOscillatorBlock(new OscillatorBlock(0.8, 220, 880, 1, false, 100, 100));
    audio_block_manager.addOscillatorBlock(new OscillatorBlock(440, -1, 1, 2, true, 300, 400));

    // audio_block_manager.connect(0,3,1,0);

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    paint();
}

void ofApp::paint()
{
    fbo.begin();
    ofClear(0, 0);

    ofPushStyle();
    ofBackground(255);

    ofSetColor(100);
    for (int i = 0; i < (int)ceil((float)ofGetWidth() / GRID_SZ); i++)
    {
        ofDrawLine(i * GRID_SZ, 0, i * GRID_SZ, ofGetHeight());
    }

    for (int i = 0; i < (int)ceil((float)ofGetHeight() / GRID_SZ); i++)
    {
        ofDrawLine(0, i * GRID_SZ, ofGetWidth(), i * GRID_SZ);
    }
    ofPopStyle();

    component_manager.draw();

    fbo.end();

    b_needs_repaint = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    component_manager.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // cursor.drawGrid();

    if (!b_needs_repaint)
        b_needs_repaint = b_needs_repaint || component_manager.needsRedraw();

    if (b_needs_repaint)
    {
        paint();
    }

    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());

    // cursor.draw();
}

void ofApp::audioOut(float *output, int buffer_size, int n_chan) { audio_manager.audioOut(output, buffer_size, n_chan); }
void ofApp::audioIn(float *input, int buffer_size, int n_chan) { audio_manager.audioIn(input, buffer_size, n_chan); }

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    cursor.keyPressed(key);

    if (key != currently_held_key)
        currently_held_key = key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    cursor.keyReleased(key);
    currently_held_key = -1;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    cursor.setPosFromGlobal(x, y);
    cursorMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    cursorDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    cursor.setPosFromGlobal(x, y);
    cursorPressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    cursorReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    ofLog() << msg.message << " ";
    if (msg.message == "cursor_moved")
    {
        // cursorMoved(cursor.getGlobalX(), cursor.getGlobalY());
    }
    else if (msg.message == "cursor_pressed")
    {
        // cursorPressed(cursor.getGlobalX(), cursor.getGlobalY(), 0);
    }
    else if (msg.message == "cursor_released")
    {
        // cursorReleased(cursor.getGlobalX(), cursor.getGlobalY(), 0);
    }
    else if (msg.message == "cursor_dragged")
    {
        // cursorDragged(cursor.getGlobalX(), cursor.getGlobalY(), 0);
    }
}

void ofApp::cursorMoved(int x, int y)
{
    component_manager.mouseMoved(x, y, currently_held_key);
}
void ofApp::cursorDragged(int x, int y, int button)
{
    component_manager.mouseDragged(x, y, button, currently_held_key);
}
void ofApp::cursorPressed(int x, int y, int button)
{
    bool connector_waiting = b_node_currently_selected;

    component_manager.mousePressed(x, y, button, currently_held_key);

    if (connector_waiting && b_node_currently_selected)
    {
        ofLog() << "dropped connector ";
        b_node_currently_selected = false;
        current_node[0] = -1;
        current_node[1] = -1;
        new_node[0] = -1;
        new_node[1] = -1;
    }

}
void ofApp::cursorReleased(int x, int y, int button)
{
    component_manager.mouseReleased(x, y, button, currently_held_key);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
void ofApp::nodeConnectionMade()
{

    component_manager.getConnectorComponent()->addConnector(
        block_manager.getBlock(current_node[0])->getNode(current_node[1]),
        block_manager.getBlock(new_node[0])->getNode(new_node[1]));

    block_manager.getBlock(current_node[0])->getNode(current_node[1])->resetBg();
    block_manager.getBlock(new_node[0])->getNode(new_node[1])->resetBg();

    audio_block_manager.connect(current_node[0], current_node[1], new_node[0], new_node[1]);

    ofLog() << "connection made ";
}

void ofApp::nodeConnectionFailed(int block, int node)
{
    block_manager.getBlock(current_node[0])->getNode(current_node[1])->resetBg();
    block_manager.getBlock(block)->getNode(node)->resetBg();

    ofLog() << "connection failed ";
}