#include "matrix.h"
#include "ascii_graph.h"

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <functional>
// C와 C++의 짬뽕
// 으 극혐


void printDecimal(double decimal)
{
	char buffer[64];
	int n = sprintf(buffer, "%lf", decimal) - 1;
	while(buffer[n] == '0') --n;
	if(buffer[n] == '.') --n;
	printf("%.*s", n + 1, buffer);
}


void printPolynomial(int size, double* coefficients)
{
	double a;
	
	for(int i = size-1; i >= 0; --i)
	{
		a = coefficients[i];
		if(a == 0 && i != 0) continue;
		if(i != size-1 || a < 0) printf(a >= 0 ? " + " : " - ");
		if(fabs(a) != 1 || i == 0) printDecimal(fabs(a));
		if(i != 0) printf("x");
		if(i >= 2) printf("^%d", i);
	}
}


double* getCoefficients(int N, double* x_array, double* y_array)
{
	Matrix::M2d<double> X(N, N), Y(1, N);
	for(int j = 0; j < N; ++j)
	{
		Y.at(0, j) = y_array[j];
		for(int i = 0; i < N; ++i)
		{
			X.at(i, j) = pow(x_array[j], i);
		}
	}
	
	Matrix::M2d<double> A = X.inverse() * Y;
	double* coefficients = new double[N];
	
	for(int i = 0; i < N; ++i)
	{
		coefficients[i] = A.at(0, i);
	}
	
	return coefficients;
}


#define make_xyfunc(expression) [](double x, double y) { return expression; }
void plotCoefficients(
	AsciiCanvas::Graph& graph,
	std::function<double(double)> func
)
{
	graph.plot_yfx(func, 0.1);
	graph.plot_fxy0(make_xyfunc(x), 0.3);
	graph.plot_fxy0(make_xyfunc(y), 0.3);
}
#undef make_xyfunc


int main()
{
	// ---------------- Input coordinate count ---------------- 
	
	int N, i;
	scanf("%d", &N);
	if(N == 1)
	{
		printf("2개 이상의 점이 필요합니다.");
		return 0;
	}
	
	
	// ---------------- Input coordinates ---------------- 
	
	double x_array[N], y_array[N];
	for(int i = 0; i < N; ++i)
	{
		scanf("%lf%lf", x_array+i, y_array+i);
		for(int j = 0; j < i; ++j)
		{
			if(x_array[i] == x_array[j])
			{
				printf("2개 이상의 점의 x 좌표가 같습니다.");
			}
		}
	}
	
	
	// ---------------- Getting the x-range ---------------- 
	
	double xmin = 0, xmax = 0;
	for(i = 0; i < N; ++i)
	{
		if(x_array[i] > xmax) xmax = x_array[i];
		if(x_array[i] < xmin) xmin = x_array[i];
	}
	double xsize = xmax - xmin;
	
	
	// ---------------- Calculate coefficients & function ---------------- 
	
	double* coefficients = getCoefficients(N, x_array, y_array);
	auto function = [&N, &coefficients](double x)
	{
		double sum = 0;
		for(int i = 0; i < N; ++i)
		{
			sum += coefficients[i] * pow(x, i);
		}
		return sum;
	};
	
	
	// ---------------- Getting the y-range ---------------- 
	
	double ymin = 0, ymax = 0;
	for(i = 0; i <= 20; ++i)
	{
		double x = xmin + (xmax - xmin) * i / 20.0;
		double y = function(x);
		if(y_array[i] > ymax) ymax = y_array[i];
		if(y_array[i] < ymin) ymin = y_array[i];
	}
	double ysize = ymax - ymin;
	
	
	// ---------------- Printing the function ----------------
	
	printf(" => f(x) = ");
	printPolynomial(N, coefficients);
	printf("\n");
	
	
	// ---------------- Plotting the function ----------------
	
	AsciiCanvas::Graph g(
		50, 30, 
		xmin - xsize * 0.1, ymin - ysize * 0.1, 
		xmax + xsize * 0.1, ymax + ysize * 0.1
	);
	plotCoefficients(g, function);
	
	
	// ---------------- Plotting points ---------------- 
	
	for(int i = 0; i < N; ++i)
	{
		g.plot_point(x_array[i], y_array[i], 1);
	}
	
	
	// ---------------- Printing the graph ----------------
	
	g.print();
	
	
	return 0;
}

