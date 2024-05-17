#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

// #include <stdint.h>

// struct ImGuiContext;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;
union SDL_Event;

class Core {
public:
    Core();
    ~Core();
    bool run();

private:
    bool init();
    void processEvents();
    void update();
    void render();

    void handleSDL_KEYDOWN(SDL_Event& event);

    bool running_ = false;
    int desiredFPS_ = 60;
    int measuredFPS_ = 0;
    
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Surface* surface_ = nullptr;
    SDL_Texture* texture_ = nullptr;
    // ImGuiContext* imguiContext_ = nullptr;
};

#endif //GAMEOFLIFE_CORE_HPP