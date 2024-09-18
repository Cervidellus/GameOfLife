#ifndef COLORMAPPER_H
#define COLORMAPPER_H

#include <../submodules/tinycolormap/include/tinycolormap.hpp>

#include <array>
#include <map>

#include <SDL3/SDL_pixels.h>

class ColorMapper
{
public:
	ColorMapper();

	enum class ColormapType
	{
		DualColor = 0, Cividis, Cubehelix, Github, Gray, Heat, Hot,
		HSV, Inferno, Jet, Magma, Parula, Plasma, Turbo, Viridis
	};

	//ColorMap names for use by ImGui widgets
	constexpr static const char* ColorMapNames[15] = {
		"DualColor","Cividis","Cubehelix","Github","Gray","Heat","Hot",
		"HSV","Inferno","Jet","Magma","Parula","Plasma","Turbo","Viridis" };

	std::map<ColormapType, std::array<SDL_Color, 256>> ColormapMap{
		{ColormapType::DualColor, std::array<SDL_Color, 256>()},
		{ColormapType::Cividis, getColormapData_(tinycolormap::ColormapType::Cividis) },
		{ColormapType::Cubehelix, getColormapData_(tinycolormap::ColormapType::Cubehelix) },
		{ColormapType::Github, getColormapData_(tinycolormap::ColormapType::Github) },
		{ColormapType::Gray, getColormapData_(tinycolormap::ColormapType::Gray) },
		{ColormapType::Heat, getColormapData_(tinycolormap::ColormapType::Heat) },
		{ColormapType::Hot, getColormapData_(tinycolormap::ColormapType::Hot) },
		{ColormapType::HSV, getColormapData_(tinycolormap::ColormapType::HSV) },
		{ColormapType::Inferno, getColormapData_(tinycolormap::ColormapType::Inferno) },
		{ColormapType::Jet, getColormapData_(tinycolormap::ColormapType::Jet) },
		{ColormapType::Magma, getColormapData_(tinycolormap::ColormapType::Magma) },
		{ColormapType::Parula, getColormapData_(tinycolormap::ColormapType::Parula) },
		{ColormapType::Plasma, getColormapData_(tinycolormap::ColormapType::Plasma) },
		{ColormapType::Turbo, getColormapData_(tinycolormap::ColormapType::Turbo) },
		{ColormapType::Viridis, getColormapData_(tinycolormap::ColormapType::Viridis) }
	};

	SDL_Color getSDLColor(const int& colorIndex);

	//getters and setters needed to modify DualColor colormap
	//And to make the float and SDL_Color values equivalent.
	SDL_Color getDualColorAliveSDLColor() { return dualColorAliveSDLColor_; };
	SDL_Color getDualColorDeadSDLColor() { return dualColorDeadSDLColor_; };
	std::array<float, 3> getDualColorAliveColorFloat() { return dualColorAliveFloatColor_; };
	std::array<float, 3> getDualColorDeadColorFloat() { return dualColorDeadFloatColor_; };

	void setDualColorAliveColor(const SDL_Color& color);
	void setDualColorDeadColor(const SDL_Color& color);
	void setDualColorAliveColor(const std::array<float, 3>& color);
	void setDualColorDeadColor(const std::array<float, 3>& color);

	int selectedColorMapIndex = (int)ColormapType::Plasma;

private:
	//Alive and dead colors are stored as both an SDL_Color, which is used in the colormap, 
	//and float arrays, which are used by ImGui.
	//This prevents having to divide the float arrays every time I call the gui.
	SDL_Color dualColorAliveSDLColor_ = SDL_Color(255, 255, 0, 255 );
	SDL_Color dualColorDeadSDLColor_ = SDL_Color( 0, 0, 255, 255 );
	std::array<float, 3> dualColorAliveFloatColor_ = { 1.0f, 1.0f, 0.0f };
	std::array<float, 3> dualColorDeadFloatColor_ = { 0.0f, 0.0f, 1.0f };

	constexpr std::array<SDL_Color, 256> getColormapData_(const tinycolormap::ColormapType colorMapType);
};

#endif //COLORMAPPER_H

