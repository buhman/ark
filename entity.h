#pragma once

#include "SDL.h"

class Entity {
    protected:
        SDL_Renderer *renderer;

    public:
        float x, y;

    protected:
        int width, height;
        
    public:
        float x_vel, y_vel;
        Entity(SDL_Renderer*, int, int, int, int);

        void update();
        void draw();

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
