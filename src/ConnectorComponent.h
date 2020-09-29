#ifndef CONNECTOR_COMPONENT_H
#define CONNECTOR_COMPONENT_H

#include "Component.h"
#include "MovementListener.h"

class Connector
{
public:
    Connector(Component *start_component, Component *end_component)
    {
        this->start_component = start_component;
        this->end_component = end_component;
    }

    void draw()
    {
        ofSetLineWidth(4);
        ofSetColor(col);
        ofDrawLine(
            start_component->getGlobalX() + start_component->getWidth() / 2,
            start_component->getGlobalY() + start_component->getHeight() / 2,
            end_component->getGlobalX() + end_component->getWidth() / 2,
            end_component->getGlobalY() + end_component->getHeight() / 2);
        ofSetLineWidth(1);
    }

    bool compare(Component *a, Component *b)
    {
        if (a == start_component && b == end_component)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    ofColor col = ofColor::fromHsb(ofRandom(0, 255), 255, 255);
    Component *start_component, *end_component;
};

//
//=======================================================================
//

class ConnectorComponent : public Component, public MovementListener
{
public:
    ConnectorComponent() : Component(0, 0, ofGetWidth(), ofGetHeight())
    {
        disableBg();
    }

    void addConnector(Component *start_component, Component *end_component)
    {
        int i = 0;
        for (auto connector : connectors)
        {
            if (connector->compare(start_component, end_component) ||
                connector->compare(end_component, start_component))
            {
                ofLog() << "WARNING, connector already exists\n";
                delete connector;
                connectors.erase(connectors.begin() + i);
                repaint();
                return;
            }
            i++;
        }

        start_component->addMovementListener(this);
        end_component->addMovementListener(this);

        connectors.push_back(new Connector(start_component, end_component));
        repaint();
    }

    void moved() override
    {
        repaint();
    }

    void _paint() override
    {
        for (auto c : connectors)
        {
            c->draw();
        }
    }

private:
    vector<Connector *> connectors;
};

#endif