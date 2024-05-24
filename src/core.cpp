#include <core.hpp>

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

Core::Core() {
}

bool Core::run() {
    if(!init()) return false;

    int lastTime = SDL_GetTicks();//returns milliseconds 

    while (coreAppRunning_) {  
        int now = SDL_GetTicks();
        if(now - lastTime < 1000/desiredFPS_) continue;
        measuredFPS_ = 1000/(now - lastTime);
        lastTime = now;

        processEvents();
        if(modelRunning_) update();//only update if the model is running (otherwise it is paused
        // update();
        render();
    }

    return true;
}

bool Core::init() {

    //Configure SDL2
    //Temporary surface 
    surface_ = SDL_LoadBMP("C:/src/GameOfLife/resources/fern.bmp");//not loading from debug?

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error intializing SDL2: " << SDL_GetError() << std::endl;
        coreAppRunning_ = false;
        return false;
    }
    std::cout << "SDL2 video initialized..." << std::endl;
    
    window_ = SDL_CreateWindow(
        "GameOfLife", 
        50, 
        50, 
        800, 
        600, 
        SDL_WINDOW_RESIZABLE);
        
    renderer_ = SDL_CreateRenderer(
        window_, 
        -1, 
        SDL_RENDERER_ACCELERATED);

    //auto driver = SDL_GetCurrentVideoDriver();

    //Configure ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
    ImGui_ImplSDLRenderer2_Init(renderer_);
    ImGui_ImplSDLRenderer2_CreateDeviceObjects();

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
    //for each pixel:
//     Any live cell with fewer than two live neighbours dies (referred to as underpopulation).
// Any live cell with more than three live neighbours dies (referred to as overpopulation).
// Any live cell with two or three live neighbours lives, unchanged, to the next generation.
// Any dead cell with exactly three live neighbours comes to life.


    // 1. Count the number of neighbor pixels that are alive. Will have to handle edge cases.

    // 2. If pixel is alive and has 2 or 3 neighbors, do nothing.

    // 3. If pixel is alive and has less than 2 or more than 3 neighbors, kill it.  

    // 4. If pixel is dead and has exactly 3 neighbors, bring it to life.

    for (int row = 0; row < matrixHeight_; row++) {
        for (int column = 0; column < matrixWidth_; column++) {
            //Uint8 pixelValue = *((Uint8*)surface_->pixels + row * surface_->pitch + column );
            //std::cout << "Pixel value: " << static_cast<int16_t>(pixelValue) << " at column:" << column << std::endl;
			//Count the number of living neighbors
			int livingNeighbors = 0;
            //I might be able to expand this to different scan window sizes? e.g. 5x5
            for (int neighborRow = -1; neighborRow <= 1; neighborRow++) {
                for (int neighborColumn = -1; neighborColumn <= 1; neighborColumn++) {
                    //center pixel
                    if (neighborRow == 0 && neighborColumn == 0) continue;
                    //out of range rows
                    if (row + neighborRow < 0 || row + neighborRow >= matrixHeight_) continue;
                    //out of range columns
                    if (column + neighborColumn < 0 || column + neighborColumn >= matrixWidth_) continue;
                    if (*((Uint8*)surface_->pixels + (row + neighborRow) * surface_->pitch + column + neighborColumn) == 1) livingNeighbors++;
                }
            }

            //if (row == 1 && column < 20) {
            //    std::cout << "Neighbors:" << livingNeighbors << " at column:" << column << " row: " << row << std::endl;
            //}
            
   //         for (int dy = -1; dy <= 1; dy++) {
   //             for (int dx = -1; dx <= 1; dx++) {
			//		if(dx == 0 && dy == 0) continue;
			//		if(column + dx < 0 || column + dx >= matrixWidth_) continue;
			//		if(row + dy < 0 || row + dy >= matrixHeight_) continue;
   //                 if (*((Uint8*)surface_->pixels + (row + dy) * surface_->pitch + (column + dx) / 8) & (1 << ((column + dx) % 8))) {
			//			neighbors++;
			//		}
			//	}
			//}

			////If pixel is alive
   //         if (*((Uint8*)surface_->pixels + row * surface_->pitch + column / 8) & (1 << (column % 8))) {
   //             if (neighbors < 2 || neighbors > 3) {
			//		//Kill the pixel
			//		*((Uint8*)surface_->pixels + row * surface_->pitch + column / 8) &= ~(1 << (column % 8));
			//	}
   //         }
   //         else {
   //             if (neighbors == 3) {
			//		//Bring the pixel to life
			//		*((Uint8*)surface_->pixels + row * surface_->pitch + column / 8) |= 1 << (column % 8);
			//	}
			//}
		}   
    }


     //for (int pixelIndex = 0; pixelIndex < matrixWidth_ * matrixHeight_; pixelIndex++) {
     //    // int x = pixelIndex % matrixWidth_;
     //    // int y = pixelIndex / matrixWidth_;
     //    //Count the number of neighbors
     //    int neighbors = 0;
     //    // surface_
     //    // auto pixel = ;
     //    // if ((Uint8*)surface_->pixels + pixelIndex)

     //    surface_->
     //    Uint8 *pixelValue = (Uint8*)surface_->pixels + pixelIndex * 8;
     //    auto pixelValue2 = *pixelValue;
     //    bool alive = *((Uint8*)surface_->pixels + pixelIndex) & 1;
     //    //just to verify I will just turn it off if it is alive, or turn it on if it is dead
     //    //if (alive) {
     //    //    *((Uint8*)surface_->pixels + pixelIndex) &= ~1;
     //    //} else {
     //    //    *((Uint8*)surface_->pixels + pixelIndex) |= 1;
     //    //}

     //}
        // bool alive = *((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) & (1 << (x % 8));
        // for(int dy = -1; dy <= 1; dy++) {
        //     for(int dx = -1; dx <= 1; dx++) {
        //         if(dx == 0 && dy == 0) continue;
        //         if(x + dx < 0 || x + dx >= matrixWidth_) continue;
        //         if(y + dy < 0 || y + dy >= matrixHeight_) continue;
        //         if(*((Uint8*)surface_->pixels + (y + dy) * surface_->pitch + (x + dx) / 8) & (1 << ((x + dx) % 8))) {
        //             neighbors++;
        //         }
        //     }
        // }

    //     // If pixel is alive
    //     if(*((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) & (1 << (x % 8))) {
    //         if(neighbors < 2 || neighbors > 3) {
    //             //Kill the pixel
    //             *((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) &= ~(1 << (x % 8));
    //         }
    //     } else {
    //         if(neighbors == 3) {
    //             //Bring the pixel to life
    //             *((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) |= 1 << (x % 8);
    //         }
    //     }
    // }
    
    // for(int row = 0; row < matrixHeight_; row++) {
    //     for(int x = 0; x < matrixWidth_; x++) {
    //         //Count the number of neighbors
    //         int neighbors = 0;
    //         for(int dy = -1; dy <= 1; dy++) {
    //             for(int dx = -1; dx <= 1; dx++) {
    //                 if(dx == 0 && dy == 0) continue;
    //                 if(x + dx < 0 || x + dx >= matrixWidth_) continue;
    //                 if(y + dy < 0 || y + dy >= matrixHeight_) continue;
    //                 if(*((Uint8*)surface_->pixels + (y + dy) * surface_->pitch + (x + dx) / 8) & (1 << ((x + dx) % 8))) {
    //                     neighbors++;
    //                 }
    //             }
    //         }

    //         //If pixel is alive
    //         if(*((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) & (1 << (x % 8))) {
    //             if(neighbors < 2 || neighbors > 3) {
    //                 //Kill the pixel
    //                 *((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) &= ~(1 << (x % 8));
    //             }
    //         } else {
    //             if(neighbors == 3) {
    //                 //Bring the pixel to life
    //                 *((Uint8*)surface_->pixels + y * surface_->pitch + x / 8) |= 1 << (x % 8);
    //             }
    //         }
    //     }
    // }

}

