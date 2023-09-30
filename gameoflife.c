#include <stdio.h>

#include <SDL2/SDL.h>

#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CELL_SIZE 16

int grid[SCREEN_WIDTH / CELL_SIZE][SCREEN_HEIGHT / CELL_SIZE] = {0};

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int pause = 0;

void drawGrid() {
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    for (int x = 0; x < SCREEN_WIDTH; x += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
    }
    for (int y = 0; y < SCREEN_HEIGHT; y += CELL_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
}

void updateGrid() {
    if (pause)
        return;

    int newGrid[SCREEN_WIDTH / CELL_SIZE][SCREEN_HEIGHT / CELL_SIZE];
    for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++) {
        for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++) {
            int neighbors = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && ny >= 0 && nx < SCREEN_WIDTH / CELL_SIZE && ny < SCREEN_HEIGHT / CELL_SIZE) {
                        neighbors += grid[nx][ny];
                    }
                }
            }
            if (grid[x][y] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    newGrid[x][y] = 0;
                } else {
                    newGrid[x][y] = 1;
                }
            } else {
                if (neighbors == 3) {
                    newGrid[x][y] = 1;
                } else {
                    newGrid[x][y] = 0;
                }
            }
        }
    }
    for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++) {
        for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++) {
            grid[x][y] = newGrid[x][y];
        }
    }
}

void drawCells() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++) {
        for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++) {
            if (grid[x][y] == 1) {
                SDL_Rect cellRect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x / CELL_SIZE;
            int y = e.button.y / CELL_SIZE;
            if (x >= 0 && y >= 0 && x < SCREEN_WIDTH / CELL_SIZE && y < SCREEN_HEIGHT / CELL_SIZE) {
                grid[x][y] = !grid[x][y];
            }
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                pause = !pause;
            }
        }
    }
}

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    srand(time(NULL));

    // Initialize grid with random cells
    for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++) {
        for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++) {
            grid[x][y] = rand() % 2;
        }
    }

    SDL_bool quit = SDL_FALSE;

    while (!quit) {
        handleInput();
        
        updateGrid();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        drawCells();
        drawGrid();

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
