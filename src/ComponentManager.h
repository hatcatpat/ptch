#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "Component.h"
#include "ConnectorComponent.h"

class ComponentManager
{
public:
    ComponentManager()
    {
        setup();
    }

    void setup()
    {
    }

    int addComponent(Component *component)
    {
        component->setComponentId((int)components.size());
        components.push_back(component);

        return components.size() - 1;
    }

    Component *getComponent(int id)
    {
        for (auto component : components)
        {
            if (component->getComponentId() == id)
                return component;
        }
        ofLog() << "ERROR, component id not valid\n";
        return NULL;
    }

    ConnectorComponent *getConnectorComponent() { return &connector_component; }

    void draw()
    {
        for (auto component : components)
        {
            ofPushStyle();
            component->draw();
            ofPopStyle();
        }

        connector_component.draw();
    }

    bool needsRedraw()
    {
        for (auto component : components)
        {
            if (component->needsRedraw())
                return true;
        }
        return false;
    }

    void update()
    {
        for (auto component : components)
        {
            component->update();
        }
    }

    //================================================================================

    void mouseMoved(int x, int y, int currently_held_key)
    {
        for (auto component : components)
        {
            component->mouseMoved(x, y, currently_held_key);
        }
    }

    void mouseDragged(int x, int y, int button, int currently_held_key)
    {
        for (auto component : components)
        {
            component->mouseDragged(x, y, button, currently_held_key);
            if (component->isPressed())
                currently_dragging_component = component;
        }
    }

    void mousePressed(int x, int y, int button, int currently_held_key)
    {
        for (auto component : components)
        {
            component->mousePressed(x, y, button, currently_held_key);
        }
    }

    void mouseReleased(int x, int y, int button, int currently_held_key)
    {
        for (auto component : components)
        {
            component->mouseReleased(x, y, button, currently_held_key);
        }
    }

private:
    vector<Component *> components;
    ConnectorComponent connector_component;
    Component *currently_dragging_component;
};

#endif