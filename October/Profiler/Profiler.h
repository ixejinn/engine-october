#pragma once
#include <chrono>
#include <deque>
#include <vector>
#include <map>
#include <string>
#include <cfloat>
#include "../Utils/imgui/imgui.h"

namespace OctProfiler
{
	const int MAX_PROFILED_FUNCTIONS = 20;

	struct Block
	{
		std::string name_;
		std::chrono::steady_clock::time_point start;
		std::chrono::steady_clock::time_point end;

		std::deque<Block*> children_;
		Block* parent_;

		Block(const std::string& funcName, Block* parent = nullptr);
		~Block();

		void End();
		void Dump() const;

		double GetSeconds() const;

		Block* AddChild(const std::string& childFuncName);
	};

	struct ScrollingBuffer
	{
		int maxSize_;
		int offset_;
		ImVector<ImVec2> data_;

		ScrollingBuffer(int maxSize = 300)
		{
			maxSize_ = maxSize;
			offset_ = 0;
			data_.reserve(maxSize_);
		}

		void AddPoint(float t, float v)
		{
			if (data_.size() < maxSize_)
				data_.push_back(ImVec2(t, v));
			else
			{
				data_[offset_] = ImVec2(t, v);
				offset_ = (offset_ + 1) % maxSize_;
			}
		}

		void Erase()
		{
			if (data_.size() > 0)
			{
				data_.shrink(0);
				offset_ = 0;
			}
		}
	};

	struct ProfilingData
	{
		// About execution time
		double sum_;
		double min_;
		double max_;

		int callCnt_;

		ProfilingData() : sum_(0.0), min_(0.0), max_(DBL_MAX), callCnt_(0) {}
	};

	class Profiler
	{
	private:
		Block* current_{ nullptr };

		std::vector<ScrollingBuffer*> graphData_{};	// 둘 다 frame 끝날 때 마다
		std::map<std::string, ProfilingData*> reportData_{};

		Profiler();
		~Profiler() = default;

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

		void StartBlock(const std::string& funcName);
		void EndBlock();
		void RecordBlock(Block* block);

		void End();

		void Clear();
	};
}