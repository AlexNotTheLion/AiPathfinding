#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <glm.hpp>
#include "map.h"
class geneticAlgorithm
{
public:

	void GenPop(int size);
	void selection();
	void crossover();
	void calcFitness();
	void add();
	void start(map m_map);

	//getter
	std::vector<std::pair<std::vector<std::string>, int>> getPath() { return population; };
private:
	std::vector<std::pair<std::vector<std::string>, int>> population;
	std::vector<std::pair<std::vector<std::string>, int>> parents;
	glm::vec2 _playerStartPos = glm::vec2(0.0f);
	glm::vec2 _goalPos = glm::vec2(0.0f);
	glm::vec2 _entityPos = glm::vec2(0.0f);
	void canMove(glm::vec2 _player);
	int stringLen = 1;
	int moveCount;
	std::vector<int> avaliableMoves = { 0,0,0,0 };
	std::vector<int> board;
	map _map;
	int width;
	int height;

	std::string move;

	bool found = false;
	float mutateRate = 10.0f;
	int rowNum[4] = { -1,0,0,1 };
	int colNum[4] = { 0,-1,1,0 };

	int dist(int length)
	{
		unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
		std::mt19937 generator(seed);
		std::uniform_int_distribution<int> distribution(0, length);

		return distribution(generator);
	}
};

