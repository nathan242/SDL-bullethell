#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "engine.h"

class game_over : public engine_obj
{
    protected:
        engine *i_eng;

    public:
        game_over(engine *eng);
        void init();
};

#endif