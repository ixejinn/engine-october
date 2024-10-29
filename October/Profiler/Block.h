#pragma once
#include <string>
#include <chrono>
#include <deque>

class Block
{
private:
	std::string name_{};
	std::chrono::steady_clock::time_point start_{};
	std::chrono::steady_clock::time_point end_{};

	std::deque<Block*> children_{};
	Block* parent_{ nullptr };

public:
	Block(const std::string& name, Block* parent);
	~Block();

	void End();
	void Dump(int n = 0) const;

	double GetSeconds() const;
	std::string GetName() const { return name_; }
	Block* GetParent() const { return parent_; }

	Block* AddChild(const std::string& name);
};