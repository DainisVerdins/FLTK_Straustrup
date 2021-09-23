#pragma once
#include<string>
#include<iostream>

#include "b2.h"

class D21:public B2 {
public:
	void pvf() const override { std::cout << str; }

private:
	std::string str{ "D21::pvf() "};
};
