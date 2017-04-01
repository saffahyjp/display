#ifndef _MATRIX_H
#define _MATRIX_H

#include "global.h"

template <class T>
class Matrix: public Vector<Vector<T>>{
public:
	using Vector<Vector<T>>::resize;
	using Vector<Vector<T>>::operator[];
	void resize(int n, int m){
		this->resize(n);
		for(auto &row: *this)
			row.resize(m);
	}
	T &operator[](Pair<int, int> pii){
		return (*this)[pii.first][pii.second];
	}
	const T &operator[](
		Pair<int, int> pii
	) const{
		return (*this)[pii.first][pii.second];
	}
};

#endif // _MATRIX_H
