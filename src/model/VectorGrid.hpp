#ifndef VECTOR_GRID
#define VECTOR_GRID

#include <mdspan>
#include <stdexcept>
#include <vector>

//For now using uint8_t. I should template it later.
struct VectorGrid
{
	VectorGrid() = default;
	VectorGrid(std::size_t width, std::size_t height) { resize(width, height); }

	std::vector<uint8_t> vector;
	//view of the vector as a 2D array, expressed underneath in column major order.
	std::mdspan<uint8_t, std::dextents<std::size_t, 2>> gridView;

	uint8_t& operator()(std::size_t x, std::size_t y) {
		if (!checkBounds(x, y)) throw std::out_of_range("Index out of bounds in VectorGrid");
		return gridView[std::array{x, y}];  // Column-major access
	}

	const uint8_t& operator()(std::size_t x, std::size_t y) const {
		if (!checkBounds(x, y)) throw std::out_of_range("Index out of bounds in VectorGrid");
		return gridView[std::array{ x, y }];  // Column-major access
	}

	auto begin() { return vector.begin(); }

	auto end() { return vector.end(); }

	auto begin() const { return vector.begin(); }

	auto end() const { return vector.end(); }


	//intended to be called within the struct for getting and setting, but can also be accessed externally.
	bool checkBounds(std::size_t x, std::size_t y) const
	{
		return (x < gridView.extent(0) && y < gridView.extent(1));
	}

	uint8_t get(std::size_t x, std::size_t y)
	{
		if (!checkBounds(x, y)) return 0;
		return gridView[std::array{ x, y }];
	}

	void set(std::size_t x, std::size_t y, uint8_t value)
	{
		if (!checkBounds(x,y)) throw std::out_of_range("Out of bound values in VectorGrid::set.");
		gridView[std::array{ x, y }] = value;
	}

	//Resize the underlying vector and the view, fill it with zeros. Then make a new mdspan to access a 2d array.
	void resize(std::size_t width, std::size_t height)
	{
		vector.resize(width * height, 0);
		gridView = std::mdspan<uint8_t, std::dextents<size_t, 2>>(vector.data(), width, height);
	}

	void zero()
	{
		std::fill(vector.begin(), vector.end(), 0);
	}

	std::size_t rows()
	{
		return gridView.extent(1);
	}

	std::size_t columns()
	{
		return gridView.extent(0);
	}
};


#endif// VECTOR_GRID

