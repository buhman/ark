#include <algorithm>

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "entity.h"

Entity::Entity(SDL_Renderer *renderer, int x, int y, int width, int height) : 
    renderer(renderer), 
    x(x), y(y), 
    width(width), height(height) {
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

    bool x_o = overlap(x_min, e->x, e->x + e->width) ||
                     overlap(e->x, x_min, x_max + width);

    bool y_o = overlap(y_min, e->y, e->y + e->height) ||
                     overlap(e->y, y_min, y_max + height);

    return x_o && y_o;
}

Paddle::Paddle(SDL_Renderer *renderer, int x, int y, int width, int height) :
    Entity::Entity(renderer, x, y, width, height) {
}

void Paddle::draw() {

    rectangleRGBA(renderer, x, y, x+width, y+height, 0, 0, 255, 255);
}

Ball::Ball(SDL_Renderer *renderer, int x, int y, int width, int height) :
    Entity::Entity(renderer, x, y, width, height) {
}

void Ball::draw() {

    aacircleRGBA(renderer, x, y, width, 255, 255, 0, 255);
}

void Ball::update() {

    if (x + x_vel <= 0 || x + x_vel + width >= 640) {
        x_vel = -x_vel;
    }
    if (y + y_vel <= 0 || y + y_vel + height >= 480) {
        y_vel = -y_vel;
    }

    Entity::update();
}

Block::Block(SDL_Renderer *renderer) : 
    Entity::Entity(renderer, rand() % 300 + 20, rand() % 440 + 20, 50, 12) {
}

void Block::draw() {

    rectangleRGBA(renderer, x, y, x+width, y+height, 0, 255, 0, 255);
}
