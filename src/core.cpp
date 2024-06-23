#include <core.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

Core::Core() 
{
}

bool Core::run() {
    if(!init_()) return false;
    int lastDisplayUpdate, lastModelUpdate, now;
    now = lastDisplayUpdate = lastModelUpdate = SDL_GetTicks();

    while (coreAppRunning_) {  
        now = SDL_GetTicks();

        processEvents_();
        if (activeModelParams_.isRunning && now - lastModelUpdate > 1000 / activeModelParams_.modelFPS)
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
        //wait function so we don't burn too much cpu
        int waitTime = std::min(1000 / displayFPS_ - (now - lastDisplayUpdate), 1000 / activeModelParams_.modelFPS - (now - lastModelUpdate));
        if (waitTime > 0) SDL_Delay(waitTime);
    }

    return true;
}

bool Core::init_() {
    if (!sdlManager_.isInitialized()) {
		coreAppRunning_ = false;
		return false;
	}

    cpuModel_.initialize();

    gui_.initialize(
		"Game of Life",
		[&](ModelParameters params) {}
	);

    coreAppRunning_ = true;
    return true;
}

void Core::processEvents_() {
    //Eventually I will have a SDL_Event handler
    //You will be able to register callbacks for events
    //For example the sidebar, which uses ImGui, will be able to use ImGui_ImplSDL2_ProcessEvent(&event) as a callback.
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type)
        {
            case SDL_QUIT:
                coreAppRunning_ = false;
                break;
            case SDL_KEYDOWN:
                handleSDL_KEYDOWN(event);
                break;
        }
        //In the future, I would like an event manager where you can register objects to receive events.
        gui_.mainWindow.processEvent(event);
    }
}

void Core::update_() {
    //I might also have a model manager where I can register models, and have the manager call update on all models.
    cpuModel_.update();
}

void Core::render_() {
    
    gui_.mainWindow.clear();

    int windowWidth, windowHeight;
    SDL_GetWindowSize(gui_.mainWindow.sdlWindow, &windowWidth, &windowHeight);
    cpuModel_.draw(gui_.mainWindow.sdlRenderer, 0, 0, windowWidth, windowHeight);

    //I should have the models provide their interface
    gui_.interface.draw(gui_.mainWindow.sdlRenderer, activeModelParams_, measuredModelFPS_);
    gui_.mainWindow.renderPresent();
}

void Core::handleSDL_KEYDOWN(SDL_Event& event) {
    switch(event.key.keysym.sym)
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
    sdlManager_.shutdown();
}