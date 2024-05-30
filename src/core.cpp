#include <core.hpp>
#include <interface.hpp>
#include <modelparameters.hpp>
//#include <modelpresets.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

Core::Core() {
    interface_ = std::make_unique<Interface>();
}

bool Core::run() {
    if(!init()) return false;
    int lastDisplayUpdate, lastModelUpdate, now;
    now = lastDisplayUpdate = lastModelUpdate = SDL_GetTicks();

    while (coreAppRunning_) {  
        now = SDL_GetTicks();

        processEvents();
        if (modelRunning_ && now - lastModelUpdate > 1000 / modelFPS_)
        {
            update();
            measuredModelFPS_ = 1000 / (now - lastModelUpdate);
            lastModelUpdate = now;
        }
        if (now - lastDisplayUpdate > 1000 / displayFPS_)
        {
            render();
            lastDisplayUpdate = now;
        }
        //wait function so we don't burn too much cpu
        int waitTime = std::min(1000 / displayFPS_ - (now - lastDisplayUpdate), 1000 / modelFPS_ - (now - lastModelUpdate));
        if (waitTime > 0) SDL_Delay(waitTime);
    }

    return true;
}

bool Core::init() {
    surface_ = generateModelPresetSurface(ModelPresets::randomParams);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error intializing SDL2: " << SDL_GetError() << std::endl;
        coreAppRunning_ = false;
        return false;
    }
    std::cout << "SDL2 video initialized..." << std::endl;
    
    window_ = SDL_CreateWindow(
        "GameOfLife", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 
        720, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
        
    renderer_ = SDL_CreateRenderer(
        window_, 
        -1, 
        SDL_RENDERER_ACCELERATED);

    //Make modelwidth and modelheight the same as window size
    //SDL_GetRendererOutputSize(renderer_, &modelWidth_, &modelHeight_);

    interface_->init(
        window_, 
        renderer_,
        [&](ModelParameters params) {handleGenerateModelRequest(params); }
    );

    coreAppRunning_ = true;
    return true;
}

void Core::processEvents() {
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
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
}

void Core::update() {
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

void Core::render() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer_, &windowWidth, &windowHeight);
    SDL_Rect destinationRect{0, 0, windowWidth, windowWidth};

    interface_->render
    (
        modelFPS_,
        measuredModelFPS_,
        modelRunning_,
        fillFactor_,
        modelWidth_,
        modelHeight_,
        rule1_,
        rule3_,
        rule4_
    );

    //Is this the right way, or should I be passing to an existing texture?
    //SDL_updateTexture could be the way.. 
    //I won't worry about this for now as this would change if I move to GPU. 
    auto texture = SDL_CreateTextureFromSurface(renderer_, surface_);

    SDL_SetRenderTarget(renderer_, texture);
    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
    SDL_RenderClear(renderer_);
     SDL_RenderCopy(renderer_, texture, NULL, &destinationRect);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    
    SDL_RenderPresent(renderer_);

    SDL_DestroyTexture(texture);
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
        else if (*modelIterator == '$')
        {
            column = startColumn;
            row++;
        }
        else {
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


        }


       


        //if (*modelIterator == 'b') column++;
        //else if (*modelIterator == 'o')
        //{
        //    *((Uint8*)surface->pixels + (startRow + row) * surface->pitch + startColumn + column) = 1;
        //    column++;
        //}


    }
	/*for (char c : model) {
		if (c == 'b') {
			column++;
		}
		else if (c == 'o') {
			*((Uint8*)surface_->pixels + row * surface_->pitch + column) = 1;
			column++;
		}
		else if (c == '$') {
			row++;
			column = startColumn;
		}
		else if (c == '!') {
			break;
		}
		else if (isdigit(c)) {
			int count = c - '0';
			while (isdigit(model[1])) {
				count = count * 10 + (model[1] - '0');
				model.erase(1, 1);
			}
			for (int i = 0; i < count; i++) {
				if (model[0] == 'b') {
					column++;
				}
				else if (model[0] == 'o') {
					*((Uint8*)surface_->pixels + row * surface_->pitch + column) = 1;
					column++;
				}
			}
		}
	}*/

}

Core::~Core() {
    //ImGui interface must be deleted before SDL
    interface_.reset();

    //shutdown SDL
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_FreeSurface(surface_);

    SDL_Quit();
}