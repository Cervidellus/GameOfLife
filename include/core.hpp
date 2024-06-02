#ifndef GAMEOFLIFE_CORE_HPP
#define GAMEOFLIFE_CORE_HPP

#include <memory>

#include <interface.hpp>
#include <modelpresets.hpp>

enum class ModelPresets::ModelPresetName;
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
    bool modelRunning_ = false;
    const int displayFPS_ = 60;
    int modelFPS_ = 60;
    int measuredModelFPS_ = 0;

    int modelWidth_ = 500;
    int modelHeight_ = 500;
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