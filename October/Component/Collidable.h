#pragma once

struct Collision;

class Collidable
{
private:

public:
	virtual void OnCollision(Collision*) = 0;
};