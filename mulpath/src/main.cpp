#include <cstdio>
#include "include/columngen.h"
using namespace std;

// Straightforward console input

int main(){
	int n, m; cin >> n >> m;
	Board board(n, m);
	for(;;){
		String s;
		cin >> s;
		if(s[0] == 'R') break;
		else if(s[0] == 'T'){
			int t; cin >> t;
			Vector<Point> vec;
			while(t--){
				int x, y; cin >> x >> y;
				vec.push_back(Point(x, y));
			}
			board.addTermset(vec);
		}else if(s[0] == 'O'){
			int x, y; cin >> x >> y;
			board.addObs(Point(x, y));
		}
	}
	int T; cin >> T;
	String mode; cin >> mode;
	ColumnGen cg(board, mode);
	Vector<Tree *> trees = cg.route(T);
	cg.output(trees);
	Matrix<int> ans = board.map();
	for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	if(ans[i][j] > 0)
		ans[i][j] = 0;
	for(
		int idx = 1;
		idx <= (int) trees.size();
		idx++
	)
	if(trees[idx - 1])
		for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
		if(trees[idx - 1]->m_tree[i][j])
			ans[i][j] = idx;
	cout << ans;
	return 0;
}
