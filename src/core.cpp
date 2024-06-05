#include <core.hpp>
#include <ui\mainwindow.hpp>
#include <ui\interface.hpp>
#include "ui\ui.hpp"
#include <model\modelparameters.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

Core::Core() :
    sdlManager_()/*,
    ui_()*/

{
    //interface_ = std::make_unique<Interface>();//initializes for second time!
}

bool Core::run() {
    if(!init_()) return false;
    int lastDisplayUpdate, lastModelUpdate, now;
    now = lastDisplayUpdate = lastModelUpdate = SDL_GetTicks();

    while (coreAppRunning_) {  
        now = SDL_GetTicks();

        processEvents_();
        if (modelRunning_ && now - lastModelUpdate > 1000 / modelFPS_)
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
        int waitTime = std::min(1000 / displayFPS_ - (now - lastDisplayUpdate), 1000 / modelFPS_ - (now - lastModelUpdate));
        if (waitTime > 0) SDL_Delay(waitTime);
    }

    return true;
}

bool Core::init_() {
    surface_ = generateModelPresetSurface(ModelPresets::randomParams);

    if (!sdlManager_.isInitialized()) {
		coreAppRunning_ = false;
		return false;
	}


    //if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    //    std::cout << "Error intializing SDL2: " << SDL_GetError() << std::endl;
    //    coreAppRunning_ = false;
    //    return false;
    //}
    //std::cout << "SDL2 video initialized..." << std::endl;

    //interface_->init(
    //    window_.getSDLWindow().get(),
    //    window_.getSDLRenderer().get(),
    //    [&](ModelParameters params) {handleGenerateModelRequest(params); }
    //);
    //modelTexture_ = std::make_shared<SDL_Texture>(SDL_CreateTextureFromSurface(window_.getSDLRenderer().get(), surface_), SDL_DestroyTexture);

    coreAppRunning_ = true;
    return true;
}

void Core::processEvents_() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        //SDL 
        switch(event.type)
        {
            case SDL_QUIT:
                coreAppRunning_ = false;
                break;
            case SDL_KEYDOWN:
                handleSDL_KEYDOWN(event);
                break;
        }
        //ImGui
        //ImGui_ImplSDL2_ProcessEvent(&event);
    }
}

void Core::update_() {
    SDL_Surface* previousState = SDL_ConvertSurface(surface_, surface_->format, 0);

    for (int row = 0; row < modelHeight_; row++) {
        int livingNeighbors = 0;
        bool cellAlive = false;
        for (int column = 0; column < modelWidth_; column++) {
            cellAlive = *((Uint8*)previousState->pixels + row * previousState->pitch + column);
            livingNeighbors = 0;
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //out of range rows
                    if (row + neighborRow < 0 || row + neighborRow >= modelHeight_) continue;
                    //out of range columns
                    if (column + neighborColumn < 0 || column + neighborColumn >= modelWidth_) continue;
                    ////center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;
                    //count
                    if (*((Uint8*)previousState->pixels + ((row + neighborRow) * previousState->pitch) + column + neighborColumn) == 1) livingNeighbors++;
                }
            }
            
            //If not alive and has 3 neighbors, become alive
            if (!cellAlive) {
                if (livingNeighbors == rule4_) *((Uint8*)surface_->pixels + row * surface_->pitch + column) = 1;
            }
            //If neighbors are less than 2 or more than 3, kill it.
            else  if(livingNeighbors < rule1_ || livingNeighbors > rule3_) *((Uint8*)surface_->pixels + row * surface_->pitch + column) = 0;
		}  
        
    }
    SDL_FreeSurface(previousState);
}

