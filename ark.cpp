#include <iostream>
#include <algorithm>
#include <list>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ark.h"
#include "entity.h"

Ark::Ark() {

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "ark",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!(font = TTF_OpenFont("freesans.ttf", 16))) {
        std::cout << TTF_GetError() << std::endl;
        running = 0;
    }

    paddle = new Paddle(renderer, 220, 450, 100, 10);

    ball = new Ball(renderer, 220, 240, 8, 8);
    ball->x_vel = 2;
    ball->y_vel = 3;

    for (int i = 0; i < 10; i++) {
        
        block_list.push_front(new Block(renderer));
    }
}

void Ark::quit() {

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Ark::tick() {

    delay = fps_target - (SDL_GetTicks() - start);
    if (delay > 0)
        SDL_Delay((uint32_t)delay);
    start = SDL_GetTicks();
}

void Ark::render() {

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);

    std::for_each(block_list.begin(), block_list.end(), std::mem_fun(&Block::draw));

    paddle->update();
    paddle->draw();
    ball->update();
    ball->draw();

    SDL_Color c = {255,255,255,255};
    SDL_Surface *s;
    SDL_Texture *t;

    char n[21];
    sprintf(n, "score: %d", score);

    s = TTF_RenderText_Blended(font, n, c);

    SDL_Rect SrcR = {0, 0, s->w, s->h};
    SDL_Rect DestR = {
        640 / 2 - s->w / 2,
        480 / 2 - s->h / 2,
        s->w, s->h};

    t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    SDL_RenderCopy(renderer, t, &SrcR, &DestR);
    SDL_DestroyTexture(t);

    SDL_RenderPresent(renderer);
}

void Ark::event_handler() {

    switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.repeat == 0) { // ignore repeated keys
                switch(event.key.keysym.sym) {
                    case SDLK_a:
                        paddle->x_vel = -3;
                        break;
                    case SDLK_d:
                        paddle->x_vel = 3;
                        break;
                    case SDLK_w:
                        paddle->y_vel = -3;
                        break;
                    case SDLK_s:
                        paddle->y_vel = 3;
                }
            }
            break;

        case SDL_KEYUP:
            if (event.key.repeat == 0) { // ignore repeated keys
                switch(event.key.keysym.sym) {
                    case SDLK_a:
                        if (paddle->x_vel < 0)
                            paddle->x_vel = 0;
                        break;
                    case SDLK_d:
                        if (paddle->x_vel > 0)
                            paddle->x_vel = 0;
                        break;
                    case SDLK_w:
                        if (paddle->y_vel < 0)
                            paddle->y_vel = 0;
                        break;
                    case SDLK_s:
                        if (paddle->y_vel > 0)
                            paddle->y_vel = 0;
                        break;
                }
            }
            break;

        case SDL_QUIT:
            running = 0;
            break;
    }
}

void Ark::ball_collide() {

    {
        if (ball->collision(paddle)) {
            
            if (ball->x_vel > ball->y_vel) {
                ball->x_vel = -ball->x_vel;
            }
            else if (ball->x_vel < ball->y_vel) {
                ball->y_vel = -ball->y_vel;
            } 
            else {
                ball->x_vel = -ball->x_vel;
                ball->y_vel = -ball->y_vel;
            }
        }
    }

    {
        std::list<Block*>::iterator i = block_list.begin();

        while (i != block_list.end()) {

            if (ball->collision(*i)) {

                score++;
                delete *i;
                block_list.erase(i++);
                std::cout << "blockc" << std::endl;
            }

            ++i;
        }
    }
}

void Ark::loop() {

    while (running) {

        while (SDL_PollEvent(&event)) {
            event_handler();
        }

        ball_collide();

        render();
        tick();
    }
}

int main(int argc, char* argv[]) {

    Ark game;

    if (game.window == NULL) {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    game.loop();
    game.quit();

    return 0;
}
