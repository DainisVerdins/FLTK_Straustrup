#pragma once
#include<iostream>
class B1 {
public:
	virtual void vf() const { std::cout << "B1::vf()"; };
	virtual void pvf() const = 0;
	void f() const { std::cout << "B1::f()"; };
};
