// Mancala.h : Include file for standard system include files,
// or project specific include files.



#include <iostream>
#include <array>

// TODO: Reference additional headers your program requires here.
#include "State.h"
#include "Eval.h"
#include "Agent.h"
#include "thc.h"

class MancalaState :public State {
public:
	short cur_p, p1pock, p2pock;
	bool gameover;
	short board[12];
	const int BOARD_SIZE = 12;

public:
	MancalaState();
	void init();
	void set_cur_p(short);
	void add_p1pock(short);
	void add_p2pock(short);
	void set_gameover();
	void set_board(short, short);

	void make_move(int);
	std::array<bool, 6> get_legal_moves();

	MancalaState copy();

	void print();
};

class MancalaEval :public Eval {
public:
	float evaluate(MancalaState);
};

class SMDouble {
public:
	float score;
	short move;
	SMDouble(float, short);
};

class SMQuad {
public:
	float score;
	short move;
	float alpha;
	float beta;

	SMQuad(float, short, float, float);
};

class MancalaMMAgent {
	MancalaEval evaluator;
	int max_depth;
	
public:
	MancalaMMAgent();
	MancalaMMAgent(MancalaEval, int);
	short get_move(MancalaState);

private:
	SMDouble minimax(MancalaState, int cur_depth, int max_depth, bool isMax);
	SMDouble abpruned(MancalaState, int cur_depth, int max_depth, bool isMax, float alpha, float beta);
};

class Mancala{
public:
	MancalaState state;
	Agent* p1;
	MancalaMMAgent ai1;
	MancalaMMAgent ai2;
	
public:
	Mancala(Agent*, MancalaMMAgent, MancalaMMAgent);
	short play(short);
};

