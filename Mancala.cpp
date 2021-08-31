// Mancala.cpp : Defines the entry point for the application.
//

#include "Mancala.h"
#include <array>

using namespace std;

MancalaState::MancalaState() {
	init();
}

void MancalaState::init() {
	cur_p = 0;
	p1pock = 0;
	p2pock = 0;
	gameover = false;
	for (int i = 0; i < BOARD_SIZE; i++) {
		board[i] = 4;
	}
}

void MancalaState::set_cur_p(short cp) {
	cur_p = cp;
}

void MancalaState::add_p1pock(short num) {
	p1pock += num;
}

void MancalaState::add_p2pock(short num) {
	p2pock += num;
}

void MancalaState::set_gameover() {
	gameover = true;
}

void MancalaState::set_board(short index, short value) {
	if (index < BOARD_SIZE) {
		board[index] = value;
	}
}

void MancalaState::make_move(int move) {
	int held_rocks;
	bool flip = true;
	if (move < 0 || move >= 6)
		return;

	int cur_spot = cur_p * 6 + move;

	held_rocks = board[cur_spot];
	board[cur_spot] = 0;



	while (held_rocks > 0) {
		if (cur_spot == 0) {
			if (cur_p == 0) {
				p1pock++;
				held_rocks--;
				if (held_rocks == 0) {
					flip = false;
					break;
				}
			}
			cur_spot = 6;
			board[cur_spot]++;
			held_rocks--;
			continue;
		}
		else if (cur_spot == 11) {
			if (cur_p == 1) {
				p2pock++;
				held_rocks--;
				if (held_rocks == 0) {
					flip = false;
					break;
				}
			}
			cur_spot = 5;
			board[cur_spot]++;
			held_rocks--;
			continue;
		}

		// Handle directional movement
		if (cur_spot < 6)
			cur_spot--;
		else
			cur_spot++;

		board[cur_spot]++;
		held_rocks--;
	}

	// Check for capture
	if (flip && (cur_spot / 6 == cur_p) && board[cur_spot] == 1) {
		if (cur_p == 0) {
			if (board[cur_spot + 6] > 0) {
				p1pock += board[cur_spot + 6];
				p1pock++;
				board[cur_spot] = 0;
				board[cur_spot + 6] = 0;
			}
		}
		else if (cur_p == 1) {
			if (board[cur_spot - 6] > 0) {
				p2pock += board[cur_spot - 6];
				p2pock++;
				board[cur_spot] = 0;
				board[cur_spot - 6] = 0;
			}
		}
	}

	// Flip players if needed
	if (flip) {
		if (cur_p == 0)
			cur_p = 1;
		else
			cur_p = 0;
	}

	array<bool, 6> legal_moves = this->get_legal_moves();
	for (int i = 0; i < 6; i++) {
		if (legal_moves[i])
			return;

	}
	gameover = true;
	for (int j = 0; j < 6; j++) {
		p1pock += board[j];
		p2pock += board[j + 6];
		board[j] = 0;
		board[j + 6] = 0;
	}
}

array<bool, 6> MancalaState::get_legal_moves() {
	array<bool, 6> retvals;

	for (int i = 0; i < 6; i++) {
		if (board[cur_p * 6 + i] > 0)
			retvals[i] = true;
		else
			retvals[i] = false;
	}

	return retvals;
}

void MancalaState::print() {
	cout << "Player 1: " << p1pock << endl;
	cout << "Player 2: " << p2pock << endl;
	for (int i = 0; i < 6; i++) {
		cout << "  " << board[i];
	}
	cout << endl;
	for (int i = 6; i < 12; i++) {
		cout << "  " << board[i];
	}
	cout << endl;
	if (cur_p == 0)
		cout << "Player 1 to move." << endl;
	else
		cout << "Player 2 to move." << endl;
}

