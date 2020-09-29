#ifndef CURSOR_H
#define CURSOR_H

#include "ofMain.h"

class Cursor
{
public:
    int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;

    Cursor() {}

    void setup(int grid_res_x, int grid_res_y, float x, float y)
    {
        this->grid_res_x = grid_res_x;
        this->grid_res_y = grid_res_y;

        grid_width = (float)ofGetWidth() / grid_res_x;
        grid_height = (float)ofGetHeight() / grid_res_y;

        this->x = nearest(x, grid_width);
        this->y = nearest(y, grid_height);
    }

    float getX() { return x; }
    float getY() { return y; }

    float getGlobalX() { return x - 0.5 * grid_width; }
    float getGlobalY() { return y - 0.5 * grid_height; }

    void move(int dir)
    {
        bool moved = false;
        switch (dir)
        {
        case OF_KEY_UP:
            y -= grid_height;
            moved = true;
            break;
        case OF_KEY_RIGHT:
            x += grid_width;
            moved = true;
            break;
        case OF_KEY_DOWN:
            y += grid_height;
            moved = true;
            break;
        case OF_KEY_LEFT:
            x -= grid_width;
            moved = true;
            break;
        }

        if (moved)
        {
            x = nearest(x, grid_width);
            y = nearest(y, grid_height);

            if (x > grid_width * grid_res_x)
                x = 0;

            if (y > grid_height * grid_res_y)
                y = 0;

            if (b_is_pressed)
            {
                ofSendMessage("cursor_dragged");
            }
            else
            {
                ofSendMessage("cursor_moved");
            }
        }
    }

    void pressed()
    {
        b_is_pressed = true;
        ofSendMessage("cursor_pressed");
    }
    void released()
    {
        b_is_pressed = false;
        ofSendMessage("cursor_released");
    }

    void setPos(int x, int y)
    {
        this->x = x % grid_res_x;
        this->y = y % grid_res_y;
    }

    void setPosFromGlobal(float x, float y)
    {
        this->x = x - grid_width / 2;
        this->y = y - grid_height / 2;
        this->x = nearest(this->x, grid_width);
        this->y = nearest(this->y, grid_height);
    }

    void keyPressed(int key)
    {
        move(key);

        if (key == OF_KEY_RETURN)
            pressed();
    }

    void keyReleased(int key)
    {
        if (key == OF_KEY_RETURN)
            released();
    }

    int nearest(float a, float b)
    {
        return (int)floor(a - fmod(a, b));
    }

    void draw()
    {
        ofPushStyle();
        ofSetLineWidth(4);

        rect.set(x, y, grid_width, grid_height);

        ofSetColor(col);
        ofFill();
        ofDrawRectangle(rect);

        ofSetColor(col.getInverted());
        ofNoFill();
        ofDrawRectangle(rect);

        ofPopStyle();
    }

    void drawGrid()
    {
        ofPushStyle();

        ofBackground(255);

        ofSetColor(100);
        for (int i = 0; i < grid_res_x; i++)
        {
            ofDrawLine(i * grid_width, 0, i * grid_width, ofGetHeight());
        }

        for (int i = 0; i < grid_res_y; i++)
        {
            ofDrawLine(0, i * grid_height, ofGetWidth(), i * grid_height);
        }
        ofPopStyle();
    }

private:
    ofColor col = ofColor(255);
    int grid_res_x, grid_res_y;
    int step = 5, fast_step = 5;
    float grid_width, grid_height;
    float x, y;
    ofRectangle rect;
    bool b_is_pressed = false;
};

#endif