#ifndef MENU_TITLE_H
#define MENU_TITLE_H

#include "engine.h"

class menu_title : public engine_obj
{
    protected:
        engine *i_eng;

    public:
        menu_title(engine *eng);
        void init();
};

#endif