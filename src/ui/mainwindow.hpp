#ifndef GAMEOFLIFE_MAINWINDOW_H
#define GAMEOFLIFE_MAINWINDOW_H

#include <memory>

//If you don't forward declare SDL, you get linkage errors due to an SDL macro that messes with the main function on windows.
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

	void render(std::shared_ptr<SDL_Texture> texture);
	void display();

	MainWindowSize getSize();
	std::shared_ptr<SDL_Window> getSDLWindow() { return window_; }
	std::shared_ptr<SDL_Renderer> getSDLRenderer() { return renderer_; }

private:
	std::shared_ptr<SDL_Window> window_;//shared so I can pass it to the interface.
	std::shared_ptr<SDL_Renderer> renderer_;//shared so I can pass it to the interface.
};

#endif //GAMEOFLIFE_MAINWINDOW_H