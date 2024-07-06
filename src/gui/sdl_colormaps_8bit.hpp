#ifndef SDL_COLORMAPS_8BIT_HPP
#define SDL_COLORMAPS_8BIT_HPP

#include <../submodules/tinycolormap/include/tinycolormap.hpp>

#include <array>
#include <SDL2/SDL_pixels.h>
namespace Colormaps {

	enum class SDL_ColormapNames_8Bit
	{
		DualColor = 0,
		Cividis,
		Cubehelix,
		Github,
		Gray,
		Heat,
		Hot,
		HSV,
		Inferno, 
		Jet,
		Magma,
		Parula,
		Plasma,
		Turbo,
		Viridis
	};

	constexpr auto black_colors = [] {
		std::array<SDL_Color, 256> colors{};
		for (int i = 0; i < 256; ++i) {
			colors[i] = { 0, 0, 0, 255 }; // RGBA: Black with full opacity
		}
		return colors;
		}();

	constexpr std::array<SDL_Color, 256> getColormapData(const tinycolormap::ColormapType colorMapType)
	{
		std::array<SDL_Color, 256> colors{};
		for (int i = 0; i < 256; ++i) {
			auto color = tinycolormap::GetColor((double)i/255, colorMapType);
			colors[i] = SDL_Color{
				color.ri(),
				color.gi(),
				color.bi(),
				255
			};
		}

		//const std::array<const SDL_Color, 256> constColors;
		return colors;
	}

	const auto CividisLookup = getColormapData(tinycolormap::ColormapType::Cividis);
	const auto CubehelixLookup = getColormapData(tinycolormap::ColormapType::Cubehelix);
	const auto GithubLookup = getColormapData(tinycolormap::ColormapType::Github);
	const auto GrayLookup = getColormapData(tinycolormap::ColormapType::Gray);
	const auto HeatLookup = getColormapData(tinycolormap::ColormapType::Heat);
	const auto HotLookup = getColormapData(tinycolormap::ColormapType::Hot);
	const auto HSVLookup = getColormapData(tinycolormap::ColormapType::HSV);
	const auto InfernoLookup = getColormapData(tinycolormap::ColormapType::Inferno);
	const auto JetLookup = getColormapData(tinycolormap::ColormapType::Jet);
	const auto MagmaLookup = getColormapData(tinycolormap::ColormapType::Magma);
	const auto ParulaLookup = getColormapData(tinycolormap::ColormapType::Parula);
	const auto PlasmaLookup = getColormapData(tinycolormap::ColormapType::Plasma);
	const auto TurboLookup = getColormapData(tinycolormap::ColormapType::Turbo);
	const auto ViridisLookup = getColormapData(tinycolormap::ColormapType::Viridis);
}



#endif // SDL_COLORMAPS_8BIT_HPP