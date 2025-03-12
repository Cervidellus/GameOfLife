#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

#include "gui\gui.hpp"
#include "model\NaiveModel.hpp"
#include "model\LessNaiveModel.hpp"
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
    //When the GUI makes a change, change model to the selected index
    void selectModel_();

    void handleSDL_KEYDOWN(SDL_Event& event);

    bool coreAppRunning_ = false;
    bool modelRunning_ = false;

    ModelParameters activeModelParams_{
        true, 
        1260,
        720
    };

    SDL_Rect modelViewport_ = { 0, 0, 1260, 720 };

    const int displayFPS_ = 60;
    int desiredModelFPS_ = 60;
    int measuredModelFPS_ = 0;

    SDLManager sdlManager_;
    GUI gui_;
    std::unique_ptr<AbstractModel> cpuModel_;
    
    //AbstractModel cpuModel_ = NaiveModel();
    //NaiveModel cpuModel_;

    enum class AvailableModels
    {
        Naive = 0, LessNaive = 1
    };

    //Model currently selected in the GUI
    int selectedModel_ = 0;
    //Model currently running
    int activeModel_ = 0;

    constexpr static const char* modelNames_[] = {"Naive", "Less Naive"};
};

#endif //GAMEOFLIFE_CORE_HPP