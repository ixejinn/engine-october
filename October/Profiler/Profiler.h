#pragma once
#include <chrono>
#include <deque>
#include <map>
#include <string>
#include <cfloat>
#include "../Utils/imgui/imgui.h"

namespace OctProfiler
{
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

		//void Erase()
		//{
		//	if (data_.size() > 0)
		//	{
		//		data_.shrink(0);
		//		offset_ = 0;
		//	}
		//}
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

	enum ProfilerState
	{
		INACTIVE,
		ACTIVE,
		REPORT
	};

	class Profiler
	{
	private:
		Block* current_{ nullptr };

		std::map<std::string, ProfilingData*> reportData_{};

		Profiler();
		~Profiler() = default;

		Profiler(const Profiler&) = delete;
		Profiler& operator =(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;
		Profiler& operator =(Profiler&&) = delete;

	public:
		ProfilerState state_{ INACTIVE };

		std::map<std::string, ScrollingBuffer*> graphData_{};

		static Profiler& GetInstance()
		{
			static Profiler instance;
			return instance;
		}

		void StartBlock(const std::string& funcName);
		void EndBlock();

		void GenerateGraphData(const Block* block);
		void RecordBlock(const Block* block);

		void End();
	};
}

#ifndef __FUNCTION_NAME__

#ifdef WIN32
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif	// WIN32

#endif	// __FUNCTION_NAME__

#ifdef _DEBUG

#define DEBUG_PROFILER_BLOCK_START(x) OctProfiler::Profiler::GetInstance().StartBlock(x)
#define DEBUG_PROFILER_BLOCK_END OctProfiler::Profiler::GetInstance().EndBlock()
#define DEBUG_PROFILER_END OctProfiler::Profiler::GetInstance().End()

#else	// _DEBUG

#define DEBUG_PROFILER_BLOCK_START(x)	//
#define DEBUG_PROFILER_BLOCK_END		//
#define DEBUG_PROFILER_END				//

#endif	// _DEBUG