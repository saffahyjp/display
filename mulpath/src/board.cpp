#include "include/board.h"

/*
	Implementation of some simple and also
	straightforward functions.
*/

Board::Board(int n, int m): m_n(n), m_m(m){
	this->m_map.resize(n, m);
}

bool Board::addObs(Point obs){
	int &pos = this->m_map[obs];
	if(pos)
		return false;
	this->m_obss.push_back(obs);
	pos = -1;
	return true;
}

bool Board::addTermset(
	const Vector<Point> &termset
){
	for(auto point: termset)
		if(this->m_map[point])
			return false;
	this->m_termsets.push_back(termset);
	int idx = this->m_termsets.size();
	for(auto point: termset)
		this->m_map[point] = idx;
	return true;
}

OStream &operator <<(
	OStream &ost, const Board &board
){
	ost << "Board " << board.n() << " * "
		<< board.m() << "\n";
	ost << board.map();
	return ost;
}
