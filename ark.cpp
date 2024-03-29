#include <iostream>
#include <algorithm>
#include <functional>
#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ark.h"
#include "entity.h"

Ark::Ark() {

    running = true;
    score = 0;
    lives = 3;
    level_size = 10;

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

    ball = new Ball(renderer, 200, 200, 8, 8);
    ball->x_vel = 2;
    ball->y_vel = 2;

    smooth_fps = fps_target;
    
    new_level();
}

void Ark::new_level() {

    for (int i = 0; i < level_size; i++) {
        
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

    smooth_fps = smooth_fps * 0.9 +
        (1000.f / (SDL_GetTicks() - start)) * 0.1;

    start = SDL_GetTicks();
}

void Ark::render() {

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);

    using namespace std::placeholders;

    std::for_each(block_list.begin(), block_list.end(),
		  std::bind(&Block::draw, _1));

    paddle->update();
    paddle->draw();
    ball->update();
    ball->draw();

    {
        char n[64];

        snprintf(n, (sizeof (n)), "score: %d", score);
        hud(n, 5, 0);

        n[0] = 0;
        snprintf(n, (sizeof (n)), "lives: %d", lives);
        hud(n, 5, 16);

        n[0] = 0;
        snprintf(n, (sizeof (n)), "fps: %.1f", smooth_fps);
        hud(n, 5, 32);
    }

    SDL_RenderPresent(renderer);
}

void Ark::hud(char* text, int x, int y) {

    SDL_Color c = {255,255,255,255};
    SDL_Surface *s;
    SDL_Texture *t;

    s = TTF_RenderText_Blended(font, text, c);

    SDL_Rect SrcR = {0, 0, s->w, s->h};
    SDL_Rect DestR = {x, y, s->w, s->h};

    t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    SDL_RenderCopy(renderer, t, &SrcR, &DestR);
    SDL_DestroyTexture(t);
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
 
            ball->y_vel = -ball->y_vel;

            ball->x_vel = ball->x_vel + (0.5 * paddle->x_vel);
        }
    }

    {
        if (ball->y + ball->y_vel > 480) {

            lives--;

            ball->y = 200;
            ball->x = 200;
            ball->y_vel = 2;
            ball->x_vel = 2;
        }
    }

    {
        std::list<Block*>::iterator i = block_list.begin();

        while (i != block_list.end()) {

            if (ball->collision(*i)) {

                ball->y_vel = -ball->y_vel;

                score++;
                delete *i;
                block_list.erase(i++);

                if (block_list.size() == 0) {

                    level_size++;
                    new_level();
                }
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

int main() {

    Ark game;

    if (game.window == NULL) {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    game.loop();
    game.quit();

    return 0;
}
