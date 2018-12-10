#include "map.h"
#include <queue>


void map::loadMap()
{
	// ref : https://docs.microsoft.com/en-us/windows/desktop/api/commdlg/ns-commdlg-tagofna  // for opening file 21/11/18

	HWND hwnd = 0;
	std::string path(MAX_PATH, '\0');
	OPENFILENAME file = { sizeof(OPENFILENAME) };
	file.hwndOwner = hwnd;
	file.lpstrFilter = "txt\0 * .TXT\0";//only accept text files
	file.lpstrFile = &path[0];//file path
	file.lpstrTitle = "Open map";//name of window
	file.nMaxFile = MAX_PATH;//max path length
	file.Flags = OFN_FILEMUSTEXIST;//cant be deleted must exist

	if (GetOpenFileName(&file))//open file window and get file
	{
		success = true;
		path.resize(strlen(path.c_str()));//get file location
		mapPath = path.c_str();//store path
	}
	else
	{
		success = false;
		std::cout << "failed to open file\n";//if failed close 
		return;
	}

	// http://www.cplusplus.com/doc/tutorial/files/ reading a txt file 22/11/18
	// reading utf 16 https://stackoverflow.com/questions/42404126/c-correctly-read-files-whose-unicode-characters-might-be-larger-than-a-byte
	// https://en.wikipedia.org/wiki/Byte_order_mark


	std::wifstream startFile(mapPath, std::ios::binary);//open file and test start character to test encoding
	wchar_t start;
	startFile.get(start);
	startFile.close();

	std::wifstream openFile(mapPath, std::ios::binary);//open file using correct utf reading
	if ((int)start == 255 || (int)start == 254)//starts with utf16 char
		openFile.imbue(std::locale(openFile.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));//utf16
	else
		openFile.imbue(std::locale(openFile.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));//utf8 and ANSI

	std::string temp;

	for (wchar_t c; openFile.get(c); )
	{
		if (c != 32 || c != ' ')
			temp.push_back((char)c);
		else if (c == 32 || c == ' ')
		{
			tileTypes.emplace_back(std::stoi(temp));//if space is detected as a character store the current string as a int into a list 
			temp.erase(); // clear the temp string
		}
	}
	tileTypes.emplace_back(std::stoi(temp));//store the final character
	temp.erase();


	this->setSize(tileTypes[0], tileTypes[1]);//set map size
	height = size.m_y;//store height from pos 0
	width = size.m_x;//store width from pos 1
	tileTypes.erase(tileTypes.begin(), tileTypes.begin() + 2);//clear pos 0 and 1 of the tiletypes leaving only the map

	if (height == 0 || width == 0)
	{
		success = false;
		std::cout << "map size is 0\n";//if failed close 
		return;
	}

	for (int y = 0; y < height; y++)//get player start pos
	{
		for (int x = 0; x < width; x++)
		{
			int type = tileTypes[x + y * width];
			if (type == 2)
				playerStartPos = glm::vec2(x, y);
			if (type == 3)
				goalPos = glm::vec2(x, y);
		}
	}
	distToGoal = findPath(tileTypes, width, height, playerStartPos, goalPos);
	if (distToGoal <= 0)
	{
		success = false;
		std::cout << "can't find path to goal \n";//if failed close 
		return;
	}
	std::cout << "dist to goal: " << distToGoal << "\n";
}

void map::update()
{
	for (int y = 0; y < height; y++)//draw map
	{
		for (int x = 0; x < width; x++)
		{
			int type = tileTypes[x + y * width];
			switch (type)
			{
			case 0:
				tile.setId('f');//floor
				break;
			case 1:
				tile.setId('w');//wall
				break;
			case 2:
				tile.setId('f');//player start
				break;
			case 3:
				tile.setId('g');//goal
				break;
			}
			tile.setPos(((windowWidth / 2 - tileSize / 2) + (tileSize + tileSpace) * x) - ((tileSize + tileSpace) * (width - 1)) / 2, ((windowHeight / 2 - tileSize / 2) - (tileSize + tileSpace) * y) + ((tileSize + tileSpace) * (height - 1)) / 2);
			tile.Draw();
		}
	}
}

int rowNum[] = { -1,0,0,1 };//set check direction from current tile
int colNum[] = { 0,-1,1,0 };

//https://www.geeksforgeeks.org/shortest-path-in-a-binary-maze/
int map::findPath(std::vector<int> _tileTypes, int width, int height, glm::vec2 src, glm::vec2 goal)//return min distance to goal using A*
{
	if (_tileTypes[src.x + src.y*width] != 2 || _tileTypes[goal.x + goal.y*width] != 3)//if the goal tile and player tile do not match correct id return failed
		return -1;

	std::vector<std::vector<bool>> visited(width);//create a list of positions to be visted
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			visited[x].push_back(false);//make all positions false
		}
	}
	visited[src.x][src.y] = true;//make the source tile visited

	std::queue<queueNode> list;//make a list for the tiles
	Point srcPoint;
	srcPoint.x = src.x;
	srcPoint.y = src.y;
	queueNode s = { srcPoint,0 };//source point
	list.push(s);

	while (!list.empty())
	{
		queueNode current = list.front();//get item at front of list
		Point pt = current.pt;

		if (pt.x == goal.x && pt.y == goal.y)//if current point equals the goal point
			return current.dist;

		list.pop();
		for (int i = 0; i < 4; i++)//check to see if it can move in a direction
		{
			int row = pt.x + rowNum[i];//set point to up down left or right of current point to see if its been visited
			int col = pt.y + colNum[i];

			if (isValid(row, col) && (_tileTypes[row + col * width] == 0 || _tileTypes[row + col * width] == 3) && !visited[row][col])//check it can move there
			{
				visited[row][col] = true;//if it can make visited true
				queueNode adjCell = { {row,col},current.dist + 1 };//calculate the distance from the cell it moved to
				list.push(adjCell);//store the new cell in the list
			}
		}
	}
	//no path found
	return -1;
}

int map::newPath(std::vector<int> _tileTypes, int width, int height, glm::vec2 src, glm::vec2 goal)//return min distance to goal using A*
{
	std::vector<std::vector<bool>> visited(width);//create a list of positions to be visted
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			visited[x].push_back(false);//make all positions false
		}
	}
	visited[src.x][src.y] = true;//make the source tile visited

	std::queue<queueNode> list;//make a list for the tiles
	Point srcPoint;
	srcPoint.x = src.x;
	srcPoint.y = src.y;
	queueNode s = { srcPoint,0 };//source point
	list.push(s);

	while (!list.empty())
	{
		queueNode current = list.front();//get item at front of list
		Point pt = current.pt;

		if (pt.x == goal.x && pt.y == goal.y)//if current point equals the goal point
			return current.dist;

		list.pop();
		for (int i = 0; i < 4; i++)//check to see if it can move in a direction
		{
			int row = pt.x + rowNum[i];//set point to up down left or right of current point to see if its been visited
			int col = pt.y + colNum[i];

			if (isValid(row, col) && (_tileTypes[row + col * width] == 0 || _tileTypes[row + col * width] == 3) && !visited[row][col])//check it can move there
			{
				visited[row][col] = true;//if it can make visited true
				queueNode adjCell = { {row,col},current.dist + 1 };//calculate the distance from the cell it moved to
				list.push(adjCell);//store the new cell in the list
			}
		}
	}
	//no path found
	return -1;
}