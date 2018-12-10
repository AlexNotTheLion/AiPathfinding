#include "geneticAlgorithm.h"

void geneticAlgorithm::start(map m_map)
{
	_map = m_map;
	_playerStartPos = m_map.getPlayerStart();
	_goalPos = m_map.getGoalPos();
	board = m_map.getTileList();
	width = m_map.getSize().m_x;
	height = m_map.getSize().m_y;
}

void geneticAlgorithm::GenPop(int size)
{
	if (population.size() != size)
		population.resize(size);

	std::string temp;
	for (int i = 0; i < population.size(); i++)
	{
		population[i].first.resize(stringLen);
		population[i].second = INT_MAX;
		for (int j = 0; j < stringLen; j++)
		{
			temp.push_back(dist(1) + '0');
			temp.push_back(dist(1) + '0');
			population[i].first[j].append(temp);
			temp.clear();
		}
	}
}

void geneticAlgorithm::selection()
{
	std::sort(population.begin(), population.end(), [](auto &a, auto &b) { return a.second < b.second; });

	parents.clear();

	parents.push_back(population[0]);
	parents.push_back(population[1]);
}

void geneticAlgorithm::crossover()
{
	for (int i = 0; i < population.size(); i++)
	{
		for (int j = 0; j < population[i].first.size(); j++)
		{
			int temp = dist(parents.size() - 1);
			population[i].first[j] = parents[temp].first[j];

			if (dist(1000) < mutateRate)
			{
				std::string temp;
				temp.push_back(dist(1) + '0');
				temp.push_back(dist(1) + '0');
				population[i].first[j] = temp;
				temp.clear();
			}
		}
	}
}

void geneticAlgorithm::add()
{
	std::string temp;
	for (int i = 0; i < population.size(); i++)
	{
		population[i].first.resize(stringLen);

		temp.push_back(dist(1) + '0');
		temp.push_back(dist(1) + '0');
		population[i].first.emplace_back(temp);
		temp.clear();
	}
	stringLen++;
}

void geneticAlgorithm::calcFitness()
{
	//not just about where it ended up, but how close it got to the goal in as smallest distance as possible
	//dist from goal is distance not displacement so every move increases the distance even if the player ends up where it started
	//fitness at each step + dist
	//if fitness < currentMinFit. currentMinFit to fitness
	//after all moves are complete set entity fitness to currentMinFit
	//fitness function to include distance travelled, if new fitness is smaller than previous then change, check at every moveb 

	for (int i = 0; i < population.size(); i++)
	{
		_entityPos = _playerStartPos;
		moveCount = 0;

		for (int j = 0; j < population[i].first.size(); j++)
		{
			moveCount++;
			move = population[i].first[j];
			canMove(_entityPos);

			if (move == "00")//right
			{
				if (avaliableMoves[3] == 1)
				{
					_entityPos.x += 1.0f;
				}

			}
			else if (move == "01")//left
			{
				if (avaliableMoves[0] == 1)
				{
					_entityPos.x -= 1.0f;
				}

			}
			else if (move == "10")//up
			{
				if (avaliableMoves[1] == 1)
				{
					_entityPos.y -= 1.0f;
				}

			}
			else if (move == "11")//down
			{
				if (avaliableMoves[2] == 1)
				{
					_entityPos.y += 1.0f;
				}
			}
			//cal fitness
		}
		population[i].second = _map.newPath(board, width, height, _entityPos, _goalPos);
	}
}

void geneticAlgorithm::canMove(glm::vec2 _player)
{
	for (int i = 0; i < 4; i++)
	{
		if (_player.x + rowNum[i] < 0 || _player.y + colNum[i] < 0 || _player.x + rowNum[i] >= width || _player.y + colNum[i] >= height)
		{
			avaliableMoves[i] = 0;
			continue;
		}

		if (board[_player.x + rowNum[i] + (_player.y + colNum[i]) * width] != 1)
		{
			avaliableMoves[i] = 1;
			continue;
		}
		avaliableMoves[i] = 0;
	}
}