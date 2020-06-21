#include "SDLUtils.h"
#include <vector>
#include <cstdlib>
#include <ctime>

struct SettingGrid
{
    int n, m;           // field sizes
    int number_of_mines;
    const int square_size = 32;
};

void open(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid);
void openEmpty(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid);

void createGrid(const SettingGrid& setting, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid)
{
    for (int i = 1; i <= setting.n; ++i)
    {
        for (int j = 1; j <= setting.m; ++j)
        {
            sgrid[i][j] = 10;
            grid[i][j] = 0;
        }
    }

    int count = setting.number_of_mines;

    while (count > 0)
    {
        int x = rand() % setting.n + 1;
        int y = rand() % setting.m + 1;

        if (grid[x][y] == 0)
        {
            grid[x][y] = 9;
            --count;
        }
    }

#ifndef NDEBUG
    for (int i = 1; i <= setting.n; ++i)
    {
        for (int j = 1; j <= setting.m; ++j)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
#endif // !NDEBUG

    for (int i = 1; i <= setting.n; ++i)
    {
        for (int j = 1; j <= setting.m; ++j)
        {
            if (grid[i][j] == 9)            continue;
            if (grid[i + 1][j + 1] == 9)    grid[i][j]++;
            if (grid[i][j + 1] == 9)        grid[i][j]++;
            if (grid[i - 1][j + 1] == 9)    grid[i][j]++;
            if (grid[i - 1][j] == 9)        grid[i][j]++;
            if (grid[i - 1][j - 1] == 9)    grid[i][j]++;
            if (grid[i][j - 1] == 9)        grid[i][j]++;
            if (grid[i + 1][j - 1] == 9)    grid[i][j]++;
            if (grid[i + 1][j] == 9)        grid[i][j]++;
        }
    }

#ifndef NDEBUG
    for (int i = 1; i <= setting.n; ++i)
    {
        for (int j = 1; j <= setting.m; ++j)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
#endif // !NDEBUG
}

void openEmpty(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid)
{
    open(x, y + 1, grid, sgrid);
    open(x - 1, y + 1, grid, sgrid);
    open(x - 1, y, grid, sgrid);
    open(x - 1, y - 1, grid, sgrid);
    open(x, y - 1, grid, sgrid);
    open(x + 1, y - 1, grid, sgrid);
    open(x + 1, y, grid, sgrid);
    open(x + 1, y + 1, grid, sgrid);
}

void open(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid)
{
    if (sgrid[x][y] == 10)
    {
        sgrid[x][y] = grid[x][y];
        if (sgrid[x][y] == 0)
        {
            openEmpty(x, y, grid, sgrid);
        }
    }
}

void events(SDL_Event& e, bool& run, int& state_game, std::vector<std::vector<int>>& grid,
    std::vector<std::vector<int>>& sgrid, const SettingGrid& setting)
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            run = false;
        }

        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                state_game = 0;
                createGrid(setting, grid, sgrid);
            }
        }

        if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                int x = e.motion.x / setting.square_size;
                int y = e.motion.y / setting.square_size;

                if (x > 0 && x <= setting.n && y > 0 && y <= setting.m)
                {
                    sgrid[x][y] = grid[x][y];
                    if (sgrid[x][y] == 9)
                    {
                        state_game = -1;
                    }
                    if (sgrid[x][y] == 0)
                    {
                        openEmpty(x, y, grid, sgrid);
                    }
                }
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                int x = e.motion.x / setting.square_size;
                int y = e.motion.y / setting.square_size;

                if (x > 0 && x <= setting.n && y > 0 && y <= setting.m)
                {
                    switch (sgrid[x][y])
                    {
                    case 10:
                        sgrid[x][y] = 11;
                        break;
                    case 11:
                        sgrid[x][y] = 10;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void gameLoop(Manager* manager, std::vector<std::vector<int>>& grid, 
    std::vector<std::vector<int>>& sgrid, const SettingGrid& setting)
{
    Sprite* tiles = new Sprite("images/tiles.bmp", manager);

    SDL_Event e;
    bool run = true;
    int stateGame = 0;     // 0 - active game, 1 win, -1 game over
   // bool game_over = false;
    while (run)
    {
        // ќбработка событий
        events(e, run, stateGame, grid, sgrid, setting);

        // ќтображение сцены
        manager->clear();

        for (int i = 1; i <= setting.n; ++i)
        {
            for (int j = 1; j <= setting.m; ++j)
            {
                if (stateGame == -1)
                {
                    sgrid[i][j] = grid[i][j];
                }
                tiles->setRect(setting.square_size * sgrid[i][j], 0, setting.square_size, setting.square_size);
                tiles->draw(i * setting.square_size, j * setting.square_size);
            }
        }

        manager->display();
    }
}

int main(int argc, char** args)
{
    SettingGrid setting{ 10, 10, 10 };
    std::vector<std::vector<int>> grid(setting.n + 2, std::vector<int>(setting.m + 2, -1));      // насто€щее поле
    std::vector<std::vector<int>> sgrid(setting.n + 2, std::vector<int>(setting.m + 2));        // видима€ часть дл€ игрока

    Manager* manager = new Manager(setting.n, setting.m, setting.square_size);

    srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа

    createGrid(setting, grid, sgrid);
    gameLoop(manager, grid, sgrid, setting);

    return 0;
}