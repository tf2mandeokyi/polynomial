#pragma once

#include <stdexcept>
#include <stdlib.h>
#include <vector>
#include <memory>

namespace Matrix
{
	template<typename T> class M2d
	{
	private:
		std::vector<T> _array;
		int _width, _height;
		
	public:
		int width() { return _width; }
		int height() { return _height; }
		
		
		M2d(int width, int height) 
		 : _width(width), _height(height), _array(width * height)
		{}
		
		
		M2d(int width, int height, const T* array)
		 : M2d(width, height)
		{
			for(int i = 0; i < width * height; ++i)
			{
				_array[i] = array[i];
			}
		}
		
		
		~M2d()
		{}
		
		
		static M2d identity(int N)
		{
			M2d result(N, N);
			for(int y = 0; y < N; ++y) for(int x = 0; x < N; ++x)
			{
				result.at(x, y) = x == y ? 1 : 0;
			}
			return result;
		}
		
		
		T at(int x, int y) const
		{
			if(x < 0 || x >= _width || y < 0 || y >= _height)
				throw std::range_error("Matrix index out of range");
			return _array[y * _width + x];
		}
		
		
		T& at(int x, int y)
		{
			if(x < 0 || x >= _width || y < 0 || y >= _height)
				throw std::range_error("Matrix index out of range");
			return _array[y * _width + x];
		}
		
		
		M2d<T> operator+(const M2d<T>& other) const
		{
			if(_width != other._width || _height != other._height)
				throw std::length_error("Mismatching matrix size on addition");
			M2d<T> result(_width, _height);
			for(int i = 0; i < _width * _height; ++i)
			{
				result._array[i] = _array[i] + other._array[i];
			}
			return result;
		}
		
		
		M2d<T> operator-(const M2d<T>& other) const
		{
			if(_width != other._width || _height != other._height)
				throw std::length_error("Mismatching matrix size on subtraction");
			M2d<T> result(_width, _height);
			for(int i = 0; i < _width * _height; ++i)
			{
				result._array[i] = _array[i] - other._array[i];
			}
			return result;
		}
		
		
		M2d<T> operator*(T scale) const
		{
			M2d<T> result(_width, _height);
			for(int i = 0; i < _width * _height; ++i)
			{
				result._array[i] = _array[i] * scale;
			}
			return result;
		}
		
		
		M2d<T> operator*(const M2d<T>& other) const
		{
			if(_width != other._height)
				throw std::length_error("Mismatching matrix size on multiplication");
			M2d<T> result(other._width, _height);
			T sum;
			for(int y = 0; y < _height; ++y) for(int x = 0; x < other._width; ++x)
			{
				sum = 0;
				for(int i = 0; i < _width; ++i) sum += at(i, y) * other.at(x, i);
				result.at(x, y) = sum;
			}
			return result;
		}
		
		
		M2d<T> operator/(T scale) const
		{
			M2d<T> result(_width, _height);
			for(int i = 0; i < _width * _height; ++i)
			{
				result._array[i] = _array[i] / scale;
			}
			return result;
		}
		
		
		M2d<T> withoutRow(int row) const
		{
			if(row < 0 || row >= _height)
				throw std::range_error("Row out of range");
			M2d<T> result(_width, _height-1);
			int i = 0;
			for(int y = 0; y < _height; ++y)
			{
				if(y == row) continue;
				for(int x = 0; x < _width; ++x)
				{
					result._array[i++] = at(x, y);
				}
			}
			return result;
		}
		
		
		M2d<T> withoutColumn(int column) const
		{
			if(column < 0 || column >= _width)
				throw std::range_error("Column out of range");
			M2d<T> result(_width-1, _height);
			int i = 0;
			for(int y = 0; y < _height; ++y)
			{
				for(int x = 0; x < _width; ++x)
				{
					if(x == column) continue;
					result._array[i++] = at(x, y);
				}
			}
			return result;
		}
		
		
		T minor(int row, int column) const
		{
			if(column < 0 || column >= _width || row < 0 || row >= _height)
				throw std::range_error("Matrix index out of range");
			M2d<T> result(_width-1, _height-1);
			int i = 0;
			for(int y = 0; y < _height; ++y)
			{
				if(y == row) continue;
				for(int x = 0; x < _width; ++x)
				{
					if(x == column) continue;
					result._array[i++] = at(x, y);
				}
			}
			return result.determinant();
		}
		
		
		M2d<T> transpose() const
		{
			M2d<T> result(_height, _width);
			for(int y = 0; y < _height; ++y) for(int x = 0; x < _width; ++x)
			{
				result.at(y, x) = at(x, y);
			}
			return result;
		}
		
		
		/**
		 * This is equal to:
		 * 
		 * matrix.adjugate() / matrix.determinant()
		 */
		M2d<T> inverse() const
		{
			if(_width != _height)
			{
				throw std::length_error("Not a square matrix; couldn't find the inverse.");
			}
			
			T det = determinant();
			if(_width != _height)
			{
				throw std::length_error("The matrix is not inversable.");
			}
			
			return adjugate() / det;
		}
		
		
		/**
		 * This is equal to:
		 * 
		 * matrix.cofactor().transpose()
		 */
		M2d<T> adjugate() const
		{
			if(_width != _height)
			{
				throw std::length_error("Not a square matrix; couldn't find the adjugate.");
			}
			return cofactor();
		}
		
		
		M2d<T> cofactor() const
		{
			if(_width != _height)
			{
				throw std::length_error("Not a square matrix; couldn't find the cofactor.");
			}
			M2d<T> result(_width, _height);
			
			T temp;
			for(int y = 0; y < _height; ++y)
			{
				for(int x = 0; x < _width; ++x)
				{
					temp = minor(x, y);
					result.at(x, y) = (x + y) % 2 == 0 ? temp : -temp;
				}
			}
			
			return result;
		}
		
		
		T determinant() const
		{
			if(_width != _height)
			{
				throw std::length_error("Not a square matrix; couldn't find the determinant.");
			}
			if(_width == 1)
			{
				return _array[0];
			}
			
			T result = 0, temp;
			for(int x = 0; x < _width; ++x)
			{
				temp = at(x, 0) * withoutRow(0).withoutColumn(x).determinant();
				result += x % 2 == 0 ? temp : -temp;
			}
			return result;
		}
		
		
		void print()
		{
			int max = 0, len;
			for(int y = 0; y < _height; ++y) for(int x = 0; x < _width; ++x)
			{
				len = snprintf(NULL, 0, "%lf", at(x, y));
				if(len > max) max = len;
			}
			char buffer[32];
			sprintf(buffer, "%%%dlf", max);
			printf("[\n");
			for(int y = 0; y < _height; ++y) 
			{
				printf("  [ ");
				for(int x = 0; x < _width; ++x)
				{
					printf(buffer, at(x, y));
					if(x != _width - 1)
					{
						printf("  ");
					}
				}
				printf(" ]\n");
			}
			printf("]\n");
		}
	};


	template<typename T> M2d<T> operator*(T scale, const M2d<T>& matrix)
	{
		return matrix * scale;
	}
	
	
}
