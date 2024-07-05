#ifndef SDL_COLORMAPS_8BIT_HPP
#define SDL_COLORMAPS_8BIT_HPP

#include <../submodules/tinycolormap/include/tinycolormap.hpp>

#include <array>
#include <SDL2/SDL_pixels.h>
namespace Colormaps {

	enum class SDL_ColormapNames_8Bit
	{
		SingleColor = 0,
		Github,
		Magma,
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

	const auto GithubLookup = getColormapData(tinycolormap::ColormapType::Github);
	const auto ViridisLookup = getColormapData(tinycolormap::ColormapType::Viridis);





}



#endif // SDL_COLORMAPS_8BIT_HPP