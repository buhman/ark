#include <algorithm>

#include "SDL2/SDL.h"
#include "entity.h"

Entity::Entity(SDL_Renderer *renderer, int x, int y, int width, int height) : 
    renderer(renderer), 
    x(x), y(y), 
    width(width), height(height) {

        x_vel = 0;
        y_vel = 0;
}

void Entity::update() {

    x += x_vel;
    y += y_vel;
}

bool Entity::overlap(int value, int min, int max) {

    return (value >= min) && (value <= max);
}

bool Entity::collision(Entity *e) {
    
    int x_max = std::max(x, x + x_vel);
    int y_max = std::max(y, y + y_vel);
    int x_min = std::min(x, x + x_vel);
    int y_min = std::min(y, y + y_vel);

    int ex_max = std::max(e->x, e->x + x_vel);
    int ey_max = std::max(e->y, e->y + y_vel);
    int ex_min = std::min(e->x, e->x + x_vel);
    int ey_min = std::min(e->y, e->y + y_vel);

    bool x_o = overlap(x_min, ex_min, ex_max + e->width) ||
                     overlap(ex_min, x_min, x_max + width);

    bool y_o = overlap(y_min, ey_min, ey_max + e->height) ||
                     overlap(ey_min, y_min, y_max + height);

    return x_o && y_o;
}

void Entity::draw() {

    SDL_Rect r = {(int)x, (int)y, width, height};
    SDL_RenderDrawRect(renderer, &r);
}

Paddle::Paddle(SDL_Renderer *renderer, int x, int y, int width, int height) :
    Entity::Entity(renderer, x, y, width, height) {
}

void Paddle::draw() {

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    Entity::draw();
}

Ball::Ball(SDL_Renderer *renderer, int x, int y, int width, int height) :
    Entity::Entity(renderer, x, y, width, height) {
}

void Ball::draw() {

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    Entity::draw();
}

void Ball::update() {

    if (x + x_vel <= 0 || x + x_vel + width >= 640) {
        x_vel = -x_vel;
    }
    if (y + y_vel <= 0) {
        y_vel = -y_vel;
    }

    Entity::update();
}

Block::Block(SDL_Renderer *renderer) : 
    Entity::Entity(renderer, rand() % 600 + 20, rand() % 300 + 20, 50, 12) {
}

void Block::draw() {

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    Entity::draw();
}
