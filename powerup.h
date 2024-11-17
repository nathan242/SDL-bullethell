#ifndef POWERUP_H
#define POWERUP_H

#include "engine.h"

class powerup : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;

    public:
        powerup(engine *eng);
        virtual void init();
        ~powerup();
};

#endif
