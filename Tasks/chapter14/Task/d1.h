#pragma once
#include "b1.h"
#include<iostream>
class D1 : public B1 {
public:
	void vf() const override { std::cout << " D1::vf()"; }
	void f() const { std::cout << " D1::f()"; }
	void pvf() const override { std::cout << " D1::pvf()"; }
};
