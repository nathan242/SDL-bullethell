#ifndef PAUSED_IMG_H
#define PAUSED_IMG_H

#include "engine.h"

class paused_img : public engine_obj
{
    protected:
        engine *i_eng;

    public:
        paused_img(engine *eng);
        void init();
};

#endif