void Core::render() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer_, &windowWidth, &windowHeight);
    SDL_Rect destinationRect{0, 0, windowWidth, windowWidth};

    //IMGUI
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Options");
    ImGui::Text("Hello, world!");
    ImGui::SliderInt("Desired FPS", &desiredFPS_, 1, 120);
    ImGui::Text("Measured FPS: %d", measuredFPS_);
    if (modelRunning_) {
        if (ImGui::Button("Pause Model")) {
            modelRunning_ = false;
        }
    } else {
        if (ImGui::Button("Generate Model")) {
            surface_ = generateModelSurface(matrixWidth_, matrixHeight_, fillFactor_);
        }
        ImGui::SameLine();
        if (ImGui::Button("Start Model")) {
            modelRunning_ = true;
        }
    }

    // if(modelRunning_){
    //     ImGuiInputTextFlags inputFlags = 
    // }
    ImGuiInputTextFlags modelInputFlags = modelRunning_ ? ImGuiInputTextFlags_ReadOnly : 0;

    //TODO:: limit to positive values
    //TODO:: endure that input is 4-byte aligned
    ImGui::InputInt("Width", &matrixWidth_, 100, 100, modelInputFlags);
    // ImGui::SameLine();
    ImGui::InputInt("Height", &matrixHeight_, 100, 100, modelInputFlags);

    ImGui::End();
    ImGui::Render();
    

    auto texture = SDL_CreateTextureFromSurface(renderer_, surface_);
    // texture_ = SDL_CreateTexture(renderer_, 
    //                             SDL_PIXELFORMAT_RGBA8888, 
    //                             SDL_TEXTUREACCESS_TARGET, 
    //                             windowWidth, 
    //                             windowHeight);

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

//This can be a bit slow for large surfaces..
//Cuda  could speed this up
//Large models also really decrease frame rate...
//working directly with a texture could be much faster
SDL_Surface* Core::generateModelSurface(int width, int height, float fillFactor) {
    //Create and single bit surface to represent black and white values
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 1, SDL_PIXELFORMAT_INDEX8);//single bit per pixel image format
    if(surface == nullptr) {
        std::cout << "Error creating surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    //Set the color palette
    const SDL_Color colors[2] = {
        {0, 0, 0, 255},
        {255, 255, 255, 255}
    };
    SDL_SetPaletteColors(surface->format->palette, colors, 0, 2);

    //Fill with random data
    srand(static_cast<unsigned>(time(nullptr)));
    for(int row = 0; row < height; row++) {
        for(int column = 0; column < width; column++) {
            *((Uint8*)surface->pixels + row * surface->pitch + column) = (rand() < fillFactor * (float)RAND_MAX) ? 1 : 0;
        }
    }

    return surface;
}

Core::~Core() {
    //Need to shutdown ImGui first
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    //shutdown SDL
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_FreeSurface(surface_);

    SDL_Quit();
}