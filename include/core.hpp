#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

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

    SDL_Surface* generateModelSurface(int width, int height, float fillFactor);
    SDL_Surface* generateBlinkerTestSurface();

    void handleSDL_KEYDOWN(SDL_Event& event);

    bool coreAppRunning_ = false;
    bool modelRunning_ = false;
    const int displayFPS_ = 60;
    int modelFPS_ = 60;
    int measuredModelFPS_ = 0;

    int matrixWidth_ = 200;
    int matrixHeight_ = 200;
    float fillFactor_ = 0.2f;
    
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Surface* surface_ = nullptr;
};

#endif //GAMEOFLIFE_CORE_HPP