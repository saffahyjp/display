#ifndef _COLUMNGEN_H
#define _COLUMNGEN_H

#include "router.h"

class ColumnGen: public Router{
public:
	String m_mode;
	ColumnGen(const Board &board, String mode):
	Router(board), m_mode(mode){
	}
	virtual ~ColumnGen(){
	}
	/*
		The core routing function.
	*/
	virtual Vector<Tree *> route(int) const;
protected:
	struct expandFinished{
	};
	/*
		Call Gurobi and solve the linear
		programming problem. mode = 0 for
		[0, 1], mode = 1 for {0, 1}.
	*/
	double solveLP(
		Vector<Vector<Tree>> &treesets,
		int mode,
		Matrix<double> &mapPi,
		Vector<double> &vecLambda,
		int ignoreIdx = -1
	) const;
	/*
		Given a tree and the Pi matrix,
		calculate the total weight of the tree.
	*/
	static double CalcTreeLambda(
		const Matrix<double> &mapPi,
		const Tree &tree, int n, int m
	);
	/*
		Try to generate at most "r" new trees of
		terminal set "idx" to "treeset".
	*/
	bool expand(
		const Matrix<double> &mapPi,
		double lambda,
		Vector<Tree> &treeset,
		int n, int m, int idx, int &r
	) const;
	/*
		Try to generate at most "r" new trees of
		terminal set "idx" from "tree" to
		"treeset".
	*/
	bool expand(
		const Matrix<double> &mapPi,
		double lambda,
		Vector<Tree> &treeset, const Tree &tree,
		int n, int m, int idx, int &r
	) const;
	/*
		Remove all non-cut grids of "tree",
		except terminals "idx" of "map",
		and except the grid ("exx", "exy").
	*/
	static void RemoveNonCuts(
		const Matrix<int> &map, int idx,
		Matrix<bool> &tree, int n, int m,
		int exx = -1, int exy = -1
	);
	/*
		Remove all non-cut grids of "tree",
		except terminals "idx" of "map",
		mark all removed grids in "visited",
		and return the total removed weight
		in regards of "mapW".
	*/
	static double RemoveNonCuts(
		Matrix<bool> &visited,
		const Matrix<int> &map, int idx,
		const Matrix<double> &mapW,
		Matrix<bool> &tree, int n, int m
	);
	/*
		Try to generate at most "r" new trees of
		terminal set "idx" from "base" (the
		broken pieces of a tree) to "treeset".
	*/
	bool expand(
		const Matrix<double> &mapW,
		double lambda,
		Vector<Tree> &treeset,
		const Matrix<bool> &base,
		int n, int m, int idx, int &r
	) const;
	/*
		Perform Dijkstra's algorithm from source
		"base" and in respect of weight "mapW".
	*/
	Matrix<Pair<double, Matrix<bool>>> dijkstra(
		const Matrix<bool> &base,
		const Matrix<double> &mapW,
		int n, int m, int idx
	) const;
	/*
		Try to generate and insert the best tree
		of terminal set "termset", "idx" in
		regards of "mapW".
	*/
	bool suggestTree(
		const Vector<Vector<Point>> &termsets,
		Vector<Vector<Tree>> &treesets,
		const Matrix<double> &mapW,
		int n, int m, int idx
	) const;
	/*
		Generate the best tree of terminal set
		"termset", "idx" in regards of "mapW".
	*/
	Tree suggestTree(
		const Vector<Point> &termset,
		const Matrix<double> &mapW,
		int n, int m, int idx
	) const;
};

#endif // _COLUMNGEN_H
