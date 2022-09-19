#include "matrix.h"
#include "ascii_graph.h"

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <functional>

#include <windows.h>


#pragma execution_character_set( "utf-8" )


int WIDTH = 50;
int HEIGHT = 30;


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
	graph.plot_yfx(func, 0.7);
	graph.plot_fxy0(make_xyfunc(x), 0.3);
	graph.plot_fxy0(make_xyfunc(y), 0.3);
}
#undef make_xyfunc


int main()
{
	SetConsoleOutputCP( 65001 );

	// ---------------- Input coordinate count ---------------- 
	
	int N = 0, i;
	printf("점의 개수를 입력해주세요: ");
	while(N < 2)
	{
		scanf("%d", &N);
		if(N >= 2) break;
		printf("점의 개수는 2 이상이어야 합니다. ");
	}
	
	
	// ---------------- Input coordinates ---------------- 
	
	double x_array[N], y_array[N];
	for(int i = 0; i < N; ++i)
	{
		input_x:
		printf("\nx_%d: ", i + 1);
		scanf("%lf", x_array+i);
		for(int j = 0; j < i; ++j)
		{
			if(x_array[i] == x_array[j])
			{
				printf("x 좌표가 겹치는 점이 있습니다.");
				goto input_x;
			}
		}
		printf("y_%d: ", i + 1);
		scanf("%lf", y_array+i);
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
	
	printf("\n계수 계산 중...\n");
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
		if(y > ymax) ymax = y;
		if(y < ymin) ymin = y;
	}
	double ysize = ymax - ymin;
	
	
	// ---------------- Plotting the function ----------------
	
	printf("그래프 그리는 중...\n");
	AsciiCanvas::Graph g(
		WIDTH, HEIGHT, 
		xmin - xsize * 0.3, ymin - ysize * 0.2, 
		xmax + xsize * 0.3, ymax + ysize * 0.2
	);
	plotCoefficients(g, function);
	
	
	// ---------------- Plotting points ---------------- 
	
	for(int i = 0; i < N; ++i)
	{
		g.plot_point(x_array[i], y_array[i], 1);
	}
	
	
	// ---------------- Printing the function ----------------
	
	printf(" => f(x) = ");
	printPolynomial(N, coefficients);
	printf("\n");
	
	
	// ---------------- Printing the graph ----------------
	
	g.print();
	
	
	return 0;
}

