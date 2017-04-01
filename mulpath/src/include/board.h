#ifndef _BOARD_H
#define _BOARD_H

#include "global.h"
#include "matrix.h"

class Board{
protected:
	const int m_n, m_m;
	Vector<Point> m_obss;
	Vector<Vector<Point>> m_termsets;
	/*
		-1	obstacle
		0	empty grid
		>0	terminal, same number from same set
	*/
	Matrix<int> m_map;
public:
	Board(int n, int m);
	// Add an obstacle
	// Return false if already something there
	bool addObs(Point obs);
	// Add a terminal set
	bool addTermset(
		const Vector<Point> &termset
	);
	const Vector<Point> &obss() const{
		return m_obss;
	}
	const Vector<Vector<Point>> &termsets(
	) const{
		return m_termsets;
	}
	const Matrix<int> &map(
	) const{
		return m_map;
	}
	int n() const{
		return m_n;
	}
	int m() const{
		return m_m;
	}
};

OStream &operator <<(
	OStream &ost, const Board &board
);

#endif // _BOARD_H