void Core::render_() {
    MainWindowSize windowSize = window_.getSize();
    SDL_Rect destinationRect{0, 0, windowSize.width, windowSize.height};
     
    //I should change this to have an update method, and then a simple render call.
    //interface_->render
    //(
    //    modelFPS_,
    //    measuredModelFPS_,
    //    modelRunning_,
    //    fillFactor_,
    //    modelWidth_,
    //    modelHeight_,
    //    rule1_,
    //    rule3_,
    //    rule4_
    //);


    //THIS WILL ALL MOVE TO THE WINDOW AND WE WILL CALL MainWindow::render(texture)
    //I might also provide in MainWindow something to get the texture, so you don't have to do the SDL call here...
    // And I should just use a single texture so that I don't allocate all of the time.  
    //Is this the right way, or should I be passing to an existing texture?
    //SDL_updateTexture could be the way.. 
    //I won't worry about this for now as this would change if I move to GPU. 
    /*auto texture = SDL_CreateTextureFromSurface(window_.getSDLRenderer().get(), surface_);*/

    //SDL_SetRenderTarget(window_.getSDLRenderer().get(), modelTexture_.get());
    //SDL_SetRenderDrawColor(window_.getSDLRenderer().get(), 0, 255, 0, 255);
    //SDL_RenderClear(window_.getSDLRenderer().get());
    //SDL_RenderCopy(window_.getSDLRenderer().get(), modelTexture_.get(), NULL, &destinationRect);
    //ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    //
    //SDL_RenderPresent(window_.getSDLRenderer().get());

    //SDL_DestroyTexture(modelTexture_.get());
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

void Core::handleGenerateModelRequest(const ModelParameters& params) {
    surface_ = generateModelPresetSurface(params);
}

SDL_Surface* Core::generateModelPresetSurface(const ModelParameters& params) {
    //First set the members to correspond with the parameters
    if (params.modelFPS > 0) modelFPS_ = params.modelFPS;
    if(modelWidth_ < params.minWidth) modelWidth_ = params.minWidth;
    if(modelHeight_ < params.minHeight) modelHeight_ = params.minHeight;
    if (params.modelWidth >  0) modelWidth_ = params.modelWidth;
    if (params.modelHeight > 0) modelHeight_ = params.modelHeight;
    if (params.fillFactor > 0) fillFactor_ = params.fillFactor;
    if (params.rule1 > 0) rule1_ = params.rule1;
    if (params.rule3 > 0) rule3_ = params.rule3;
    if (params.rule4 > 0) rule4_ = params.rule4;

    //Generate surface from parameters
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, modelWidth_, modelHeight_, 1, SDL_PIXELFORMAT_INDEX8);
    if (surface == nullptr) {
        std::cout << "Error creating surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    //Set the color palette
    const SDL_Color colors[2] = {
        {0, 0, 0, 255},
        {255, 255, 255, 255}
    };
    SDL_SetPaletteColors(surface->format->palette, colors, 0, 2);

    if (params.random) {

        srand(static_cast<unsigned>(time(nullptr)));
        for (int row = 0; row < modelHeight_; row++) {
            for (int column = 0; column < modelWidth_; column++) {
                *((Uint8*)surface->pixels + row * surface->pitch + column) = (rand() < params.fillFactor * (float)RAND_MAX) ? 1 : 0;
            }
        }
    }
    else {
        int startColumn = (modelWidth_ / 2) - (params.minWidth / 2);
        int startRow = modelHeight_ / 2 - (params.minHeight / 2);
        //std::vector encoding
        if (params.aliveCells.size() > 0) {
            for (auto pixel : params.aliveCells) {
                *((Uint8*)surface->pixels + (startRow + pixel.second) * surface->pitch + startColumn + pixel.first) = 1;
            }
        }

        //RLE encoding
        if (!params.runLengthEncoding.empty()) {
            populateSurfaceFromRLEString(
                surface,
                params.runLengthEncoding,
                startColumn,
                startRow
            );
        }
    }
    return surface;
}

void Core::populateSurfaceFromRLEString(
    SDL_Surface* surface,
    std::string model, 
    int startColumn, 
    int startRow) 
{
    int row = startRow;
	int column = startColumn;

    for (std::string::iterator modelIterator = model.begin(); modelIterator != model.end(); modelIterator++) {
        //First handle the edge cases
        
        if(*modelIterator == '!') break;

        int count = 1;
        if (isdigit(*modelIterator))
        {
            std::string stringInteger = "";
            while (isdigit(*modelIterator) && modelIterator != model.end())
            {
                stringInteger += *modelIterator;
                modelIterator++;
            }
            count = std::stoi(stringInteger);
        }
        if (*modelIterator == 'b') {
            for (int i = 0; i < count; i++) {
                column++;
            }
        }
        else if (*modelIterator == 'o') {
            for (int i = 0; i < count; i++) {
                *((Uint8*)surface->pixels + row * surface->pitch + column) = 1;
                column++;
            }
        }
        else if (*modelIterator == '$')
		{
            column = startColumn;
            for (int i = 0; i < count; i++) {
                row++;
            }
		}
    }
}

Core::~Core() {
    //ImGui interface must be deleted before SDL
    interface_.reset();

    //shutdown SDL
    //SDL_DestroyRenderer(renderer_);
    //SDL_DestroyWindow(window_);
    SDL_FreeSurface(surface_);

    sdlManager_.shutdown();
    //SDL_Quit();
}