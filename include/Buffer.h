#pragma once

#include <Util.h>
#include <assert.h>
#include <Color.h>

namespace RE
{
	template <typename T>
	class Buffer2D
	{
	public:
		// Default constructor. Does not allocate any memory.
		Buffer2D();

		// Allocates a 2D buffer with the specified width and height.
		// @param width - The width of the buffer.
		// @param height - The height of the buffer.
		// @param bClear - Should we clear the buffer?
		// @param clearVal - The value to clear the buffer to, if bClear is true.
		Buffer2D(int width, int height, bool bClear = false, int clearVal = 0);

		// Copy constructor.
		// @param other - The buffer to copy.
		Buffer2D(const Buffer2D<T>& other);
		// Move constructor.
		// @param other - The buffer to copy.
		Buffer2D(Buffer2D<T>&& other);

		// Destructor.
		virtual ~Buffer2D();

		// Equal operator
		// @param other - The buffer to copy.
		Buffer2D<T>& operator=(const Buffer2D<T>& other);
		// Equal operator
		// @param other - The buffer to move.
		Buffer2D<T>& operator=(Buffer2D<T>&& other);

		// Returns the element at the row and column.
		virtual T& operator()(int row, int col)const;
		virtual T& operator()(int flatIndex)const;

		// Gets the width of this buffer.
		int GetWidth()const;
		// Gets the height of thsi buffer.
		int GetHeight()const;
		// Gets the size of the buffer, in bytes.
		int GetSize()const;

		void Fill(T val);

	private:
		// Helper copy function.
		void Copy(const Buffer2D<T>& other);
		// Helper destroy function.
		void Destroy();

	protected:
		T * buffer;
		int width, height;
	};

	
	class ColorBuffer2D : public Buffer2D<Color>
	{
	public:
		ColorBuffer2D() : Buffer2D<Color>() { }
		ColorBuffer2D(int width, int height, bool bZeroBuffer = false) : Buffer2D<Color>(width, height, bZeroBuffer) { }

		// Copy constructor.
		// @param other - The buffer to copy.
		ColorBuffer2D(const ColorBuffer2D& other) : Buffer2D<Color>(other) { }
		// Move constructor.
		// @param other - The buffer to copy.
		ColorBuffer2D(ColorBuffer2D&& other) : Buffer2D<Color>(other) { }

		// Destructor.
		virtual ~ColorBuffer2D() {  };


		ColorBuffer2D& operator=(const ColorBuffer2D& other) = default;
		ColorBuffer2D& operator=(ColorBuffer2D&& other) = default;



		void SaveToImage(const std::string& file)const;
	};
}







// Implementation
namespace RE
{
	template <typename T>
	Buffer2D<T>::Buffer2D()
	{
		buffer = nullptr;
	}

	template <typename T>
	Buffer2D<T>::Buffer2D(int width, int height, bool bClear, int clearVal)
	{
		buffer = new T[width * height];
		this->width = width;
		this->height = height;

		if (bClear)
			memset(buffer, clearVal, width * height * sizeof(T));
	}



	template <typename T>
	Buffer2D<T>::Buffer2D(const Buffer2D<T>& other)
	{
		Copy(other);
	}

	template <typename T>
	Buffer2D<T>::Buffer2D(Buffer2D<T>&& other)
	{
		this->buffer = other.buffer;
		this->width = other.width;
		this->height = other.height;

		other.buffer = nullptr;
		other.width = 0;
		other.height = 0;
	}


	template <typename T>
	Buffer2D<T>::~Buffer2D()
	{
		Destroy();
	}



	template <typename T>
	Buffer2D<T>& Buffer2D<T>::operator=(const Buffer2D<T>& other)
	{
		if (this != &other)
		{
			Destroy();
			Copy(other);
		}

		return *this;
	}

	template <typename T>
	Buffer2D<T>& Buffer2D<T>::operator=(Buffer2D<T>&& other)
	{
		if (this != &other)
		{
			Destroy();

			this->buffer = other.buffer;
			this->width = other.width;
			this->height = other.height;

			other.buffer = nullptr;
			other.width = 0;
			other.height = 0;
		}

		return *this;
	}



	template <typename T>
	void Buffer2D<T>::Copy(const Buffer2D<T>& other)
	{
		this->width = other.width;
		this->height = other.height;

		buffer = new T[width * height];

		memcpy(buffer, other.buffer, width * height * sizeof(T));
	}

	template <typename T>
	void Buffer2D<T>::Destroy()
	{
		if (buffer)
		{
			delete[] buffer;
			buffer = nullptr;
		}
	}





	template <typename T>
	T& Buffer2D<T>::operator()(int row, int col)const
	{
		assert(col >= 0 && col < width && row >= 0 && row < height);

		return buffer[row * width + col];
	}

	template <typename T>
	T& Buffer2D<T>::operator()(int flatIndex)const
	{
		assert(flatIndex >= 0 && flatIndex < (width * height));

		return buffer[flatIndex];
	}



	template <typename T>
	int Buffer2D<T>::GetWidth()const
	{
		return width;
	}

	template <typename T>
	int Buffer2D<T>::GetHeight()const
	{
		return height;
	}

	template <typename T>
	int Buffer2D<T>::GetSize()const
	{
		return width * height * sizeof(T);
	}

	
	template <typename T>
	void Buffer2D<T>::Fill(T val)
	{
		for (auto row = 0; row < height; row++)
		{
			for (auto col = 0; col < width; col++)
			{
				operator()(row, col) = val;
			}
		}
	}
}