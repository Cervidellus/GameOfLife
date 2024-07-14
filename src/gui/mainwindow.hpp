#ifndef GAMEOFLIFE_MAINWINDOW_H
#define GAMEOFLIFE_MAINWINDOW_H

#include <memory>
#include <string>

//Window creates and destroys the SDL window and renderer.
//It returns properties of the window.
//It initializes the window for use with ImGui and passes events to ImGui.

//If you don't forward declare SDL, you get linkage errors due to an SDL macro that messes with the main function on windows.

//struct ImGuiContext;
union SDL_Event;
struct SDL_Rect;
struct SDL_Renderer;
//class SDL_Surface;
class SDL_Texture;
struct SDL_Window;

struct MainWindowSize
{
	int width;
	int height;
};

class MainWindow
{
public:
	MainWindow() ;

	~MainWindow();

	void initialize(std::string windowName);

	//I'm not quite sure where this belongs. This processes the event globally, so If I have more than one window it will repeat. 
	void processEvent(SDL_Event& event);

	MainWindowSize getSize();

	void drawTexture(SDL_Texture* texture, SDL_Rect destination);
	void clear();
	void renderPresent();

	//Not super happy about the raw pointers.
	SDL_Window* sdlWindow = nullptr;
	SDL_Renderer* sdlRenderer = nullptr;

private:

};

#endif //GAMEOFLIFE_MAINWINDOW_H