#pragma once

class State {
public:
	virtual void init() = 0;
	virtual void make_move(int) = 0;
};