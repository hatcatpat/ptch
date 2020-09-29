#ifndef BLOCK_H
#define BLOCK_H

#include "ConnectorListener.h"
#include "Component.h"
#include "Node.h"

class Block : public Component
{
public:
    Block() {}
    Block(float x, float y, int n_inputs, int n_outputs, int id = 0)
    {
        setup(x, y, n_inputs, n_outputs, id);
    }
    ~Block()
    {
        delete input_nodes;
        delete output_nodes;
    }

    void setup(float x, float y, int n_inputs, int n_outputs, int id = 0)
    {
        Component::setup(x, y, max(n_inputs, n_outputs) * node_sz, node_sz * 3, id);

        setBg(ofColor(100));

        this->id = id;
        this->n_inputs = n_inputs;
        this->n_outputs = n_outputs;

        input_nodes = new Node[n_inputs];
        output_nodes = new Node[n_outputs];

        for (int i = 0; i < n_inputs + n_outputs; i++)
        {
            if (i < n_inputs)
            {
                input_nodes[i].setup(i * node_sz, 0, node_sz, id, i);
                add(&input_nodes[i]);
            }
            else
            {
                output_nodes[i - n_inputs].setup((i - n_inputs) * node_sz, getHeight() - node_sz, node_sz, id, i);
                add(&output_nodes[i - n_inputs]);
            }
        }
    }

    void setBlockId(int id)
    {
        this->id = id;

        for (int i = 0; i < n_inputs + n_outputs; i++)
        {
            if (i < n_inputs)
            {
                input_nodes[i].setBlockId(id);
            }
            else
            {
                output_nodes[i - n_inputs].setBlockId(id);
            }
        }
    }
    int getBlockId() { return id; }

    Node *getNode(int id)
    {
        for (int i = 0; i < n_inputs + n_outputs; i++)
        {
            if (i < n_inputs)
            {
                if (input_nodes[i].getComponentId() == id)
                    return &input_nodes[i];
            }
            else
            {
                if (output_nodes[i - n_inputs].getComponentId() == id)
                    return &output_nodes[i - n_inputs];
            }
        }
        ofLog() << "ERROR, node id not valid";
        return NULL;
    }

    Node *getOutputNode(int pos) { return getNode((pos + n_inputs) % (n_inputs + n_outputs)); }
    Node *getInputNode(int pos) { return getNode(pos % n_inputs); }

    Node *getNodeByName(string name)
    {
        for (int i = 0; i < n_inputs + n_outputs; i++)
        {
            if (i < n_inputs)
            {
                if (input_nodes[i].getName() == name)
                    return &input_nodes[i];
            }
            else
            {
                if (output_nodes[i - n_inputs].getName() == name)
                    return &output_nodes[i - n_inputs];
            }
        }
        ofLog() << "ERROR, node name not valid";
        return NULL;
    }
    void setNodeName(string name, int id)
    {
        getNode(id)->setName(name);
    }

    void mouseDraggedAction(int x, int y, int button, int key = -1) override
    {
        setBg(ofColor(0, 0, 255), false);

        setRelativePosition(
            x - getDragStartPos().x - (getGlobalX() - getX()),
            y - getDragStartPos().y - (getGlobalY() - getY()));

        repaint();
    }

    void mouseReleasedAction(int x, int y, int button, int key = -1) override
    {
        resetBg();
        repaint();
    }

    void addConnectorListener(ConnectorListener *c)
    {
        for (int i = 0; i < n_inputs + n_outputs; i++)
        {
            if (i < n_inputs)
            {
                input_nodes[i].addConnectorListener(c);
            }
            else
            {
                output_nodes[i - n_inputs].addConnectorListener(c);
            }
        }
    }

private:
    float node_sz = 50;
    int id = 0;
    int n_inputs, n_outputs;
    Node *input_nodes, *output_nodes;
};

#endif