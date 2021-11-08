#pragma once

#include <vector>
#include <stdio.h>
#include <math.h>

namespace AsciiCanvas
{
	
	namespace
	{
		const char* GRADIENT = " .:;+=xX$&#";
		const int GRADIENT_LEVEL = 10;
	}
	
	
	class Canvas
	{
	private:
		int _width, _height;
		std::vector<float> _pixels;
		
		void set(int x, int y, float value)
		{
			if(x >= 0 && x < _width && y >= 0 && y < _height)
				_pixels[y * _width + x] = value;
		}
		
		void add(int x, int y, float value)
		{
			if(x >= 0 && x < _width && y >= 0 && y < _height)
				_pixels[y * _width + x] += value;
		}
		
	public:
		int width() { return _width; }
		int height() { return _height; }
		
		
		Canvas(int width, int height)
		 : _width(width), _height(height), _pixels(width * height)
		{
			std::fill(_pixels.begin(), _pixels.end(), 0);
		}
		
		
		Canvas() : Canvas(0, 0)
		{}
		
		
		void drawPoint(float x, float y, float opacity)
		{
			
			int fx = floor(x), fy = floor(y),
			    cx = ceil(x), cy = ceil(y);
			
			float dx = x - fx, dy = y - fy;
			
			double d00 = dx*dx + dy*dy,
				   d10 = (1-dx)*(1-dx) + dy*dy,
				   d01 = dx*dx + (1-dy)*(1-dy),
				   d11 = (1-dx)*(1-dx) + (1-dy)*(1-dy);
			
			double v00 = d00 < 1 ? sqrt(-d00*d00 + 2*d00) + 1 : 0,
				   v10 = d10 < 1 ? sqrt(-d10*d10 + 2*d10) + 1 : 0,
				   v01 = d01 < 1 ? sqrt(-d01*d01 + 2*d01) + 1 : 0,
				   v11 = d11 < 1 ? sqrt(-d11*d11 + 2*d11) + 1 : 0;
			
			add(fx, fy, v00 * opacity);
			add(cx, fy, v10 * opacity);
			add(fx, cy, v01 * opacity);
			add(cx, cy, v11 * opacity);
			
		}
		
		
		void print()
		{
			int i = 0, temp, ch;
			for(int y = 0; y < _height; ++y)
			{
				for(int x = 0; x < _width; ++x)
				{
					temp = round(_pixels[i++] * GRADIENT_LEVEL);
					if(temp < 0) temp = 0;
					if(temp > GRADIENT_LEVEL) temp = GRADIENT_LEVEL;
					ch = GRADIENT[temp];
					printf("%c%c", ch, ch);
				}
				printf("\n");
			}
		}
	};
}
