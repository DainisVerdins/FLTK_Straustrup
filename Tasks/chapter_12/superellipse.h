#pragma once
#define _USE_MATH_DEFINES
#include "Fix/Simple_window.h"
#include "Fix/Graph.h"
#include "Fix/std_lib_facilities.h"
#include<iostream>
#include<cstdint>
#include<cmath>
#include<vector>
class Superellipse
{
	Graph_lib::Lines graph;
	std::vector<Point>points;
	//adds line to previos one dot to make line 
	void add_connected(const Point& a, const Point& b);

	double sgn(double x);

	

	int a;
	int b;
	double m;
	double n;
	double precision;
public:
	Graph_lib::Lines& get_graph() { return graph; }
	Superellipse(const Point& centerCoords,double a, double b, double m, double n, double precision = 0.01);
	~Superellipse();
};

