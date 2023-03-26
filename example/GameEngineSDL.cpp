#include "GameEngineSDL.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <string>

constexpr uint32_t NUM_TILE_W = 25u;
constexpr uint32_t NUM_TILE_H = 20u;
constexpr uint32_t TILE_W = 32u;
constexpr uint32_t TILE_H = 32u;
constexpr uint32_t WINDOW_W = NUM_TILE_W * TILE_W;
constexpr uint32_t WINDOW_H = NUM_TILE_H * TILE_H;
auto time_init = std::chrono::high_resolution_clock::now();

GameEngineSDL::GameEngineSDL() :fps(0.0f), running(true), pStart{-1,-1}
{
    SDL_Init(SDL_INIT_EVERYTHING);
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) <= 0)
    {
        this->running = false;
    }
    if (TTF_Init() < 0)
    {
        this->running = false;
    }
}

GameEngineSDL::~GameEngineSDL()
{
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

double GameEngineSDL::getTicks()
{
    auto time_current = std::chrono::high_resolution_clock::now();
    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(time_current - time_init);
    return time_diff.count();
}

void GameEngineSDL::configure()
{
    this->Init(WINDOW_W, WINDOW_H, SDL_WindowFlags::SDL_WINDOW_SHOWN);
    //Set the swap interval for the current OpenGL context. 
    SDL_GL_SetSwapInterval(1); //(0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync)
    //SDL_RenderSetLogicalSize(this->ren, WINDOW_W/2, WINDOW_H/2);   // Set renderer height and width
    raycastMap.setMapDimension(WINDOW_W, WINDOW_H);
    raycastMap.setTileDimension(TILE_W, TILE_H);
}

void GameEngineSDL::input()
{
    SDL_Event event;
    uint32_t x, y;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        {
            x = event.button.x / TILE_W;
            y = event.button.y / TILE_W;
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                this->raycastMap.toogleMatrix(x, y);
            }
            else
            {
                if (this->pStart.x == x && this->pStart.y == y)
                {
                    this->pStart.x = this->pStart.y = -1;
                }
                else
                {
                    this->pStart.x = x;
                    this->pStart.y = y;
                }
            }
        }
        break;
        case SDL_EventType::SDL_MOUSEMOTION:
            if (this->pStart.x != -1)
            {
                //Normalize vector pStart - event.motion
                this->pfRayDir.x = (float)(this->pStart.x * TILE_W) - event.motion.x;
                this->pfRayDir.y = (float)(this->pStart.y * TILE_H) - event.motion.y;
                float mag = std::sqrtf((this->pfRayDir.x * this->pfRayDir.x) + (this->pfRayDir.y * this->pfRayDir.y));
                float r = 1.0f / mag;
                this->pfRayDir.x *= -r;
                this->pfRayDir.y *= -r;

               // printf("%f %f\n", pfRayDir.x, pfRayDir.y);
                pDir.x = event.motion.x / TILE_W;
                pDir.y = event.motion.y / TILE_H;
            }
            break;
        case SDL_EventType::SDL_QUIT:
            this->running = false;
            break;
        }
    }
}

void GameEngineSDL::update()
{
    SDL_FPoint pfRayCast;
    pRayCastCollision = { -1, -1 };
    if (this->pStart.x != -1)
    {
        //pStart is a point normalized respect the grid (stay always in the corner up-left)
        if(this->raycastMap.raycast(this->pStart, this->pfRayDir, 100.0f, pfRayCast))
        {
            //decode
            pRayCastCollision.x = floor(pfRayCast.x * TILE_W);
            pRayCastCollision.y = floor(pfRayCast.y * TILE_H);
        }
    }
}

void GameEngineSDL::drawGrid()
{
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(this->ren, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(this->ren, 0u, 0u, 255u, 255u);
    for (uint32_t x = 0; x < WINDOW_W; x += TILE_W)
    {
        SDL_RenderDrawLine(this->ren, x, 0u, x, WINDOW_H);
    }
    for (uint32_t y = 0; y < WINDOW_H; y += TILE_H)
    {
        SDL_RenderDrawLine(this->ren, 0u, y, WINDOW_W, y);
    }
    SDL_SetRenderDrawColor(this->ren, r, g, b, a);
}

void GameEngineSDL::drawItems()
{
    uint8_t r, g, b, a;
    SDL_Rect rect = { 0u, 0u, 0u, 0u };
    SDL_GetRenderDrawColor(this->ren, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(this->ren, 0u, 0u, 255u, 255u);

    for (uint32_t x = 0u; x < NUM_TILE_W; x++)
    {
        for (uint32_t y = 0u; y < NUM_TILE_H; y++)
        {
            if (this->raycastMap.getItem(x,y))
            {
                rect.x = x * TILE_W;
                rect.y = y * TILE_H;
                rect.w = TILE_W;
                rect.h = TILE_H;
                SDL_RenderFillRect(this->ren, &rect);
            }
        }
    }
    SDL_SetRenderDrawColor(this->ren, 255u, 0u, 0u, 255u);
    if (this->pStart.x != -1)
    {
        rect.x = this->pStart.x * TILE_W;
        rect.y = this->pStart.y * TILE_H;
        rect.w = TILE_W;
        rect.h = TILE_H;
        SDL_RenderFillRect(this->ren, &rect);
        SDL_SetRenderDrawColor(this->ren, 255u, 255u, 0u, 255u);
        rect.x = this->pDir.x * TILE_W;
        rect.y = this->pDir.y * TILE_H;
        SDL_RenderFillRect(this->ren, &rect);
    }

    SDL_SetRenderDrawColor(this->ren, r, g, b, a);
}

void GameEngineSDL::drawCollision()
{
    uint8_t r, g, b, a;
    SDL_Rect rect = { pRayCastCollision.x - 5, pRayCastCollision.y - 5, 10, 10 };
    SDL_GetRenderDrawColor(this->ren, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(this->ren, 0, 255, 0, 255);
    if (pRayCastCollision.x != -1)
    {
        SDL_RenderDrawRect(this->ren, &rect);
    }
    SDL_SetRenderDrawColor(this->ren, r, g, b, a);
}

void GameEngineSDL::draw()
{
    SDL_RenderClear(this->ren);
    
    drawGrid();
    drawItems();
    drawCollision();

    SDL_RenderPresent(this->ren);
}

void GameEngineSDL::execute()
{
    std::string fpsString;
    while (this->running)
    {
        double time_start = this->getTicks();
        
        this->input();
        this->update();
        this->draw();

        this->fps = 1.0 / ((this->getTicks() - time_start) / 1000.0);
        fpsString = std::to_string(this->fps);
        SDL_SetWindowTitle(this->win, fpsString.c_str());
    }
}