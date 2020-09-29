#ifndef AUDIO_OBJECT_H
#define AUDIO_OBJECT_H

#include "ofMath.h"
#include "ofMathConstants.h"

#define SAMPLE_RATE (44100)

struct AudioParam
{
    AudioParam() {}
    AudioParam(float default_value) { setup(default_value); }
    void setup(float default_value)
    {
        this->default_value = default_value;
        this->value = default_value;
    }

    void setValue(float value) { this->value = value; }
    void setDefaultValue(float value) { this->default_value = value; }

    int setPtr(float **ptr)
    {
        if (*ptr != NULL)
        {
            external_value_ptr = ptr;
            return 1;
        }
        return 0;
    }

    void resetPtr(){ external_value_ptr = &internal_value_ptr; }

    float get() { return **external_value_ptr; }

    float default_value;
    float value;
    float *internal_value_ptr = &value;
    float **external_value_ptr = &internal_value_ptr;

    string name = "none";
};

//====================================================================

class AudioObject
{
public:
    AudioObject() {}
    AudioObject(int n_params, int n_outputs, int id=0) { setup(n_params, n_outputs,id); }
    ~AudioObject()
    {
        delete output;
        delete params;
    }

    void setup(int n_params, int n_outputs, int id=0)
    {
        this->n_params = n_params;
        this->n_outputs = n_outputs;
        this->id = id;

        output = new float[n_outputs];

        params = new AudioParam[n_params];

        b_is_setup = true;
    }

    virtual void audioUpdate() {}

    float getOutputChannel(int c)
    {
        if (c >= 0)
        {
            return output[c % n_outputs];
        }
        else
        {
            return 0.0;
        }
    }
    void setOutputChannel(int c, float v)
    {
        output[c % n_outputs] = v;
    }
    float *getOutput(){ return output; }
    float **getOutputRef(){ return &output; }; 
    int getNumOutputs() { return n_outputs; }

    AudioParam *getParam(int p) { return &params[p % n_params]; }
    AudioParam *getParamByName(string name)
    {
        for(int i=0; i < n_params; i++){
            if(getParam(i)->name == name)
                return getParam(i);
        }
        ofLog() << "ERROR, param name not valid";
        return NULL;
    }
    void setParamName(string name, int p)
    {
        getParam(p)->name = name;
    }

    bool hasAudioOutput(){ return b_audio_output; }
    void setAudioOutput(bool b){ b_audio_output = b; }

    int getId(){ return id; }
    void setId(int id){ this->id = id; }

private:
    int id, n_params, n_outputs;
    float *output;
    AudioParam *params;

    bool b_is_setup = false, b_audio_output = false;
};

#endif