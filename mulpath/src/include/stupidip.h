#ifndef _STUPIDIP_H
#define _STUPIDIP_H

#include "router.h"

class StupidIP: public Router{
public:
	StupidIP(const Board &board): Router(board){
	}
	virtual ~StupidIP(){
	}
	virtual Vector<Tree *> route(int) const;
};

#endif // _STUPIDIP_H