MancalaState MancalaState::copy() {
	MancalaState new_state;
	new_state.cur_p = cur_p;
	new_state.p1pock = p1pock;
	new_state.p2pock = p2pock;
	new_state.gameover = gameover;
	new_state.board[0] = board[0];
	new_state.board[1] = board[1];
	new_state.board[2] = board[2];
	new_state.board[3] = board[3];
	new_state.board[4] = board[4];
	new_state.board[5] = board[5];
	new_state.board[6] = board[6];
	new_state.board[7] = board[7];
	new_state.board[8] = board[8];
	new_state.board[9] = board[9];
	new_state.board[10] = board[10];
	new_state.board[11] = board[11];

	return new_state;
}

float MancalaEval::evaluate(MancalaState state) {
	// Should only ever see MancalaState
	float eval = state.p1pock - state.p2pock;
	return eval;
}

Mancala::Mancala(Agent* p_1, MancalaMMAgent a1, MancalaMMAgent a2) {
	p1 = p_1;
	ai1 = a1;
	ai2 = a2;
}

short Mancala::play(short mode) {
	int move;

	if (state.gameover)
		state.init();

	while (!state.gameover) {
		state.print();
		if (mode == 0) {
			move = p1->get_move();
		}
		else if (mode == 1) {
			// AIvP
			if (state.cur_p == 0) {
				move = ai1.get_move(state);
				cout << "Computer plays: " << move << endl;
				cout << endl;
			}
			else {
				move = p1->get_move();
			}
		}
		else if (mode == 2) {
			// PvAI
			if (state.cur_p == 0) {
				move = p1->get_move();
			}
			else {
				move = ai1.get_move(state);
				cout << "Computer plays: " << move << endl;
			}
		}
		else {
			// AIvAI
			if (state.cur_p == 1) {
				move = ai2.get_move(state);
				cout << "Computer plays: " << move << endl;
			}
			else {
				move = ai1.get_move(state);
				cout << "Computer plays: " << move << endl;
			}
		}

		state.make_move(move);
	}

	cout << "Game Over!" << endl;
	state.print();

	if (state.p1pock > state.p2pock)
		return 0;
	return 1;
}

SMDouble::SMDouble(float sc, short mv) {
	score = sc;
	move = mv;
}

SMQuad::SMQuad(float sc, short mv, float a, float b) {
	score = sc;
	move = mv;
	alpha = a;
	beta = b;
}

MancalaMMAgent::MancalaMMAgent() {
	evaluator = MancalaEval();
	max_depth = 10;
}

MancalaMMAgent::MancalaMMAgent(MancalaEval e, int max_d) {
	evaluator = e;
	max_depth = max_d;
}

short MancalaMMAgent::get_move(MancalaState state) {
	bool isMax = state.cur_p == 0;

	SMDouble result = abpruned(state, 0, max_depth, isMax, -9999, 9999);

	cout << "Eval of position: " << result.score << endl;

	return result.move;
}

SMDouble MancalaMMAgent::minimax(MancalaState state, int cur_depth, int max_depth, bool isMax) {
	if (cur_depth == max_depth) {
		float score = evaluator.evaluate(state);
		return SMDouble(score, -1);
	}

	array<bool, 6> legal_moves = state.get_legal_moves();

	array<float, 6> move_evals;

	bool still_legal_moves_flag = false;

	for (int i = 0; i < 6; i++) {
		// Explore all moves to legal depth
		if (!legal_moves[i])
			continue;

		still_legal_moves_flag = true;
		MancalaState state_copy = state.copy();
		state_copy.make_move(i);
		
		if (state_copy.gameover) {
			float score = evaluator.evaluate(state);
			return SMDouble(score, i);
		}
		else {
			SMDouble future = minimax(
				state_copy,
				cur_depth + 1,
				max_depth,
				state_copy.cur_p == 0
			);

			move_evals[i] = future.score;
		}
	}
	// Look for best move

	short best_move = -1;
	float best_score = -1000;

	for (int i = 0; i < 6; i++) {
		if (!legal_moves[i])
			continue;
		if (best_move == -1) {
			best_move = i;
			best_score = move_evals[i];
		}
		else {
			if (isMax) {
				if (move_evals[i] > best_score) {
					best_move = i;
					best_score = move_evals[i];
				}
			}
			else {
				if (move_evals[i] < best_score) {
					best_move = i;
					best_score = move_evals[i];
				}
			}
		}
	}
	return SMDouble(best_score, best_move);
}

