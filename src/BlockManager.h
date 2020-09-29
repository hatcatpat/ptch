#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include "ComponentManager.h"
#include "Block.h"

class BlockManager
{
public:
    BlockManager(){}

    void setup(ComponentManager *component_manager, ConnectorListener *connector_listener)
    {
        this->component_manager = component_manager;
        this->connector_listener = connector_listener;
    }

    int addBlock(Block *block)
    {
        block->setBlockId((int)blocks.size());
        block->addConnectorListener(connector_listener);
        blocks.push_back(block);
        component_manager->addComponent(block);

        return blocks.size() - 1;
    }

    Block *getBlock(int id)
    {
        for (auto block : blocks)
        {
            if (block->getBlockId() == id)
                return block;
        }
        ofLog() << "ERROR, block id not valid\n";
        return NULL;
    }

private:
    ComponentManager *component_manager;
    ConnectorListener *connector_listener;
    vector<Block *> blocks;
};

#endif