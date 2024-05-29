#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

#include <memory>

#include <interface.hpp>

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;
union SDL_Event;

//class Interface;
//enum Class ModelPreset;

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

    void handleGenerateModelRequest(ModelPreset preset);

    SDL_Surface* generateRandomModelSurface();
    SDL_Surface* generateBlinkerTestSurface();

    void handleSDL_KEYDOWN(SDL_Event& event);

    bool coreAppRunning_ = false;
    bool modelRunning_ = false;
    const int displayFPS_ = 60;
    int modelFPS_ = 60;
    int measuredModelFPS_ = 0;

    int modelWidth_ = 200;
    int modelHeight_ = 200;
    float fillFactor_ = 0.2f;
    int rule1_ = 2; //fewer than 2 neighbors, an alive cell dies
    //Rule 2 is implied by rules 1 and 3
    int rule3_ = 3; //more than 3 neighbors, an alive cell dies
    int rule4_ = 3; //dead cell with 3 neighbors becomes alive
    
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Surface* surface_ = nullptr;

    std::unique_ptr<Interface> interface_;
};

#endif //GAMEOFLIFE_CORE_HPP