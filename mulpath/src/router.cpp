#include "include/router.h"

/*
	ANSI character coding is used here, and may
	not have expected output on platforms other
	then Windows.
*/

void Router::output(
	const Vector<Tree *> &result,
	OStream &ost
) const{
	// String constants
	String strObs = "¡Á";
	String strEmpty = "¡¡";
	// 1 left, 2 right, 4 up, 8 down
	String strDir[16] = {
		"£¿", "£¿", "£¿", "©¥",
		"£¿", "©¿", "©»", "©ß",
		"£¿", "©·", "©³", "©×",
		"©§", "©Ï", "©Ç", "©ï"
	};
	String strNum[100] = {
		"£°", "£±", "£²", "£³", "£´",
		"£µ", "£¶", "£·", "£¸", "£¹"
	};
	for(int i = 10; i < 100; i++)
		strNum[i] = String()
			+ (char) (i / 10 + '0')
			+ (char) (i % 10 + '0');
	// Calculate total cost
	int cnt = 0, cost = 0;
	for(auto tree: result)
		if(tree){
			++cnt; cost += tree->cost();
		}
	int n = this->m_board.n();
	int m = this->m_board.m();
	int t = result.size();
	const Matrix<int> &map
		= this->m_board.map();
	// Plot original map
	Matrix<String> mapOri, mapRoute;
	mapOri.resize(n, m);
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		if(map[i][j] == 0)
			mapOri[i][j] = strEmpty;
		else if(map[i][j] == -1)
			mapOri[i][j] = strObs;
		else
			mapOri[i][j] = strNum[map[i][j] - 1];
	ost << "Original map:\n";
	ost << "©°";
	for(const auto &col: mapOri[0])
		ost << "©¨";
	ost << "©´\n";
	for(const auto &row: mapOri){
		ost << "©ª";
		for(const auto &col: row)
			ost << col;
		ost << "©ª\n";
	}
	ost << "©¸";
	for(const auto &col: mapOri[0])
		ost << "©¨";
	ost << "©¼\n";
	ost << "Successfully route " << cnt
		<< " sets of terminals with length "
		<< cost << "\n";
	mapRoute.resize(n, m);
	// Plot routed map
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
		if(map[i][j] == 0)
			mapRoute[i][j] = strEmpty;
		else if(map[i][j] == -1)
			mapRoute[i][j] = strObs;
		else if(result[map[i][j] - 1])
			mapRoute[i][j] =
				strNum[map[i][j] - 1];
		else
			mapRoute[i][j] = strEmpty;
	for(int idx = 1; idx <= t; idx++){
		Tree *tree = result[idx - 1];
		if(!tree)
			continue;
		for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++){
			if(!tree->m_tree[i][j])
				continue;
			if(map[i][j] == idx)
				continue;
			int sid = 0;
			// For each grid, find adjacent tree
			// grids and plot the line.
			// 1 left, 2 right, 4 up, 8 down
			if(i > 0 && tree->m_tree[i - 1][j])
				sid += 4;
			if(i < n - 1
				&& tree->m_tree[i + 1][j])
				sid += 8;
			if(j > 0 && tree->m_tree[i][j - 1])
				sid += 1;
			if(j < m - 1
				&& tree->m_tree[i][j + 1])
				sid += 2;
			mapRoute[i][j] = strDir[sid];
		}
	}
	ost << "Routed map:\n";
	ost << "©°";
	for(const auto &col: mapRoute[0])
		ost << "©¨";
	ost << "©´\n";
	for(const auto &row: mapRoute){
		ost << "©ª";
		for(const auto &col: row)
			ost << col;
		ost << "©ª\n";
	}
	ost << "©¸";
	for(const auto &col: mapRoute[0])
		ost << "©¨";
	ost << "©¼\n";
	ost.flush();
}
