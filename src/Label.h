#ifndef LABEL_H
#define LABEL_H

#include "ofMain.h"
#include "Component.h"

class Label : public Component
{
public:
    Label() {}
    Label(string str, float x, float y, float width, float height, int id = 0)
    {
        setup(str, x, y, width, height, id);
    }

    void setup(string str, float x, float y, float width, float height, int id = 0)
    {
        font.load("fonts/noto-bold.ttf", 10);
        this->str = str;

        font_box = font.getStringBoundingBox(str,0,0);

        Component::setup(x, y, width, height, id);

        disableBg();
        disableMouseBlocking();

        repaint();
    }

    void _paint() override
    {
        ofSetColor(255);
        font.drawString(
            str,
            getWidth()/2-font_box.getWidth()/2,
            getHeight()/2+font_box.getHeight()/2
        );
    }

    void setString(string str)
    { 
        this->str = str; 
        repaint();
    }

private:
    string str;
    ofTrueTypeFont font;
    ofRectangle font_box;
};

#endif