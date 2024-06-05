#ifndef GAMEOFLIFE_GUI_HPP
#define GAMEOFLIFE_GUI_HPP

#include "mainwindow.hpp"

#include <memory>

class MainWindow;

class GUI {
public:
	GUI();
	~GUI();

public:

	MainWindow mainWindow;
	//void shutdown();
	
private:
	ImGuiContext* context_;
};

#endif //GAMEOFLIFE_GUI_HPP

