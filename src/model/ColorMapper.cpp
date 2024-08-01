#include "ColorMapper.hpp"

ColorMapper::ColorMapper()
{
	//Initialize the DualColor colormap
	ColormapMap[ColormapType::DualColor].fill(dualColorDeadSDLColor_);
	ColormapMap[ColormapType::DualColor][255] = dualColorAliveSDLColor_;
}

SDL_Color ColorMapper::getSDLColor(const int& colorIndex)
{
	return ColormapMap[static_cast<ColormapType>(selectedColorMapIndex)][colorIndex];
}

void ColorMapper::setDualColorAliveColor(const SDL_Color& color) 
{
	dualColorAliveSDLColor_ = color;
	ColormapMap[ColormapType::DualColor][255] = color;
	dualColorAliveFloatColor_ = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };
}

void ColorMapper::setDualColorAliveColor(const std::array<float, 3>& color) 
{
	dualColorAliveFloatColor_ = color;
	dualColorAliveSDLColor_ = SDL_Color(color[0] * 255, color[1] * 255, color[2] * 255);
	ColormapMap[ColormapType::DualColor][255] = dualColorAliveSDLColor_;
}

void ColorMapper::setDualColorDeadColor(const SDL_Color& color) 
{ 
	dualColorDeadSDLColor_ = color;
	std::fill_n(ColormapMap[ColormapType::DualColor].begin(), 254, color);
	dualColorDeadFloatColor_ = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };
}

void ColorMapper::setDualColorDeadColor(const std::array<float, 3>& color) 
{
	dualColorDeadFloatColor_ = color;
	dualColorDeadSDLColor_ = SDL_Color(color[0] * 255, color[1] * 255, color[2] * 255);
	std::fill_n(ColormapMap[ColormapType::DualColor].begin(), 254, dualColorDeadSDLColor_);
}

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