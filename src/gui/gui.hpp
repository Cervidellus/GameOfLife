#ifndef GAMEOFLIFE_GUI_HPP
#define GAMEOFLIFE_GUI_HPP

#include "mainwindow.hpp"
#include "interface.hpp"

#include <memory>
#include <string>

//class ImGuiContext; Forward declaring ImGuiContext causes a linker error.
class MainWindow;

class GUI {
public:
	GUI();
	~GUI();

public:

	bool initialize(
		std::string windowTitle,
		std::function<void(ModelParameters presetParameters)> presetCallback
		//std::function<SDL_Texture* ()> getWindowTextureCallback
	);

	MainWindow mainWindow;
	Interface interface;
	//void shutdown();
	
private:
	//For now I am using a global context. 
	// later I can add support for multiple contexts used in different windows.
	//ImGuiContext* context_;
};

#endif //GAMEOFLIFE_GUI_HPP

