#ifndef COLORMAPPER_H
#define COLORMAPPER_H

#include <../submodules/tinycolormap/include/tinycolormap.hpp>

#include <array>
#include <map>

#include <SDL2/SDL_pixels.h>

class ColorMapper
{
public:
	//ColorMapper();

	static enum class ColormapType
	{
		DualColor = 0, Cividis, Cubehelix, Github, Gray, Heat, Hot,
		HSV, Inferno, Jet, Magma, Parula, Plasma, Turbo, Viridis
	};

	//ColorMap names for use by ImGui widgets
	constexpr static const char* ColorMapNames[15] = {
		"DualColor","Cividis","Cubehelix","Github","Gray","Heat","Hot",
		"HSV","Inferno","Jet","Magma","Parula","Plasma","Turbo","Viridis" };

	const std::array<SDL_Color, 256> CividisLookup = getColormapData_(tinycolormap::ColormapType::Cividis);
	const std::array<SDL_Color, 256> CubehelixLookup = getColormapData_(tinycolormap::ColormapType::Cubehelix);
	const std::array<SDL_Color, 256> GithubLookup = getColormapData_(tinycolormap::ColormapType::Github);
	const std::array<SDL_Color, 256> GrayLookup = getColormapData_(tinycolormap::ColormapType::Gray);
	const std::array<SDL_Color, 256> HeatLookup = getColormapData_(tinycolormap::ColormapType::Heat);
	const std::array<SDL_Color, 256> HotLookup = getColormapData_(tinycolormap::ColormapType::Hot);
	const std::array<SDL_Color, 256> HSVLookup = getColormapData_(tinycolormap::ColormapType::HSV);
	const std::array<SDL_Color, 256> InfernoLookup = getColormapData_(tinycolormap::ColormapType::Inferno);
	const std::array<SDL_Color, 256> JetLookup = getColormapData_(tinycolormap::ColormapType::Jet);
	const std::array<SDL_Color, 256> MagmaLookup = getColormapData_(tinycolormap::ColormapType::Magma);
	const std::array<SDL_Color, 256> ParulaLookup = getColormapData_(tinycolormap::ColormapType::Parula);
	const std::array<SDL_Color, 256> PlasmaLookup = getColormapData_(tinycolormap::ColormapType::Plasma);
	const std::array<SDL_Color, 256> TurboLookup = getColormapData_(tinycolormap::ColormapType::Turbo);
	const std::array<SDL_Color, 256> ViridisLookup = getColormapData_(tinycolormap::ColormapType::Viridis);


private:
	constexpr std::array<SDL_Color, 256> getColormapData_(const tinycolormap::ColormapType colorMapType)
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

};

#endif //COLORMAPPER_H

