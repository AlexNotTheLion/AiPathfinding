#include "player.h"
#include <SDL2/SDL.h>

#include <sstream>

void player::start(int win_w, int win_h, float t_size, float t_space, map map)
{
	m_map = map;
	height = map.getSize().m_y;
	width = map.getSize().m_x;

	playerPos.x = (width / 2 - 1) - map.getPlayerStart().x;
	playerPos.y = (height / 2 - 1) - map.getPlayerStart().y;
	playerList = map.getPlayerStart();
	tileList = map.getTileList();

	goalPos = map.getGoalPos();

	windowWidth = win_w;
	windowHeight = win_h;

	tileSize = t_size;
	tileSpace = t_space;

	entity.setSize(tileSize);
}

void player::update(geneticAlgorithm &ga, bool &quit)//draw with best fitness
{
	path = ga.getPath();
	entity.setId('p');
	entity.setPos((windowWidth / 2 - tileSize / 2) - (tileSize + tileSpace) * playerPos.x - (tileSize + tileSpace) / 2, (windowHeight / 2 - tileSize / 2) + (tileSize + tileSpace) * playerPos.y + (tileSize + tileSpace) / 2);
	entity.Draw();
	
		for (int j = 0; j < path[0].first.size(); j++)
		{
			move = path[0].first[j];
			openMoves(playerList);
			if (move == "00")//right
			{
				if (avaliableMoves[3] == 1)
				{
					playerPos.x -= 1.0f;
					playerList.x += 1.0f;
				}
			}
			else if (move == "01")//left
			{
				if (avaliableMoves[0] == 1)
				{
					playerPos.x += 1.0f;
					playerList.x -= 1.0f;
				}

			}
			else if (move == "10")//up
			{
				if (avaliableMoves[1] == 1)
				{
					playerPos.y += 1.0f;
					playerList.y -= 1.0f;
				}

			}
			else if (move == "11")//down
			{
				if (avaliableMoves[2] == 1)
				{
					playerPos.y -= 1.0f;
					playerList.y += 1.0f;
				}

			}
			setPos(playerPos);

			if (j == path[0].first.size()-1)
				entity.setId('e');
			else
				entity.setId('p');
			entity.setPos((windowWidth / 2 - tileSize / 2) - (tileSize + tileSpace) * playerPos.x - (tileSize + tileSpace) / 2, (windowHeight / 2 - tileSize / 2) + (tileSize + tileSpace) * playerPos.y + (tileSize + tileSpace) / 2);
			entity.Draw();
		}

		//spit out fittest population item
		std::cout << " max fittness : " << path[0].second << " with : ";
		for (auto i : path[0].first)
		{
			std::cout << i;
		}
		std::cout << "\n";
		if (path[0].second == 0)
		{
			quit = true;
		}

		resetPlayer();		
}

void player::openMoves(glm::vec2 _playerPos)
{
	for (int i = 0; i < 4; i++)
	{
		if (_playerPos.x + rowNum[i] < 0 || _playerPos.y + colNum[i] < 0 || _playerPos.x + rowNum[i] >= width || _playerPos.y + colNum[i] >= height)
		{
			avaliableMoves[i] = 0;
			continue;
		}

		if (tileList[_playerPos.x + rowNum[i] + (_playerPos.y + colNum[i]) * width] != 1)
		{
			avaliableMoves[i] = 1;
			continue;
		}
		avaliableMoves[i] = 0;
	}
}

// http://www.cplusplus.com/forum/beginner/101334/ // int to char

void player::resetPlayer()
{
	playerPos.x = (width / 2 - 1) - m_map.getPlayerStart().x;
	playerPos.y = (height / 2 - 1) - m_map.getPlayerStart().y;
	playerList.x = m_map.getPlayerStart().x;
	playerList.y = m_map.getPlayerStart().y;
	//std::cout << "playerReset\n";
}

