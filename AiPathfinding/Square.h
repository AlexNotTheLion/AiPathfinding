#pragma once
#include <iostream>
#include <glm.hpp>
#include <GL/glew.h>

class Square
{
private:
	glm::vec2 pos;
	float size;
	char id;

public:
	void Draw();
	void setPos(float _x, float _y) { pos = glm::vec2(_x, _y); };
	void setSize(float _size) { size = _size; };
	void setId(char _type) { id = _type; };
};