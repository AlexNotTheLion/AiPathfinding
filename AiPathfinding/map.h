#pragma once
#include "Square.h"
#include <vector>
#include <Windows.h>
#include <commdlg.h>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <glm.hpp>

class map
{
public:
	struct Size
	{
		int m_x;
		int m_y;
	};
	void loadMap();
	void update();
	int findPath(std::vector<int> _tileTypes, int width, int height, glm::vec2 src, glm::vec2 goal);
	int newPath(std::vector<int> _tileTypes, int width, int height, glm::vec2 src, glm::vec2 goal);

	//setters
	void setSize(int x, int y) { size.m_x = x; size.m_y = y; }

	//getters
	Size getSize() { return size; }
	bool fileLoaded() { return success; }
	std::vector<int>  getTileList() { return tileTypes; };
	glm::vec2 getPlayerStart() { return playerStartPos; }
	glm::vec2 getGoalPos() { return goalPos; }
	void start(int win_w, int win_h, float t_size, float t_space) { windowWidth = win_w; windowHeight = win_h; tileSize = t_size; tileSpace = t_space; tile.setSize(tileSize); }

private:
	std::vector<int> tileTypes;
	std::string mapData;
	std::string mapPath;
	glm::vec2 playerStartPos;
	glm::vec2 goalPos;
	Size size;
	Square tile;
	bool success = false;
	float tileSize;
	float tileSpace;
	float height, width;
	int temp;
	int windowWidth, windowHeight;
	int distToGoal;

	struct Point
	{
		int x;
		int y;
	};

	struct queueNode
	{
		Point pt;  // The cordinates of a cell 
		int dist;  // cell's distance of from the source 
	};
	bool isValid(int row, int col)
	{
		return (row >= 0) && (row < width) && (col >= 0) && (col < height);
	}
};