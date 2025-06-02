#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "engine.h"

class explosion : public engine_obj
{
    protected:
        engine *i_eng;

    public:
        explosion(engine *eng);
        void init();
        ~explosion();
};

#endif
