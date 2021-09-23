#pragma once
#include<iostream>
#include "b2.h"
class D22 : public B2 {
public:
	void pvf() const override { std::cout << num; }

private:
	int num=13;
};
