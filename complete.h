#ifndef COMPLETE_H
#define COMPLETE_H

#include "engine.h"

class complete : public engine_obj
{
    protected:
        engine *i_eng;

    public:
        complete(engine *eng);
        void init();
};

#endif