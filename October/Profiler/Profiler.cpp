#include "Profiler.h"

#include <iostream>

OctProfiler::Profiler::Profiler() {}

void OctProfiler::Profiler::StartBlock(const std::string& funcName)
{
	if (!active_)
		return;

	if (!current_)
		current_ = new Block(funcName);	// Root
	else
		current_ = current_->AddChild(funcName);
}

void OctProfiler::Profiler::EndBlock()
{
	if (!active_)
		return;

	// Stop counting time on current block
	current_->End();

	// Go 1 step back
	Block* parent = current_->parent_;

	// If no parent, Record children blocks
	if (!parent)
	{
		for (const auto* b : current_->children_)
		{
			GenerateGraphData(b);
			RecordBlock(b);
		}
	}

	current_ = parent;
}

void OctProfiler::Profiler::GenerateGraphData(const Block* block)
{
	if (graphData_.find(block->name_) == graphData_.end())
		graphData_[block->name_] = new ScrollingBuffer();

	ScrollingBuffer* scrollingBuf = graphData_[block->name_];
	static float t = 0;
	t += ImGui::GetIO().DeltaTime;
	scrollingBuf->AddPoint(t, block->GetSeconds());
}

void OctProfiler::Profiler::RecordBlock(const Block* block)
{
	if (reportData_.find(block->name_) == reportData_.end())
		reportData_[block->name_] = new ProfilingData();

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
	for (auto it = reportData_.begin(); it != reportData_.end(); ++it)
	{
		ProfilingData* data = it->second;

		std::cout << "\n"
			<< "========== " << it->first << " ==========\n"
			<< " call count: " << data->callCnt_ << "\n"
			<< " execution time(microseconds)"
			<< "   average: " << data->sum_ / data->callCnt_
			<< "   min    : " << data->min_
			<< "   max    : " << data->max_ << std::endl;

		delete data;
	}
	reportData_.clear();

	for (auto it = graphData_.begin(); it != graphData_.end(); ++it)
		delete it->second;
	graphData_.clear();

	current_ = nullptr;
	active_ = false;
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

double OctProfiler::Block::GetSeconds() const
{
	return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

OctProfiler::Block* OctProfiler::Block::AddChild(const std::string& childFuncName)
{
	Block* newBlock = new Block(childFuncName, this);
	children_.push_back(newBlock);
	return newBlock;
}