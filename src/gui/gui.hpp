#ifndef GAMEOFLIFE_GUI_HPP
#define GAMEOFLIFE_GUI_HPP

#include "mainwindow.hpp"
#include "interface.hpp"

#include <memory>
#include <string>

class MainWindow;

class GUI {
public:
	GUI();
	~GUI();

public:

	bool initialize(std::string windowTitle);

	MainWindow mainWindow;
	Interface interface;
	void shutdown();
};

#endif //GAMEOFLIFE_GUI_HPP

