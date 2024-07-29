#include "ColorMapper.hpp"

ColorMapper::ColorMapper()
{
	//Initialize the DualColor colormap
	ColormapMap[ColormapType::DualColor].fill(dualColorDeadSDLColor_);
	ColormapMap[ColormapType::DualColor][255] = dualColorAliveSDLColor_;
}

void ColorMapper::setDualColorAliveColor(SDL_Color color) 
{
	dualColorAliveSDLColor_ = color;
	ColormapMap[ColormapType::DualColor][255] = color;
	
};

void ColorMapper::setDualColorDeadColor(SDL_Color color) 
{ 
	dualColorDeadSDLColor_ = color;
	std::fill_n(ColormapMap[ColormapType::DualColor].begin(), 254, color);
	//ColormapMap[ColormapType::DualColor].fill(color);
	//ColormapMap[ColormapType::DualColor][255] = dualColorAliveSDLColor_;
};

constexpr std::array<SDL_Color, 256> ColorMapper::getColormapData_(const tinycolormap::ColormapType colorMapType)
{
	std::array<SDL_Color, 256> colors{};
	for (int i = 0; i < 256; ++i) {
		auto color = tinycolormap::GetColor((double)i / 255, colorMapType);
		colors[i] = SDL_Color{
			color.ri(),
			color.gi(),
			color.bi(),
			255
		};
	}
	return colors;
}