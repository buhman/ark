#pragma once

#include "SDL2/SDL.h"

class Entity {
    protected:
        SDL_Renderer *renderer;

    public:
        int x, y;

    protected:
        int width, height;
        
    public:
        int x_vel = 0, y_vel = 0;
        Entity(SDL_Renderer*, int, int, int, int);

        void update();

        bool overlap(int, int, int);
        bool collision(Entity*);
};

class Paddle: public Entity {
    public:
        Paddle(SDL_Renderer*, int, int, int, int);
        void draw();
};

class Ball: public Entity {
    public:
        Ball(SDL_Renderer*, int, int, int, int);
        void draw();
        void update();
};

class Block: public Entity {
    public:
        Block(SDL_Renderer*);
        void draw();
};
