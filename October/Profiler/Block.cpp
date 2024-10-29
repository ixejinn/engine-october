#include "Block.h"

#include <iostream>

Block::Block(const std::string& _name, Block* _parent) : name(_name), parent(_parent)
{
	start = std::chrono::steady_clock::now();
}

Block::~Block()
{
	// Delete all the children
	for (auto it : children)
		delete it;

	children.clear();
}

void Block::End()
{
	end = std::chrono::steady_clock::now();
}

void Block::Dump(int n) const
{
	// Print correct amount of tabs
	for (int i = 0; i < n; i++)
		std::cout << "\t";

	// Print name and seconds
	std::cout << name << " in " << GetSeconds() << " seconds" << std::endl;

	// Print children
	for (const auto* c : children)
		c->Dump(n + 1);
}

double Block::GetSeconds() const
{
	return std::chrono::duration<double>(end - start).count();
}

Block* Block::AddChild(const std::string& _name)
{
	Block* newBlock = new Block(_name, this);
	children.push_back(newBlock);
	return newBlock;
}
