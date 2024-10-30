#include "Profiler.h"

namespace Manager
{
	extern OctProfiler::Profiler& profiler;
}

OctProfiler::Profiler::Profiler()
{
	graphData_.reserve(MAX_PROFILED_FUNCTIONS);
}

void OctProfiler::Profiler::StartBlock(const std::string& funcName)
{
	if (!current_)
		current_ = new Block(funcName);	// Root
	else
		current_ = current_->AddChild(funcName);
}

void OctProfiler::Profiler::EndBlock()
{
	// Stop counting time on current block
	current_->End();

	// Go 1 step back
	Block* parent = current_->parent_;

	// If no parent, Push current to fullyFinishedBlock
	if (!parent)
		;

	current_ = parent;
}

void OctProfiler::Profiler::RecordBlock(Block* block)
{
	if (reportData_.find(block->name_) != reportData_.end())
		ProfilingData* newData = new ProfilingData();

	ProfilingData* blockData = reportData_[block->name_];
	blockData->callCnt_++;
	double sec = block->GetSeconds();
	blockData->sum_ += sec;
	if (blockData->min_ > sec)
		blockData->min_ = sec;
	if (blockData->max_ < sec)
		blockData->max_ = sec;
}

void OctProfiler::Profiler::End()
{
}

void OctProfiler::Profiler::Clear()
{
}

OctProfiler::Block::Block(const std::string& funcName, Block* parent) : name_(funcName), parent_(parent)
{
	start = std::chrono::steady_clock::now();
}

OctProfiler::Block::~Block()
{
	// Delete all the children
	for (auto it : children_)
		delete it;

	children_.clear();
}

void OctProfiler::Block::End()
{
	end = std::chrono::steady_clock::now();
}

void OctProfiler::Block::Dump() const
{
	
}
