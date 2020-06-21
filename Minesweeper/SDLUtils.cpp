#include "SDLUtils.h"

int init(SDL_Window*& window, SDL_Renderer*& renderer, const int& width, const int& height, const int& w)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
#ifndef NDEBUG
        std::cout << SDL_GetError << std::endl;
#endif  
        return 1;
    }

    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (width + 2) * w, (height + 2) * w, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
#ifndef NDEBUG
        std::cout << SDL_GetError << std::endl;
#endif
        return 2;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_DestroyWindow(window);
#ifndef NDEBUG
        std::cout << SDL_GetError() << std::endl;
#endif
        return 3;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    return 0;
}

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren)
{
    SDL_Surface* loaded_image = SDL_LoadBMP(file.c_str());
    if (loaded_image != nullptr)
    {
        return SDL_CreateTextureFromSurface(ren, loaded_image);
    }
    return nullptr;
}

Sprite::Sprite(const std::string& path, Manager*& manager)
{
    this->manager = manager;
    texture = loadTexture(path, manager->renderer);
    this->manager->textures.push_back(texture);
    if (texture == nullptr)
    {
#ifndef NDEBUG
        std::cout << SDL_GetError();
#endif
        delete manager;
        exit(1);
    }
    this->dst = new SDL_Rect();
    this->clip = new SDL_Rect();
    clip->x = 0;
    clip->y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &clip->w, &clip->h);
}

Sprite::~Sprite()
{
    delete dst;
    delete clip;
}

void Sprite::setRect(int x, int y, int w, int h)
{
    clip->x = x;
    clip->y = y;
    clip->w = w;
    clip->h = h;
}

void Sprite::draw(int x, int y)
{
    dst->x = x;
    dst->y = y;
    dst->w = clip->w;
    dst->h = clip->h;
    SDL_RenderCopy(manager->renderer, texture, clip, dst);
}

Manager::Manager(int width, int height, int w)
{
    window = nullptr;
    renderer = nullptr;

    if (init(window, renderer, width, height, w) != 0)
    {
#ifndef NDEBUG
        std::cout << "Initialization's error";
#endif  
        exit(1);
    }
}

void Manager::clear()
{
    SDL_RenderClear(renderer);
}

void Manager::display()
{
    SDL_RenderPresent(renderer);
}

Manager::~Manager()
{
    auto end = textures.end();
    for (auto it = textures.begin(); it != end; ++it)
    {
        SDL_DestroyTexture(*it);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

