#include "../include/CleverOptimize.h"
#include <list>
#include "../include/global.h"

using namespace std;

static const int X[] = {0,-1,0,1,0};
static const int Y[] = {0,0,1,0,-1};

Solution CleverOptimize::optimize(const Solution& solution) const 
{
	cout << "paole\n";
	cout.flush();
	Solution temp(solution);
	for (unsigned i = 1;i < temp.trees.size();++i)
	{
		if(temp.trees[i] == NULL)
			continue;
		for (unsigned j = 0;j < temp.trees[i]->terminalSet->points.size();++j)
		{
			int degree = 0,lastDirection = 0;
			bool noNeedToChange = true;
			Point currentPoint(temp.trees[i]->terminalSet->points[j].x,temp.trees[i]->terminalSet->points[j].y);
			for (int k = 1;k <= 4; ++k)
			{
				int currentx = X[k] + currentPoint.x;
				int currenty = Y[k] + currentPoint.y;
				if (currenty >= 0 && currenty < temp.trees[i]->map.width && currentx >= 0 && currentx < temp.trees[i]->map.height)
				{
					if (temp.trees[i]->map.get(currentx,currenty) == 1) 
					{
						vector<Point>  resetedPoints;
						lastDirection = k;
						Point tempPoint(currentx,currenty);
						while (degree <= 2)
						{
							for (int u = 1;u <= 4; ++u)
							{
								currentx = X[u] + tempPoint.x;
								currenty = Y[u] + tempPoint.y;
								if (currenty >= 0 && currenty < temp.trees[i]->map.width && currentx >= 0 && currentx < temp.trees[i]->map.height)
								{
									if (temp.trees[i]->map.get(currentx,currenty) == 1 
									&& !(temp.board->map[currentx][currenty] > 0)) 
									{
										if (u != getLastDirection(lastDirection))
										{
											degree++;
										}
									}
								}
							}
							if (degree >= 2)
							{
								noNeedToChange = false;
								break;
							}
							else if (degree == 0) break;
							else
							{
								if (temp.board->map[tempPoint.x][tempPoint.y] != 0) break;
								temp.trees[i]->map.reset(tempPoint.x,tempPoint.y);
								temp.computeMap();
								resetedPoints.push_back(tempPoint);
								degree = 0;
								for (int u = 1;u <= 4; ++u)
								{
									currentx = X[u] + tempPoint.x;
									currenty = Y[u] + tempPoint.y;
									if (currenty >= 0 && currenty < temp.trees[i]->map.width && currentx >= 0 && currentx < temp.trees[i]->map.height)
									{
										if (temp.trees[i]->map.get(currentx,currenty) == 1) 
										{
											if (u != getLastDirection(lastDirection))
											{
												lastDirection = u;
												break;
											}
										}
									}
								}
								tempPoint.x += X[lastDirection];
								tempPoint.y += Y[lastDirection];
							}
						}
						if (!noNeedToChange)
						{
							BitMatrix _map(temp.trees[i]->map.height,temp.trees[i]->map.width);
							getMap(_map,tempPoint,temp.trees[i]->map);
							bfs(temp.trees[i]->terminalSet->points[j],temp.trees[i]->map,_map,temp);
							temp.computeMap();
						}
						else
						{
							for (unsigned int u = 0;u < resetedPoints.size();++u)
								temp.trees[i]->map.set(resetedPoints[u].x,resetedPoints[u].y);
							temp.computeMap();
						}
					}
				}
			}	
		}
	}
	return temp;
}

int CleverOptimize::getLastDirection(const int k) const
{
	if (k == 1) return 3;
	if (k == 2) return 4;
	if (k == 3) return 1;
	if (k == 4) return 2;
	return 0;
}

void CleverOptimize::getMap(BitMatrix& _map,const Point& start,BitMatrix& _originMap) const
{
	Matrix<int> visited;
	visited.resize(_map.height,_map.width);
	list<Point> waitingPoints;
	waitingPoints.push_back(start);
	visited[start.x][start.y] = 1;
	_map.set(start.x,start.y);
	while (!waitingPoints.empty())
	{
		Point currentPoint = waitingPoints.front();
		waitingPoints.pop_front();
		for (int i = 1;i <= 4;++i)
		{
			int currentx = X[i] + currentPoint.x;
			int currenty = Y[i] + currentPoint.y;
			if (currenty >= 0 && currenty < _map.width && currentx >= 0 && currentx < _map.height)
			{
				if (!visited[currentx][currenty] && _originMap.get(currentx,currenty) == 1)
				{
					waitingPoints.push_back(Point(currentx,currenty));
					_map.set(currentx,currenty);
					visited[currentx][currenty] = 1;
				}
			}
		}
	}
}

void CleverOptimize::bfs(const Point& start,BitMatrix& _map,BitMatrix& _computedMap,const Solution& temp) const
{
	Matrix<int> visited;
	list<Point> waitingPoints;
	waitingPoints.push_back(start);
	visited.resize(_map.height,_map.width);
	visited[start.x][start.y] = 1;
	bool find = false;
	int step = 1;
	while (!waitingPoints.empty() && !find)
	{
		Point currentPoint = waitingPoints.front();
		waitingPoints.pop_front();
		step = visited[currentPoint.x][currentPoint.y] + 1;
		for (int i = 1;i <= 4;++i)
		{
			int currentx = X[i] + currentPoint.x;
			int currenty = Y[i] + currentPoint.y;
				if (currenty >= 0 && currenty < 
					_map.width && currentx >= 0 && currentx < _map.height)
				if (!visited[currentx][currenty] && !(temp.board->map[currentx][currenty] == -1) 
					&& ((_map.get(currentx,currenty)^(temp.map[currentx][currenty] > 0)) == 0))
				{
					visited[currentx][currenty] = step;
					waitingPoints.push_back(Point(currentx,currenty));
					if (_computedMap.get(currentx,currenty) == 1)
					{
						find  = true;
						backtrace(start,_map,visited,Point(currentx,currenty),step);
						find = true;
						break;
					}
				}
		}
	}
}

void CleverOptimize::backtrace(const Point& start,BitMatrix& map,Matrix<int>& visited,const Point& end,int step) const
{
	Point currentPoint = end;
	while (currentPoint != start)
	{
		step = step - 1;
		for (int i = 1;i <= 4;++i)
		{
			int currentx = X[i] + currentPoint.x;
			int currenty = Y[i] + currentPoint.y;
			if (currenty >= 0 && currenty < map.width && currentx >= 0 && currentx < map.height)
			{
				if (visited[currentx][currenty] == step)
				{
					currentPoint = Point(currentx,currenty);
					map.set(currentx,currenty);
					break;
				}
			}
		}
	}
}

OptimizeStrategy *CleverOptimize::clone() const
{
	return new CleverOptimize(*this);
}

