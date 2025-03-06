#include <core.hpp>
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

    SDL_Rect modelViewport = { 0, 0, 1260, 720 };
    SDL_GetWindowSize(gui_.mainWindow.sdlWindow, &modelViewport.w, &modelViewport.h);
    cpuModel_.initialize(modelViewport);

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
                SDL_Rect modelViewport = { 0, 0, 1260, 720 };
                SDL_GetWindowSize(gui_.mainWindow.sdlWindow, &modelViewport.w, &modelViewport.h);
                cpuModel_.setViewPort(modelViewport);
            }
                break;
            case SDL_EventType::SDL_EVENT_KEY_DOWN:
                handleSDL_KEYDOWN(event);
                break;
        }
        //In the future, I would like an event manager where you can register objects to receive events.
        //When I have an event manager, objects can register for WHICH events they want to receive to make it run a little better. 
        //e.g. so that something not processing a mouse movement event won't have to process it. 

        cpuModel_.handleSDLEvent(event);

        gui_.mainWindow.processEvent(event);
    }
}

void Core::update_() {
    //I might also have a model manager where I can register models, and have the manager call update on all models.
    auto timer = ImGuiScope::TimeScope("update", false);
    cpuModel_.update();
}

void Core::render_() {
    auto timer = ImGuiScope::TimeScope("render", false);
    gui_.mainWindow.clear();//I should have it pass in the color

    cpuModel_.draw(gui_.mainWindow.sdlRenderer);

    auto guiDrawTimer = std::make_optional<ImGuiScope::TimeScope>("Draw Gui", false);
    gui_.interface.startDraw(modelRunning_, desiredModelFPS_, measuredModelFPS_);
    cpuModel_.drawImGuiWidgets(modelRunning_);
    ImGuiScope::drawResultsHeader("Timer Results");
    gui_.interface.endDraw(gui_.mainWindow.sdlRenderer);
    guiDrawTimer.reset();

    auto presentTimer = std::make_optional<ImGuiScope::TimeScope>("renderpresent", false);
    gui_.mainWindow.renderPresent();
    //guiDrawTimer.reset();
}

void Core::handleSDL_KEYDOWN(SDL_Event& event) {
    switch(event.key.key)
    //switch(event.key.keysym.sym)
    {
        case SDLK_ESCAPE:
            coreAppRunning_ = false;
            std::cout << "Escape key pressed: Exiting application." << std::endl;
            break;
        default:
            break;
    }
}

Core::~Core() {
    //ImGui interface must be deleted before SDL
    gui_.shutdown();
    sdlManager_.shutdown();
}