#pragma once

#include <iostream>

class Agent {
public:
	virtual short get_move() = 0;
};

class HumAgent :public Agent {
public:
	short get_move();
};