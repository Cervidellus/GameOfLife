#ifndef GAMEOFLIFE_MAINWINDOW_H
#define GAMEOFLIFE_MAINWINDOW_H

#include <memory>

//If you don't forward declare SDL, you get linkage errors due to an SDL macro that messes with the main function on windows.

struct ImGuiContext;
union SDL_Event;
struct SDL_Renderer;
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
	MainWindow(
		const char* windowName = "Game of Life"
		) ;

	~MainWindow();

	void initialize();

	void processEvent(SDL_Event& event);

	//void render(SDL_Texture* texture);
	//void display();

	MainWindowSize getSize();
	//std::shared_ptr<SDL_Window> getSDLWindow() { return window_; }
	//std::shared_ptr<SDL_Renderer> getSDLRenderer() { return renderer_; }


private:
	SDL_Window* window_;//shared so I can pass it to the interface.
	SDL_Renderer* renderer_;//shared so I can pass it to the interface.
};

#endif //GAMEOFLIFE_MAINWINDOW_H