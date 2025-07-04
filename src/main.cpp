// -*- coding: utf-8 -*-
#include "includes.cpp"

// ������� ��� ���������� ������� �����
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

    // ������� ����
    SDL_Window* window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // ������� ��������
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC): " << SDL_GetError() << std::endl;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC); // Fallback
        if (!renderer) {
            std::cerr << "Failed to create renderer(SDL_RENDERER_SOFTWARE): " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    }


    sell::area ar;

    std::cout << "Hello1\n" << "L1 cache" << GetL1CacheSize() << "\n";

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


        // ������� �����
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // ����� ���
        SDL_RenderClear(renderer);

        // ������ �������������
        const SDL_Rect rect = {200, 100, 300, 150}; // x, y, ������, ������
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // ����� ����
        SDL_RenderFillRect(renderer, &rect);

        // ������ �������
        const SDL_Rect square = {400, 250, 100, 100};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // ������� ����
        SDL_RenderFillRect(renderer, &square);

        // ���������� ��� ���������
        SDL_RenderPresent(renderer);
    }


    // ����������� �������
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
















