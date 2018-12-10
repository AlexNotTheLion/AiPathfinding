#pragma once
#include "Square.h"
#include "map.h"
#include "geneticAlgorithm.h"
#include <chrono>
#include <random>
#include <algorithm>
#include <queue>

class player
{
public:
	void start(int win_w, int win_h, float t_size, float t_space, map map);
	void update(geneticAlgorithm &ga, bool &quit);
	glm::vec2 getPos() { return playerPos; }
	glm::vec2 getGoalPos() { return goalPos; }
	void setPos(glm::vec2 pos) { playerPos = pos; }
	void resetPlayer();
	void setPath(std::string _move) { move = _move; }
	void openMoves(glm::vec2 _playerPos);
	std::vector<int> getMoves() { return avaliableMoves; }

private:
	map m_map;
	float height;
	float width;
	int windowWidth, windowHeight;
	float tileSize;
	float tileSpace;
	Square entity;
	std::vector<int> tileList;
	glm::vec2 playerPos = glm::vec2(0.0f);
	glm::vec2 goalPos = glm::vec2(0.0f);
	glm::vec2 playerList = glm::vec2(0.0f);

	std::vector<std::pair<std::vector<std::string>, int>> path;
	std::string move = "";

	int rowNum[4] = { -1,0,0,1 };
	int colNum[4] = { 0,-1,1,0 };
	std::vector<int> avaliableMoves = { 0,0,0,0 };//left up down right
};

