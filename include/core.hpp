#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

class SDL_Renderer;
class SDL_Surface;
class SDL_Window;

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

    bool running_ = false;
    // SDL_Renderer* renderer_ = nullptr;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Surface* surface_ = nullptr;
};

#endif //GAMEOFLIFE_CORE_HPP