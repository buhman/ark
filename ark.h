#pragma once

#include <list>

#include <SDL.h>
#include "entity.h"

class Ark {
    private:
        SDL_Renderer *renderer;
        SDL_Event event;

        bool running ;

        float smooth_fps;
        float fps_target;
        int start;
        int delay;

        Paddle *paddle;
        Ball *ball;

        //TTF_Font *font;

        std::list<Block*> block_list;

        int score;
        int lives;

        int level_size;

    public:
        SDL_Window *window;

        Ark();
        void quit();

        void tick();
        void render();

        void hud(char*, int, int);

        void ball_collide();

        void event_handler();
        void loop();

        void new_level();
};
