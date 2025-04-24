#include <core.hpp>
#include "gui/WidgetFunctions.hpp"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "../submodules/ImGuiScope/ImGuiScope.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

Core::Core() 
{
}

bool Core::init_() {
    if (!sdlManager_.isInitialized()) {
        coreAppRunning_ = false;
        return false;
    }

    SDL_GetWindowSize(gui_.mainWindow.sdlWindow, &modelViewport_.w, &modelViewport_.h);

    selectModel_();

    gui_.initialize("Game of Life");

    coreAppRunning_ = true;

    return true;
}

bool Core::run() {
    if(!init_()) return false;
    int lastDisplayUpdate, lastModelUpdate, now;
    now = lastDisplayUpdate = lastModelUpdate = SDL_GetTicks();

    while (coreAppRunning_) {  

        now = SDL_GetTicks();

        processEvents_();
        if (modelRunning_ && now - lastModelUpdate > 1000 / desiredModelFPS_)
        {
            update_();
            measuredModelFPS_ = 1000 / (now - lastModelUpdate);
            lastModelUpdate = now;
        }
        if (now - lastDisplayUpdate > 1000 / displayFPS_)
        {
            render_();
            lastDisplayUpdate = now;
        }
        int waitTime = std::min(1000 / displayFPS_ - (now - lastDisplayUpdate), 1000 / desiredModelFPS_ - (now - lastModelUpdate));
        if (waitTime > 0) SDL_Delay(waitTime);
    }

    return true;
}

void Core::processEvents_() {
    //You will be able to register callbacks for events
    //For example the sidebar, which uses ImGui, will be able to use ImGui_ImplSDL2_ProcessEvent(&event) as a callback.

    //each event handler function will recieve the SDL_Event as well as a global state struct containing e.g. current button states.
    //Or maybe I  can grab the state from SDL directly?
    
    auto timer = ImGuiScope::TimeScope("process events", false);
    SDL_Event event;

    while(SDL_PollEvent(&event)) {

        switch(event.type)
        {
            case SDL_EventType::SDL_EVENT_QUIT:
                coreAppRunning_ = false;
                break;
            case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
            {
                SDL_GetWindowSize(gui_.mainWindow.sdlWindow, &modelViewport_.w, &modelViewport_.h);
                if (cpuModel_) cpuModel_.get()->setViewPort(modelViewport_);
            }
                break;
            case SDL_EventType::SDL_EVENT_KEY_DOWN:
                handleSDL_KEYDOWN(event);
                break;
        }

        if(cpuModel_) cpuModel_.get()->handleSDLEvent(event);

        gui_.mainWindow.processEvent(event);
    }
}

void Core::update_() {
    auto timer = ImGuiScope::TimeScope("update", false);
    if (cpuModel_) cpuModel_.get()->update();
}

void Core::render_() {
    auto timer = ImGuiScope::TimeScope("render", false);
    gui_.mainWindow.clear();

    if (cpuModel_) cpuModel_.get()->draw(gui_.mainWindow.sdlRenderer);

    auto guiDrawTimer = std::make_optional<ImGuiScope::TimeScope>("Draw Gui", false);
    gui_.interface.startDraw(modelRunning_, modelNames_, selectedModel_, desiredModelFPS_, measuredModelFPS_);

    if (selectedModel_ != activeModel_)
    {
        modelRunning_ = false;
        selectModel_();
    }

    if (cpuModel_) cpuModel_.get()->drawImGuiWidgets(modelRunning_);
    ImGuiScope::drawResultsHeader("Timer Results");
    gui_.interface.endDraw(gui_.mainWindow.sdlRenderer);
    guiDrawTimer.reset();

    auto presentTimer = std::make_optional<ImGuiScope::TimeScope>("renderpresent", false);
    gui_.mainWindow.renderPresent();
}

void Core::handleSDL_KEYDOWN(SDL_Event& event) {
    switch(event.key.key)
    {
        case SDLK_ESCAPE:
            coreAppRunning_ = false;
            std::cout << "Escape key pressed: Exiting application." << std::endl;
            break;
        default:
            break;
    }
}

void Core::selectModel_()
{
    switch (selectedModel_)
    {
    case 0: 
        cpuModel_ = std::make_unique<NaiveModel>();
        break;
    case 1:
        cpuModel_ = std::make_unique<LessNaiveModel>();
        break;
    default:
        cpuModel_ = std::make_unique<NaiveModel>();
        break;
    }

    if (cpuModel_) cpuModel_.get()->initialize(modelViewport_);
    activeModel_ = selectedModel_;
}

Core::~Core() {
    //ImGui interface must be deleted before SDL
    gui_.shutdown();
    sdlManager_.shutdown();
}