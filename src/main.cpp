#include "includes.cpp"


// Функция для уменьшения яркости цвета
SDL_Color decrease_brightness(const SDL_Color& color, const float factor) {
    SDL_Color result;
    result.r = (uint8_t)(color.r * factor);
    result.g = (uint8_t)(color.g * factor);
    result.b = (uint8_t)(color.b * factor);
    result.a = (uint8_t)(color.a);
    return result;
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Создаем окно
    SDL_Window* window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Создаем рендерер
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    sell::area ar;
    std::cout << "Hello1\n";

    bool running = true;
    SDL_Event event;
    while (running) {
        //ar.next_step();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }


        // Очищаем экран
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый фон
        SDL_RenderClear(renderer);

        // Рисуем прямоугольник
        const SDL_Rect rect = {200, 100, 300, 150}; // x, y, ширина, высота
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Синий цвет
        SDL_RenderFillRect(renderer, &rect);

        // Рисуем квадрат
        const SDL_Rect square = {400, 250, 100, 100};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный цвет
        SDL_RenderFillRect(renderer, &square);

        // Отображаем все изменения
        SDL_RenderPresent(renderer);
    }


    // Освобождаем ресурсы
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
















