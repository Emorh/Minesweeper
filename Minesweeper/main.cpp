#include "SDLUtils.h"
#include <vector>
#include <cstdlib>
#include <ctime>

void open(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid);
void openEmpty(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid);

void createGrid(const int& n, const int& m, const int& k,
    std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid)
{
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            sgrid[i][j] = 10;
            grid[i][j] = 0;
        }
    }

    int count = k;

    while (count > 0)
    {
        int x = rand() % n + 1;
        int y = rand() % m + 1;

        if (grid[x][y] == 0)
        {
            grid[x][y] = 9;
            --count;
        }
    }

#ifndef NDEBUG
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
#endif // !NDEBUG

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
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
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
#endif // !NDEBUG
}

void openEmpty(const int& x, const int& y, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid)
{
    open(x - 1, y, grid, sgrid);
    open(x, y + 1, grid, sgrid);
    open(x + 1, y, grid, sgrid);
    open(x, y - 1, grid, sgrid);
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

void events(SDL_Event& e, bool& run, bool& game_over, std::vector<std::vector<int>>& grid,
    std::vector<std::vector<int>>& sgrid, const int& n, const int& m, const int& k, const int& w)
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            run = false;
        }

        if (e.type == SDL_KEYDOWN)
        {
            game_over = false;
            createGrid(n, m, k, grid, sgrid);
        }

        if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                int x = e.motion.x / w;
                int y = e.motion.y / w;
#ifndef NDEBUG
                std::cout << "X: " << x << std::endl;
                std::cout << "Y: " << y << std::endl;
#endif // !NDEBUG
                if (x > 0 && x <= n && y > 0 && y <= m)
                {
                    sgrid[x][y] = grid[x][y];
                    if (sgrid[x][y] == 9)
                    {
                        game_over = true;
                    }
                    if (sgrid[x][y] == 0)
                    {
                        openEmpty(x, y, grid, sgrid);
                    }
                }
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                int x = e.motion.x / w;
                int y = e.motion.y / w;
#ifndef NDEBUG
                std::cout << "X: " << x << std::endl;
                std::cout << "Y: " << y << std::endl;
#endif // !NDEBUG
                if (x > 0 && x <= n && y > 0 && y <= m)
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

void gameLoop(Manager* manager, std::vector<std::vector<int>>& grid, std::vector<std::vector<int>>& sgrid,
    const int& n, const int& m, const int& k, const int& w)
{
    Sprite* background = new Sprite("images/background.jpg", manager);
    Sprite* tiles = new Sprite("images/tiles.jpg", manager);

    SDL_Event e;
    bool run = true;
    bool game_over = false;
    while (run)
    {
        // Обработка событий
        events(e, run, game_over, grid, sgrid, n, m, k, w);

        // Отображение сцены
        manager->clear();
        background->draw(0, 0);

        for (int i = 1; i <= n; ++i)
        {
            for (int j = 1; j <= m; ++j)
            {
                if (game_over)
                {
                    sgrid[i][j] = grid[i][j];
                }
                tiles->setRect(w * sgrid[i][j], 0, w, w);
                tiles->draw(i * w, j * w);
            }
        }

        manager->display();
    }
}

int main(int argc, char** args)
{
    int n = 10, m = 10;   // размеры поля
    int k = 10;           // количество мин
    const int w = 32;     // размер одного квадрата
    std::vector<std::vector<int>> grid(n + 2, std::vector<int>(m + 2, -1));      // настоящее поле
    std::vector<std::vector<int>> sgrid(n + 2, std::vector<int>(m + 2));        // видимая часть для игрока

    Manager* manager = new Manager(n, m, w);

    srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа

    createGrid(n, m, k, grid, sgrid);
    gameLoop(manager, grid, sgrid, n, m, k, w);

    return 0;
}