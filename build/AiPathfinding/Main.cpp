#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <exception>
#include <Windows.h>
#include <cmath>
#include "map.h"
#include "Square.h"
#include "player.h"
#include "geneticAlgorithm.h"

#include <shellapi.h>

int windowWidth = 1920;
int windowHeight = 1080;

int main(int argc, char *argv[])
{
	map map;

	map.loadMap();

	float size = 40.0f;//size of tiles
	float space = 3.0f;//size of space between tiles
	int gen = 1;
	map.start(windowWidth, windowHeight, size, space);

	if (!map.fileLoaded())
	{
		std::cout << "map couldnt load\n";
		system("pause");
		return 0;
	}



	player player;
	player.start(windowWidth, windowHeight, size, space, map);

	geneticAlgorithm ga;
	ga.GenPop(50);
	ga.start(map);


	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception();
	}

	SDL_Window *window = SDL_CreateWindow("Pathfinding", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL /*| SDL_WINDOW_ALWAYS_ON_TOP*/ | SDL_WINDOW_RESIZABLE);

	if (!SDL_GL_CreateContext(window) || glewInit() != GLEW_OK)// create the window and ensure glew initalises
	{
		throw std::exception();
	}

	bool quit = false;
	bool loop = false;
	/*Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	float deltaTime;*/

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	glMatrixMode(GL_MODELVIEW);

	//float height = map.getSize().m_y;
	//float width = map.getSize().m_x;

	glm::vec2 playerPos = player.getPos();
	auto start = std::chrono::system_clock::now();
	while (!quit)
	{
		SDL_Event event = { 0 };
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		/*LAST = NOW;//get delta time for lerp
		NOW = SDL_GetPerformanceCounter();

		deltaTime = ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		deltaTime *= 0.01f;*/


		glClear(GL_COLOR_BUFFER_BIT);

		map.update();
		ga.calcFitness();
		ga.selection();
		std::cout << "gen: " << gen;
		player.update(ga, quit);//draw best only

		if (!quit)//double check to see if player is at goal
		{
			ga.crossover();
			gen++;

			ga.add();

		}


		SDL_GL_SwapWindow(window);
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time taken: " << diff.count() << "\n";
	system("pause");

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}