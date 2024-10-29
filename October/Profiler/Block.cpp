#include "Block.h"

#include <iostream>

Block::Block(const std::string& name, Block* parent) : name_(name), parent_(parent)
{
	start_ = std::chrono::steady_clock::now();
}

Block::~Block()
{
	// Delete all the children
	for (auto it : children_)
		delete it;

	children_.clear();
}

void Block::End()
{
	end_ = std::chrono::steady_clock::now();
}

void Block::Dump(int n) const
{
	// Print correct amount of tabs
	for (int i = 0; i < n; i++)
		std::cout << "\t";

	// Print name and seconds
	std::cout << name_ << " in " << GetSeconds() << " seconds" << std::endl;

	// Print children
	for (const auto* c : children_)
		c->Dump(n + 1);
}

double Block::GetSeconds() const
{
	return std::chrono::duration<double>(end_ - start_).count();
}

Block* Block::AddChild(const std::string& name)
{
	Block* newBlock = new Block(name, this);
	children_.push_back(newBlock);
	return newBlock;
}
