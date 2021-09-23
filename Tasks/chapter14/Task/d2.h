#pragma once
#include "d1.h"
class D2 : public D1 {
public:
	void pvf() const override { std::cout << " D2::pvf()"; }
};
