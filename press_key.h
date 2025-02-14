#ifndef PRESS_KEY_H
#define PRESS_KEY_H

#include "engine.h"

class press_key : public engine_obj
{
    protected:
        engine *i_eng;

    public:
        press_key(engine *eng);
        void init();
};

#endif