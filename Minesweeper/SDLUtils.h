#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <list>

#ifndef NDEBUG
#include <iostream>
#endif

class Manager
{
    std::list<SDL_Texture*> textures;
    Manager();

public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    Manager(int width, int height, int w);
    ~Manager();

    void freeResources();

    void clear();
    void display();

    friend class Sprite;
};

class Sprite
{
    SDL_Texture* texture;
    Manager* manager;
    SDL_Rect* clip;
    SDL_Rect* dst;

    Sprite();
public:
    Sprite(const std::string& path, Manager*& manager);
    ~Sprite();

    void setRect(int x, int y, int w, int h);
    void draw(int x, int y);

};

int init(SDL_Window*& window, SDL_Renderer*& renderer);

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren);

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, const int& x, const int& y,
    SDL_Rect* clip = nullptr);