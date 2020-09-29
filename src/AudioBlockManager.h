#ifndef AUDIO_BLOCK_MANAGER
#define AUDIO_BLOCK_MANAGER

#include "AudioManager.h"
#include "BlockManager.h"
#include "AudioBlock.h"

class AudioBlockManager
{
public:
    AudioBlockManager() {}

    void setup(AudioManager *audio_manager, BlockManager *block_manager)
    {
        this->audio_manager = audio_manager;
        this->block_manager = block_manager;
    }

    int addOscillatorBlock(OscillatorBlock *b)
    {
        b->audio_block_id = (int)audio_blocks.size();
        audio_blocks.push_back(b);
        audio_manager->addAudioObject(b->audio_object);
        block_manager->addBlock(b->block);

        return audio_blocks.size() - 1;
    }

    AudioBlock *getAudioBlockById(int id)
    {
        for (auto ao : audio_blocks)
        {
            if (ao->audio_block_id == id)
                return ao;
        }
        ofLog() << "ERROR, audio block id not valid";
        return NULL;
    }

    void connect(int start_id, int start_node, int end_id, int end_node)
    {
        getAudioBlockById(end_id)->block->getNode(end_node)->setValuePtr(
            getAudioBlockById(start_id)->block->getNode(start_node)->getValuePtr());
    }

    void disconnect(int end_id, int end_node)
    {
        getAudioBlockById(end_id)->block->getNode(end_node)->resetValuePtr();
    }

private:
    AudioManager *audio_manager;
    BlockManager *block_manager;
    vector<AudioBlock *> audio_blocks;
};

#endif