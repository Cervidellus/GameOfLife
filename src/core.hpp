#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

#include <memory>

//#include "gui\mainwindow.hpp"

#include "gui\interface.hpp"
#include "gui\gui.hpp"
#include "presets\modelpresets.hpp"
#include "sdl_manager.hpp"

enum class ModelPresets::ModelPresetName;
struct SDL_Surface;
class SDL_Texture;
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

    //TODO: I should really have this just passing the parameters, then core has to know less.
    void handleGenerateModelRequest(const ModelParameters& params);
    SDL_Surface* generateModelPresetSurface(const ModelParameters& modelParameters);
    void populateSurfaceFromRLEString(
        SDL_Surface* surface,
        std::string model, 
        int startColumn, 
        int startRow);

    void handleSDL_KEYDOWN(SDL_Event& event);

    bool coreAppRunning_ = false;
    //bool isRunning = false;
    //bool random = true;
    //int modelFPS = -1;
    //int modelWidth = -1;
    //int modelHeight = -1;
    //float fillFactor = 0.2f;
    //int rule1 = 2;
    //int rule3 = 3;
    //int rule4 = 3;
    //int minWidth = 10;
    //int minHeight = 10;
    //std::vector<std::pair <int, int>> aliveCells = {};
    //std::string runLengthEncoding = "";

    //struct ModelParameters {
    //    bool isRunning = false;
    //    bool random = true;
    //    int modelFPS = -1;
    //    int modelWidth = -1;
    //    int modelHeight = -1;
    //    float fillFactor = 0.2f;
    //    int rule1 = 2;
    //    int rule3 = 3;
    //    int rule4 = 3;
    //    int minWidth = 10;
    //    int minHeight = 10;
    //    std::vector<std::pair <int, int>> aliveCells = {};
    //    std::string runLengthEncoding = "";
    //};

    ModelParameters activeModelParams_{
        false, 
        true, 
        60,
        1260,
        720
    };

    //bool modelRunning_ = false;
    const int displayFPS_ = 60;
    //int modelFPS_ = 60;
    int measuredModelFPS_ = 0;

    //int modelWidth_ = 500;
    //int modelHeight_ = 500;
    //float fillFactor_ = 0.2f;
    //int rule1_ = 2; //fewer than 2 neighbors, an alive cell dies
    ////Rule 2 is implied by rules 1 and 3
    //int rule3_ = 3; //more than 3 neighbors, an alive cell dies
    //int rule4_ = 3; //dead cell with 3 neighbors becomes alive
    
   

    SDL_Surface* surface_ = nullptr;
    std::shared_ptr<SDL_Texture> modelTexture_ = nullptr;

    //New stuff
    //SDLManager should be constructed before GUI
    SDLManager sdlManager_;
    GUI gui_;
};

#endif //GAMEOFLIFE_CORE_HPP