#pragma once
#include <string>
#include <chrono>
#include <list>

class Block
{
private:
	std::string name;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;

	std::list<Block*> children;
	Block* parent;

public:
	Block(const std::string& _name, Block* _parent = nullptr);
	~Block();

	void End();
	void Dump(int n = 0) const;

	double GetSeconds() const;
	std::string GetName() const { return name; }
	Block* GetParent() const { return parent; }

	Block* AddChild(const std::string& _name);
};