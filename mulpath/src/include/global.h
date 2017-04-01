/*
	global.h
	
	Include standard headers and define types.
*/

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "gurobi_c++.h"

#include <iostream>
using std::cout;

#include <fstream>
typedef std::ostream OStream;
typedef std::ifstream IFStream;
typedef std::ofstream OFStream;

#include <vector>
using std::vector;
#define Vector vector

#include <utility>
using std::pair;
#define Pair pair
typedef Pair<int, int> Point;

#include <algorithm>
using std::sort;
using std::reverse;
using std::min;
using std::max;

#include <string>
typedef std::string String;

#include <cstring>
#include <cstdlib>
#include <ctime>

template <class T>
inline OStream &operator <<(
	OStream &ost, const Vector<T> &vec
){
	for(const T &elem: vec)
		ost << elem << "\n";
	return ost;
}

template <>
inline OStream &operator <<(
	OStream &ost, const Vector<int> &vec
){
	for(const int &elem: vec)
		ost << elem << "\t";
	return ost;
}

template <>
inline OStream &operator <<(
	OStream &ost, const Vector<double> &vec
){
	for(const double &elem: vec)
		ost << elem << "\t";
	return ost;
}

template <>
inline OStream &operator <<(
	OStream &ost, const Vector<bool> &vec
){
	for(const bool &elem: vec)
		ost << (int) elem;
	return ost;
}

#endif // _GLOBAL_H
