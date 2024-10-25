#pragma once

class Collision;

class Collidable
{
private:

public:
	virtual void OnCollision(Collision* collision) = 0;
};