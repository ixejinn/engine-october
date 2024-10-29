#include "Profiler.h"

#include "Block.h"

Profiler::~Profiler()
{
	Clear();
}

void Profiler::StartBlock(std::string _name)
{
	if (!current)
		current = new Block(_name);	// Root
	else
		current = current->AddChild(_name);
}

void Profiler::End()
{
	// Stop counting time on current block
	current->End();

	// Go 1 step back
	Block* parent = current->GetParent();

	// If no parent, Push current to fullyFinishedBlock
	if (!parent)
	{
		while (finishedBlocks.size() >= maxQCapacity)
		{
			delete finishedBlocks.front();
			finishedBlocks.pop();
		}

		finishedBlocks.push(current);
	}

	current = parent;
}

void Profiler::Dump()
{

}

void Profiler::Clear()
{
}
