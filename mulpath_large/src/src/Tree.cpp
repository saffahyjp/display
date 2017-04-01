#include "../include/Tree.h"

Tree::Tree(const TerminalSet* terminalSet_)
{
	terminalSet = terminalSet_;
	map.resize(terminalSet->board->height, terminalSet->board->width);
	length = 0;
}

bool operator ==(const Tree &lhs, const Tree &rhs)
{
	return lhs.terminalSet == rhs.terminalSet && lhs.map == rhs.map;
}
