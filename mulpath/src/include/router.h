#ifndef _ROUTER_H
#define _ROUTER_H

#include "tree.h"

class Router{
public:
	const Board &m_board;
	Router(const Board &board): m_board(board){
	}
	virtual ~Router(){
	}
	virtual Vector<Tree *> route(int) const = 0;
	void output(
		const Vector<Tree *> &result,
		OStream &ost = cout
	) const;
};

#endif // _ROUTER_H
