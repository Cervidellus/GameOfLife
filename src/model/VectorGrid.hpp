#ifndef VECTOR_GRID
#define VECTOR_GRID

#include <mdspan>
#include <stdexcept>
#include <vector>
#include <cstddef>

//For now using uint8_t. I should template it later.
struct VectorGrid
{
	VectorGrid() = default;
	VectorGrid(std::size_t width, std::size_t height) { resize(width, height); }

	uint8_t& operator()(std::size_t x, std::size_t y) {
		return data_[y * rows_ + x];
	}

	const uint8_t& operator()(std::size_t x, std::size_t y) const {
		return data_[y * rows_ + x];
	}

	void swap(VectorGrid& other) noexcept
	{
		//the "correct" way would be to also swap the cols and rows.
		//In my case I know they are always the same, so I am going to avoid that.
		std::swap(data_, other.data_);
	}

	auto begin() { return data_.begin(); }

	auto end() { return data_.end(); }

	auto begin() const { return data_.begin(); }

	auto end() const { return data_.end(); }

	void resize(std::size_t width, std::size_t height)
	{
		data_.resize(width * height, 0);
		rows_ = height;
		cols_ = width;
	}

	void zero()
	{
		std::fill(data_.begin(), data_.end(), 0);
	}

	std::size_t rows() const
	{
		return rows_;
	}

	std::size_t columns() const
	{
		return cols_;
	}

private:
	std::vector<uint8_t> data_;
	int cols_ = 0;
	int rows_ = 0;
};

#endif// VECTOR_GRID

