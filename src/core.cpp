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

    while (running_) {
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
        running_ = false;
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

    running_ = true;
    return true;
}

void Core::processEvents() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        //SDL 
        switch(event.type)
        {
            case SDL_QUIT:
                running_ = false;
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

    ImGui::Begin("GameOfLife");
    ImGui::Text("Hello, world!");
    ImGui::End();
    ImGui::Render();


    texture_ = SDL_CreateTextureFromSurface(renderer_, surface_);
    // texture_ = SDL_CreateTexture(renderer_, 
    //                             SDL_PIXELFORMAT_RGBA8888, 
    //                             SDL_TEXTUREACCESS_TARGET, 
    //                             windowWidth, 
    //                             windowHeight);

    SDL_SetRenderTarget(renderer_, texture_);
    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, NULL, &destinationRect);
    //ImGui goes after we clear the screen, but before RenderPresent
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    // SDL_SetRenderTarget(renderer_, NULL);

    
    SDL_RenderPresent(renderer_);
}

void Core::handleSDL_KEYDOWN(SDL_Event& event) {
    switch(event.key.keysym.sym)
    {
        case SDLK_ESCAPE:
            running_ = false;
            std::cout << "Escape key pressed: Exiting application." << std::endl;
            break;
        default:
            break;
    }
}

Core::~Core() {
    //Need to shutdown ImGui first
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    //shutdown SDL
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_DestroyTexture(texture_);
    SDL_FreeSurface(surface_);

    SDL_Quit();
}