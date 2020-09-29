#ifndef NODE_H
#define NODE_H

#include "Component.h"
#include "Label.h"

class Node : public Component
{
public:
    Node() {}
    Node(float x, float y, float sz, int block_id = 0, int id = 0)
    {
        setup(x, y, sz, block_id, id);
    }
    ~Node() {}

    void setup(float x, float y, float sz, int block_id = 0, int id = 0)
    {
        this->block_id = block_id;
        Component::setup(x, y, sz, sz, id);

        label.setup(name, 0, 0, sz, sz, id);
        add(&label);

        setBg(ofColor(0, 255, 0));
    }

    void mousePressedAction(int x, int y, int button, int key = -1) override
    {
        // drag_end_pos.set(x,y);
        // b_draw_drag_line = true;
        setBg(ofColor(255, 0, 0), false);

        if (b_has_connector_listener)
            connector_listener->nodePressed(block_id, getComponentId());

        repaint();
    }

    void mouseReleasedAction(int x, int y, int button, int key = -1) override
    {
        // b_draw_drag_line = false;
        if (isPressed())
        {
            setBg(ofColor(0, 0, 100), false);
        }
        else
        {
            setBg(getOrigBg(), false);
        }

        repaint();
    }

    void _paint() override
    {
        // if(b_draw_drag_line){
        // ofSetColor(0,255,255);
        // ofDrawLine( ofVec2f(0,0),  drag_end_pos - getPosition() );
        // }
    }

    void _update() override
    {
        // label.setString(to_string(get()));
    }

    void addConnectorListener(ConnectorListener *connector_listener)
    {
        this->connector_listener = connector_listener;
        b_has_connector_listener = true;
    }

    void setBlockId(int id) { this->block_id = id; }
    int getBlockId() { return block_id; }

    float get() { return *value_ptr; }
    float *getValuePtr() { return value_ptr; }
    float **getValuePtrRef() { return &value_ptr; }

    void setValuePtr(float *ptr) { value_ptr = ptr; }
    void resetValuePtr() { value_ptr = &default_value; }
    void setDefaultValue(float v) { default_value = v; }

    string getName() { return name; }
    void setName(string name) { 
        this->name = name; 
        label.setString(name);
    }

private:
    int block_id;
    bool b_has_connector_listener = false;
    ConnectorListener *connector_listener;
    Label label;
    string name = "none";

    float default_value = 0;
    float *value_ptr = &default_value;
    // bool b_draw_drag_line = false;
    // ofVec2f drag_end_pos;
};

#endif