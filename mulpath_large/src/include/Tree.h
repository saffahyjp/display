#ifndef TREE_H
#define TREE_H

#include "global.h"
#include "TerminalSet.h"

class Tree
{
public:
	Tree(){}
	Tree(const TerminalSet *);
	const TerminalSet *terminalSet;
	BitMatrix map;
	int length;
	void computeLength()
	{
		length = map.popcnt();
	}
	friend bool operator ==(const Tree &lhs, const Tree &rhs);
};

#endif