#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Создание рендера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Инициализация SDL2_ttf
    if (TTF_Init() != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL2_ttf: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Загрузка шрифта
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 24);
    if (font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font: %s", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Очистка экрана
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Черный фон
    SDL_RenderClear(renderer);

    // Рендеринг текста
    SDL_Color textColor = {255, 255, 255, 255}; // Белый цвет
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello World!", textColor); 
    if (textSurface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text surface: %s", SDL_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text texture: %s", SDL_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = { (640 - textWidth) / 2, (480 - textHeight) / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Обновление экрана
    SDL_RenderPresent(renderer);

    // Обработка событий
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Очистка ресурсов
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(textTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}