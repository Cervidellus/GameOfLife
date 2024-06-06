#include <core.hpp>
#include <gui\mainwindow.hpp>
#include <gui\interface.hpp>
#include <model\modelparameters.hpp>

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

    gui_.initialize(
		"Game of Life",
		[&](ModelParameters params) {handleGenerateModelRequest(params);}
	);

    surface_ = generateModelPresetSurface(ModelPresets::randomParams);

    coreAppRunning_ = true;
    return true;
}

void Core::processEvents_() {
    //Eventually I will have a SDL_Event handler
    //You will be able to register callbacks for events
    //For example the sidebar, which uses ImGui, will be able to use ImGui_ImplSDL2_ProcessEvent(&event) as a callback.
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
        
        gui_.mainWindow.processEvent(event);
    }
}

void Core::update_() {
    SDL_Surface* previousState = SDL_ConvertSurface(surface_, surface_->format, 0);

    for (int row = 0; row < activeModelParams_.modelHeight; row++) {
        int livingNeighbors = 0;
        bool cellAlive = false;
        for (int column = 0; column < activeModelParams_.modelWidth; column++) {
            cellAlive = *((Uint8*)previousState->pixels + row * previousState->pitch + column);
            livingNeighbors = 0;
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //out of range rows
                    if (row + neighborRow < 0 || row + neighborRow >= activeModelParams_.modelHeight) continue;
                    //out of range columns
                    if (column + neighborColumn < 0 || column + neighborColumn >= activeModelParams_.modelWidth) continue;
                    ////center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;
                    //count
                    if (*((Uint8*)previousState->pixels + ((row + neighborRow) * previousState->pitch) + column + neighborColumn) == 1) livingNeighbors++;
                }
            }
            
            //If not alive and has 3 neighbors, become alive
            if (!cellAlive) {
                if (livingNeighbors == activeModelParams_.rule4) *((Uint8*)surface_->pixels + row * surface_->pitch + column) = 1;
            }
            //If neighbors are less than 2 or more than 3, kill it.
            else  if(livingNeighbors < activeModelParams_.rule1 || livingNeighbors > activeModelParams_.rule3) *((Uint8*)surface_->pixels + row * surface_->pitch + column) = 0;
		}  
        
    }
    SDL_FreeSurface(previousState);
}

void Core::render_() {
    gui_.mainWindow.clear();
    //Draw the model    
    SDL_Rect destinationRect = {0, 0, activeModelParams_.modelWidth, activeModelParams_.modelHeight};
    auto modelTexture = SDL_CreateTextureFromSurface(gui_.mainWindow.sdlRenderer, surface_);
    gui_.mainWindow.drawTexture(modelTexture, destinationRect);
    SDL_DestroyTexture(modelTexture);//I should make this a member so that I am not constantly creating and destroying it.5

    gui_.interface.draw(activeModelParams_, measuredModelFPS_);
    gui_.mainWindow.renderPresent();

    

    //THIS WILL ALL MOVE TO THE WINDOW AND WE WILL CALL MainWindow::render(texture)
    //I might also provide in MainWindow something to get the texture, so you don't have to do the SDL call here...
    // And I should just use a single texture so that I don't allocate all of the time.  
    //Is this the right way, or should I be pass5ng to an existing texture?
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
    //gui_.mainWindow.render(modelTexture_);
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
    if (params.modelFPS > 0) activeModelParams_.modelFPS = params.modelFPS;
    if(activeModelParams_.modelWidth < params.minWidth) activeModelParams_.minWidth = params.minWidth;
    if(activeModelParams_.modelHeight < params.minHeight) activeModelParams_.minHeight = params.minHeight;
    if (params.modelWidth >  0) activeModelParams_.modelWidth= params.modelWidth;
    if (params.modelHeight > 0) activeModelParams_.modelHeight = params.modelHeight;
    if (params.fillFactor > 0) activeModelParams_.fillFactor = params.fillFactor;
    if (params.rule1 > 0) activeModelParams_.rule1 = params.rule1;
    if (params.rule3 > 0) activeModelParams_.rule3 = params.rule3;
    if (params.rule4 > 0) activeModelParams_.rule4 = params.rule4;

    //Generate surface from parameters...
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0, 
        activeModelParams_.modelWidth, 
        activeModelParams_.modelHeight, 
        1, 
        SDL_PIXELFORMAT_INDEX8);

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
        for (int row = 0; row < activeModelParams_.modelHeight; row++) {
            for (int column = 0; column < activeModelParams_.modelWidth; column++) {
                *((Uint8*)surface->pixels + row * surface->pitch + column) = (rand() < params.fillFactor * (float)RAND_MAX) ? 1 : 0;
            }
        }
    }
    else {
        int startColumn = (activeModelParams_.modelWidth / 2) - (params.minWidth / 2);
        int startRow = activeModelParams_.modelHeight / 2 - (params.minHeight / 2);
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
    //interface_.reset();

    //shutdown SDL
    //SDL_DestroyRenderer(renderer_);
    //SDL_DestroyWindow(window_);

    SDL_FreeSurface(surface_);

    sdlManager_.shutdown();
}