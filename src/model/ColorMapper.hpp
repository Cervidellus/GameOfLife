#ifndef COLORMAPPER_H
#define COLORMAPPER_H

#include <../submodules/tinycolormap/include/tinycolormap.hpp>

#include <array>
#include <map>

#include <SDL2/SDL_pixels.h>

class ColorMapper
{
public:
	ColorMapper();

	static enum class ColormapType
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

	void setDualColorAliveColor(SDL_Color color);
	void setDualColorDeadColor(SDL_Color color);

private:
	SDL_Color dualColorAliveSDLColor_ = SDL_Color(255, 255, 0, 255 );
	SDL_Color dualColorDeadSDLColor_ = SDL_Color( 0, 0, 255, 255 );

	constexpr std::array<SDL_Color, 256> getColormapData_(const tinycolormap::ColormapType colorMapType);
};

#endif //COLORMAPPER_H

