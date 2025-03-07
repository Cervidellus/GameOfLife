#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

#include "gui\gui.hpp"
#include "model\CpuModel.hpp"
#include "model\PrefixSumModel.hpp"
#include "sdl_manager.hpp"

union SDL_Event;

class Core {
public:
    Core();
    ~Core();
    bool run();

private:
    bool init_();
    void processEvents_();
    void update_();
    void render_();

    void handleSDL_KEYDOWN(SDL_Event& event);

    bool coreAppRunning_ = false;
    bool modelRunning_ = false;

    ModelParameters activeModelParams_{
        true, 
        1260,
        720
    };

    const int displayFPS_ = 60;
    int desiredModelFPS_ = 60;
    int measuredModelFPS_ = 0;

    SDLManager sdlManager_;
    GUI gui_;
    //PrefixSumModel cpuModel_;
    CpuModel cpuModel_;
};

#endif //GAMEOFLIFE_CORE_HPP