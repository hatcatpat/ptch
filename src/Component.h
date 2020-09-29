#ifndef COMPONENT_H
#define COMPONENT_H

#define GRID_SZ (25)

#include "Utils.h"
#include "MovementListener.h"

class Component
{
public:
    Component() {}
    Component(float x, float y, float width, float height, int id = 0)
    {
        setup(x, y, width, height, id);
    }
    ~Component()
    {
        for (int i = 0; i < (int)child_components.size(); i++)
        {
            delete child_components[i];
        }
        child_components.clear();
    }

    void setup(float x, float y, float width, float height, int id = 0)
    {
        this->x = nearest(x, GRID_SZ);
        this->y = nearest(y, GRID_SZ);
        this->width = width;
        this->height = height;
        this->id = id;

        this->global_x = x;
        this->global_y = y;

        fbo.allocate(width, height, GL_RGBA);

        b_setup = true;

        paint();
    }

    void add(Component *child)
    {
        child->setParent(this);
        child_components.push_back(child);
    }

    Component *getChild(int i)
    {
        if (i >= 0)
        {
            return child_components[i % ((int)child_components.size())];
        }
        else
        {
            return NULL;
        }
    }

    void setParent(Component *parent)
    {
        this->parent = parent;
        this->global_x = x + parent->getGlobalX();
        this->global_y = y + parent->getGlobalY();
        b_has_parent = true;
    }
    Component *getParent() { return parent; }

    void setComponentId(int id) { this->id = id; }
    int getComponentId() { return id; }

    float getX() { return x; }
    float getY() { return y; }
    ofVec2f getPosition() { return ofVec2f(x, y); }
    float getGlobalX() { return global_x; }
    float getGlobalY() { return global_y; }
    ofVec2f getGlobalPosition() { return ofVec2f(global_x, global_y); }
    float getWidth() { return width; }
    float getHeight() { return height; }

    void disableMouseBlocking() { b_blocks_mouse = false; }

    ofVec2f getDragStartPos() { return drag_start_pos; }

    virtual void _paint() {}
    void paint() // draws component to FBO
    {
        ofPushStyle();
        fbo.begin();
        ofClear(255, 0);

        if (b_has_bg)
        {
            ofSetColor(bg);
            ofDrawRectangle(0, 0, width, height);
        }

        _paint();

        fbo.end();
        ofPopStyle();
    }

    void repaint()
    {
        paint();
        redraw();
    }

    void redraw() // tells main that this components needs to be drawn
    {
        b_needs_redraw = true;
        if (b_has_parent)
        {
            parent->redraw();
        }
    }

    void draw(float gx = 0, float gy = 0) // draws FBO to screen beneath children
    {
        if (b_setup)
        {
            if (b_has_parent)
            {
                global_x = gx + x;
                global_y = gy + y;
            }

            fbo.draw(gx + x, gy + y, width, height);

            for (auto value : child_components)
            {
                value->draw(gx + x, gy + y);
            }

            b_needs_redraw = false;
        }
    }

    virtual void _update(){}

    void update()
    {
        if (b_setup)
        {
            _update();

            for (auto value : child_components)
            {
                value->update();
            }
        }
    }

    void moveRelative(float rel_x, float rel_y)
    {
        setRelativePosition(x + rel_x, y + rel_y);
    }

    void setRelativePosition(float x, float y)
    {
        this->x = nearest(x, GRID_SZ);
        this->y = nearest(y, GRID_SZ);

        global_x = x;
        global_y = y;

        if (b_has_parent)
        {
            global_x += parent->getGlobalX();
            global_y += parent->getGlobalY();
        }

        moved();

        repaint();
    }

    void moved()
    {
        for (auto child : child_components)
            child->parentMoved();

        for (auto m : movement_listeners)
            m->moved();
    }

    void parentMoved()
    {
        moved();
    }

    void setBg(ofColor new_bg, bool permanent = true)
    {
        bg = new_bg;
        if (permanent)
            orig_bg = bg;
        repaint();
    }
    void resetBg()
    {
        bg = orig_bg;
        repaint();
    }
    ofColor getBg() { return bg; }
    ofColor getOrigBg() { return orig_bg; }

    void disableBg()
    {
        b_has_bg = false;
        repaint();
    }
    void enableBg()
    {
        b_has_bg = true;
        repaint();
    }

    void addMovementListener(MovementListener *movement_listener)
    {
        movement_listeners.push_back(movement_listener);
    }

    bool needsRedraw() { return b_needs_redraw; }
    bool isPressed() { return b_is_pressed; }

    //===============================================================================

    bool mouseHit(int x, int y)
    {
        if (
            this->x <= x &&
            x < this->x + width &&
            this->y <= y &&
            y < this->y + height)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual void mouseMovedAction(int x, int y, int key = -1) {}
    bool mouseMoved(int x, int y, int key = -1)
    {
        if (mouseHit(x, y))
        {
            for (auto const &value : child_components)
            {
                if (value->mouseMoved(x - this->x, y - this->y, key))
                    return b_blocks_mouse;
            }

            mouseMovedAction(x, y, key);

            return b_blocks_mouse;
        }
        else
        {
            return false;
        }
    }

    virtual void mousePressedAction(int x, int y, int button, int key = -1) {}
    bool mousePressed(int x, int y, int button, int key = -1)
    {
        if (mouseHit(x, y))
        {
            b_is_pressed = true;
            drag_start_pos.set(x - getGlobalX(), y - getGlobalY());

            for (auto const &value : child_components)
            {
                if (value->mousePressed(x - this->x, y - this->y, button, key))
                    return b_blocks_mouse;
            }

            mousePressedAction(x, y, button, key);

            return b_blocks_mouse;
        }
        else
        {
            return false;
        }
    }

    virtual void mouseReleasedAction(int x, int y, int button, int key = -1) {}
    virtual void otherMouseDragEnded() {}
    bool mouseReleased(int x, int y, int button, int key = -1)
    {
        if (b_is_pressed)
        {
            b_is_pressed = false;

            for (auto const &value : child_components)
            {
                if (value->mouseReleased(x - this->x, y - this->y, button, key))
                    return b_blocks_mouse;
            }

            mouseReleasedAction(x, y, button, key);

            return b_blocks_mouse;
        }
        else
        {
            if (mouseHit(x, y))
                otherMouseDragEnded();

            return false;
        }
    }

    virtual void mouseDraggedAction(int x, int y, int button, int key = -1) {}
    bool mouseDragged(int x, int y, int button, int key = -1)
    {
        if (b_is_pressed)
        {
            for (auto const &value : child_components)
            {
                if (value->mouseDragged(x - this->x, y - this->y, button, key))
                    return b_blocks_mouse;
            }

            mouseDraggedAction(x, y, button, key);

            return b_blocks_mouse;
        }
        else
        {
            return false;
        }
    }

private:
    int id = 0;
    float x, y, width, height;
    float global_x, global_y;

    bool b_setup = false;
    bool b_has_parent = false;
    bool b_has_bg = true;
    bool b_blocks_mouse = true;
    bool b_is_pressed = false;
    bool b_needs_redraw = false;

    ofVec2f drag_start_pos;
    ofColor bg = ofColor(255), fg = ofColor(0);
    ofColor orig_bg = bg, orig_fg = fg;

    Component *parent;
    vector<Component *> child_components;
    ofFbo fbo;

    vector<MovementListener *> movement_listeners;
};

#endif