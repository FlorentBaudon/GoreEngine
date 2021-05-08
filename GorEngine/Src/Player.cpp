#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Player.h"

using namespace glm;

Player::Player(vec2 position, vec2 worldForward, vec2 worldRight, float angle) 
{
	this->position = position;
	this->worldForward = worldForward;
	this->worldRight = worldRight;
	this->forward = worldForward;
	this->right = worldRight;
	this->angle = angle;

	this->refreshPlayerDatas();
}

void Player::turn(float angle) 
{
	this->angle += angle;
	this->refreshPlayerDatas();
	
}

void Player::move(vec2 direction) 
{
	if (length(direction) > 0)
	{
		vec2 d = vec4(direction, 0, 0) * this->rotationMatrix;
		this->position += d;
	}
}

void Player::refreshPlayerDatas()
{
	this->rotationMatrix = rotate(mat4(1.0f), this->angle, vec3(0.0f, 0.0f, 1.0f));

	this->forward = vec4(this->worldForward, 0, 0) * this->rotationMatrix;
	this->right = vec4(this->worldRight, 0, 0) * this->rotationMatrix;
}
