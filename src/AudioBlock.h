#ifndef AUDIO_BLOCK_H
#define AUDIO_BLOCK_H

#include "Block.h"
#include "AudioObject.h"
#include "Oscillator.h"

struct AudioBlock
{
    ~AudioBlock()
    {
        delete audio_object;
        delete block;
    }

    AudioObject *audio_object;
    Block *block;

    int audio_block_id = 0;

    bool b_audio_object_setup = false, b_block_setup = false, b_setup = false;
};

//

class OscillatorBlock : public AudioBlock
{
public:
    OscillatorBlock() {}
    OscillatorBlock(float freq, float lo, float hi, int n_outputs, bool has_audio_output,
                    float x, float y, int id = 0)
    {
        setup(freq, lo, hi, n_outputs, has_audio_output, x, y, id);
    }

    void setupAudioObject(float freq, float lo, float hi, int n_outputs, bool has_audio_output = false)
    {
        audio_object = new Oscillator(freq, lo, hi, n_outputs, has_audio_output);
        b_audio_object_setup = true;
    }

    void setupBlock(float x, float y, int n_outputs, int id = 0)
    {
        block = new Block(x, y, 3, n_outputs, id);

        if (n_outputs > 1)
        {
            block->setNodeName("l", 3);
            block->setNodeName("r", 4);
        }
        else
        {
            block->setNodeName("o", 3);
        }

        b_block_setup = true;
    }

    void setup(float freq, float lo, float hi, int n_outputs, bool has_audio_output,
               float x, float y, int id = 0)
    {
        setupAudioObject(freq, lo, hi, n_outputs, has_audio_output);
        setupBlock(x, y, n_outputs, id);

        for (int i = 0; i < 3; i++)
        {
            block->setNodeName(names[i], i);
            audio_object->setParamName(names[i], i);

            audio_object->getParam(i)->setPtr(block->getInputNode(i)->getValuePtrRef());
        }

        block->getNodeByName("fr")->setDefaultValue(freq);
        block->getNodeByName("lo")->setDefaultValue(lo);
        block->getNodeByName("hi")->setDefaultValue(hi);

        if (!has_audio_output)
        {
            for (int i = 0; i < n_outputs; i++)
            {
                block->getOutputNode(i)->setValuePtr(&audio_object->getOutput()[i]);
            }
        }

        // audio_manager.getAudioObject(0)->getParam(0)->setPtr( audio_manager.getAudioObject(1)->getOutputRef() );

        b_setup = true;
    }

private:
    string names[3] = {"fr", "lo", "hi"};
};

#endif