SMDouble MancalaMMAgent::abpruned(MancalaState state, int cur_depth, int max_depth, bool isMax, float alpha, float beta) {
	if (cur_depth >= max_depth) {
		int score = evaluator.evaluate(state);
		return SMDouble(score, -1);
	}

	array<bool, 6> legal_moves = state.get_legal_moves();

	array<float, 6> move_evals = { 99, 99, 99, 99, 99, 99 };

	bool still_legal_moves_flag = false;

	if (isMax) {
		float v = alpha;
		for (int i = 0; i < 6; i++) {
			// Explore all moves to legal depth
			if (!legal_moves[i])
				continue;

			still_legal_moves_flag = true;
			MancalaState state_copy = state.copy();
			state_copy.make_move(i);

			if (state_copy.gameover) {
				float score = evaluator.evaluate(state_copy);
				move_evals[i] = score;
				v = max(score, v);
				if (beta <= v)
					break;
			}

			else {
				int new_depth = cur_depth;
				if (state_copy.cur_p != state.cur_p)
					new_depth++;

				SMDouble future = abpruned(
					state_copy,
					new_depth,
					max_depth,
					state_copy.cur_p == 0,
					v,
					beta
				);

				move_evals[i] = future.score;

				v = max(future.score, v);
				if (beta <= v)
					break;
			}
		}
	}
	else {
		float v = beta;
		for (int i = 0; i < 6; i++) {
			// Explore all moves to legal depth
			if (!legal_moves[i])
				continue;

			still_legal_moves_flag = true;
			MancalaState state_copy = state.copy();
			state_copy.make_move(i);

			if (state_copy.gameover) {
				float score = evaluator.evaluate(state_copy);
				move_evals[i] = score;
				v = min(score, v);
				if (v <= alpha)
					break;
			}

			else {
				int new_depth = cur_depth;
				if (state_copy.cur_p != state.cur_p)
					new_depth++;

				SMDouble future = abpruned(
					state_copy,
					new_depth,
					max_depth,
					state_copy.cur_p == 0,
					alpha,
					v
				);

				move_evals[i] = future.score;

				v = min(future.score, v);
				if (v <= alpha)
					break;
			}
		}
	}

	// Look for best move

	short best_move = -1;
	float best_score = -1000;

	for (int i = 0; i < 6; i++) {
		if (!legal_moves[i] || move_evals[i] == 99)
			continue;
		if (best_move == -1) {
			best_move = i;
			best_score = move_evals[i];
		}
		else {
			if (isMax) {
				if (move_evals[i] > best_score) {
					best_move = i;
					best_score = move_evals[i];
				}
			}
			else {
				if (move_evals[i] < best_score) {
					best_move = i;
					best_score = move_evals[i];
				}
			}
		}
	}

	return SMDouble(best_score, best_move);
}

int main()
{
	HumAgent *h = new HumAgent();
	MancalaState s;
	MancalaEval evaluator;

	int depth;
	cout << "What search depth? ";
	cin >> depth;
	cout << endl;

	MancalaMMAgent mma(evaluator, depth);
	//cout << mma.get_move(s, 11) << endl;

	Mancala m(h, mma, mma);
	
	int status = 1;

	while (status > 0 && status < 3) {
		cout << "First or second? ";
		cin >> status;
		cout << endl;

		if (status == 1) {
			m.play(1);
		}
		else if (status == 2) {
			m.play(2);
		}
	}

	return 0;
}

