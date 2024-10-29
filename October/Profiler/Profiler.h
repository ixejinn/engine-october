#pragma once
#include <queue>
#include <vector>
#include <map>
#include <string>

class Block;

class Profiler
{
private:
	Block* current{ nullptr };

	//finishedBlocks{};
	int maxQCapacity{ 300 };

	Profiler() = default;
	~Profiler();

	Profiler(const Profiler&) = delete;
	Profiler& operator =(const Profiler&) = delete;
	Profiler(Profiler&&) = delete;
	Profiler& operator =(Profiler&&) = delete;

public:
	static Profiler& GetInstance()
	{
		static Profiler instance;
		return instance;
	}

	void StartBlock(std::string _name);
	void End();
	void Dump();

	void Clear();
};