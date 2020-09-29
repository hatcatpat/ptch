#ifndef CONNECTOR_LISTENER_H
#define CONNECTOR_LISTENER_H

struct ConnectorListener
{
    virtual void nodePressed(int block, int node)
    {
        if (!b_node_currently_selected)
        {
            current_node[0] = block;
            current_node[1] = node;
            b_node_currently_selected = true;
        }
        else if (current_node[0] == block)
        {
            nodeConnectionFailed(block, node);
            b_node_currently_selected = false;
            current_node[0] = -1;
            current_node[1] = -1;
            new_node[0] = -1;
            new_node[1] = -1;
        }
        else
        {
            new_node[0] = block;
            new_node[1] = node;
            b_node_currently_selected = false;
            nodeConnectionMade();
        }
    }

    virtual void nodeConnectionMade() {}
    virtual void nodeConnectionFailed(int,int) {}

    bool b_node_currently_selected = false;
    int current_node[2] = {-1, -1};
    int new_node[2] = {-1, -1};
};

#endif