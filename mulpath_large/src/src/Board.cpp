#include "../include/Board.h"
#include "../include/TerminalSet.h"

Board::~Board()
{
	for(auto pointer: terminalSets)
		delete pointer;
}

Board::Board(const Board& _board)
{
	this->height = _board.height;
	this->width = _board.width;
	this->map = _board.map;
	this->blocks = _board.blocks;
	for (unsigned int i = 0;i < _board.terminalSets.size();++i)
		if (_board.terminalSets[i] == NULL)
			this->terminalSets.push_back(NULL);
		else
		{
			TerminalSet* temp = new TerminalSet(_board.terminalSets[i]->id,this);
			for (unsigned int j = 0;j < _board.terminalSets[i]->points.size();++j)
				temp->AddPoint(_board.terminalSets[i]->points[j]);
			this->terminalSets.push_back(temp);
		}
			
}

void Board::input(int mode, IStream & ifs)
{
	if (mode == 0)
	{
		//input the size of the board
		ifs>>height>>width;
		map.resize(height,width);
		//input every element of the board
		int maxindex = 0;
		for (int i = 0;i < height; ++i)
			for (int j = 0; j < width; ++j)
			{
				ifs>>map[i][j];
				maxindex = max(maxindex,map[i][j]);
			}
	terminalSets.resize(maxindex+1);
	for (int i = 0;i < (int)terminalSets.size(); ++i)
		terminalSets[i] = new TerminalSet(i,this);
	for (int i = 0;i < height; ++i)
		for (int j = 0; j < width; ++j)
		{
			if (map[i][j] == -1) blocks.push_back(Point(i,j));
			else if (map[i][j] > 0)
			terminalSets[map[i][j]]->AddPoint(Point(i,j));
		}	
	}
	else
	{
		int id;
		ifs>>height>>width;
		map.resize(height,width);
		TerminalSet *terminal0 = new TerminalSet(0,this);
		terminalSets.push_back(terminal0);
		while (ifs>>id)
		{
			int x,y;
			int n;
			ifs>>n;
			if (id > 0)
			{
				TerminalSet *terminal = new TerminalSet(id,this);
				for (int i = 1;i <= n; ++i)
				{
					ifs>>x>>y;
					terminal->AddPoint(Point(x,y));
					map[x][y] = id;
				}
				terminalSets.push_back(terminal);
			}
			else if (id == -1)
			{
				for (int i = 1;i <=n; ++i)
				{
					ifs>>x>>y;
					blocks.push_back(Point(x,y));
					map[x][y] = -1;
				}
			}
				
		}
	}
}

void Board::output(int mode, OStream &ofs) const
{
	if (mode == 0)
	{
		ofs<<height<<' '<<width<<'\n';
		for (int i = 0;i < height; ++i)
		{
			for (int j = 0;j < width; ++j)
				ofs<<map[i][j]<<' ';
			ofs<<'\n';
		}
	}
	else
	{
		ofs<<height<<' '<<width<<'\n';
		for (int i = 1;i < (int)terminalSets.size(); ++i)
		{
			ofs<<i<<' '<<terminalSets[i]->points.size()<<'\n';
			for (int j = 0;j < (int)terminalSets[i]->points.size(); ++j)
				ofs<<terminalSets[i]->points[j].x<<' '<<terminalSets[i]->points[j].y<<' '<<'\n';
		}
		ofs<<-1<<' '<<blocks.size()<<'\n';
		for (int i = 0;i < (int)blocks.size(); ++i)
			ofs<<blocks[i].x<<' '<<blocks[i].y<<'\n';
	}
}