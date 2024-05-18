#include <core.hpp>

#include <iostream>

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
        update();
        render();
    }

    return true;
}

bool Core::init() {

    //Configure SDL2
    //Temporary surface 
    surface_ = SDL_LoadBMP("resources/fern.bmp");

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        // If SDL fails to initialize, print an error message
        std::cout << "Error intializing SDL2: " << SDL_GetError() << std::endl;
        coreAppRunning_ = false;
        return false;
    }
    std::cout << "SDL2 video initialized." << std::endl;
    
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

    //Configure ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
    ImGui_ImplSDLRenderer2_Init(renderer_);

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
    // Update the matrix
}

void Core::render() {
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer_, &windowWidth, &windowHeight);
    SDL_Rect destinationRect{0, 0, windowWidth, windowWidth};

    //IMGUI
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_);
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

SDL_Surface* Core::generateModelSurface(int width, int height, float fillFactor) {
    //Create and single bit surface to represent black and white values
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 1, SDL_PIXELFORMAT_INDEX1MSB);//single bit per pixel image format
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
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if(rand() < fillFactor * RAND_MAX) {
                SDL_Rect rect{x, y, 1, 1};
                SDL_FillRect(surface, &rect, 1);
            }
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