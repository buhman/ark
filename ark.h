#pragma once

#include <list>

#include <SDL2/SDL.h>
#include "entity.h"

class Ark {
    private:
        SDL_Renderer *renderer;
        SDL_Event event;

        bool running = true;

        float smooth_fps = 60.f;
        float fps_target = 1000.0f/60;
        int start = 0;
        int delay;

        Paddle *paddle;
        Ball *ball;

        TTF_Font *font;

        std::list<Block*> block_list;

        int score = 0;
        int lives = 3;

        int level_size = 10;

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
