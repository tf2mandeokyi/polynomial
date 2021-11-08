#pragma once

#include "ascii_canvas.h"
#include <functional>

namespace AsciiCanvas
{
	
	template<typename T> using f = std::function<T>;
	
	int sign(double val) {
	    return (0 < val) - (val < 0);
	}
	
	class Graph
	{
	private:
		Canvas _canvas;
		double _xmin, _ymin, _xmax, _ymax, _xscale, _yscale;
	
	public:
		Canvas& getCanvas() { return _canvas; }
		
		
		Graph(
			int width, int height,
			double xmin, double ymin,
			double xmax, double ymax
		)
		 : _canvas(width, height), _xmin(xmin), _ymin(ymin), _xmax(xmax),
		   _ymax(ymax)
		{
			_xscale = width / (xmax - xmin);
			_yscale = height / (ymax - ymin);
		}
		
		
		void plot_point(double x, double y, double opacity)
		{
			int px = ceil(_canvas.width()  * (x - _xmin) / (_xmax - _xmin));
			int py = floor(_canvas.height() * (y - _ymax) / (_ymin - _ymax));
			_canvas.drawPoint(px, py, opacity);
		}
		
		
		void plot_fxy0(f<double(double,double)> f_xy, double opacity)
		{
			double x, y;
			double dx = (_xmax - _xmin) / (double) _canvas.width();
			double dy = (_ymax - _ymin) / (double) _canvas.height();
			for(int py = 0; y < _canvas.height(); ++py)
			{
				y = _ymin + py * dy;
				for(int px = 0; px < _canvas.width(); ++px)
				{
					x = _xmin + px * dx;
					double z = sign(f_xy(x-dx, y-dy))
					         + sign(f_xy(x   , y-dy))
					         + sign(f_xy(x-dx, y   ))
					         + sign(f_xy(x   , y   ));
					if(z < 4 && z > -4)
					{
						_canvas.drawPoint(px, _canvas.height() - py, opacity);
					}
				}
			}
		}
		
		
		void plot_yfx(f<double(double)> fx, double opacity)
		{
			plot_fxy0([&fx](double x, double y) {
				return y - fx(x);
			}, opacity);
		}
		
		
		void print()
		{
			_canvas.print();
		}
		
	};
}
