#include "Agent.h"

using namespace std;

short HumAgent::get_move() {
	int mv;
	cout << "What is your move? ";
	cin >> mv;
	return mv;
}