#pragma once

class Editor
{
private:
	Editor() = default;
	~Editor() = default;

	Editor(const Editor&) = delete;
	Editor& operator =(const Editor&) = delete;
	Editor(Editor&&) = delete;
	Editor& operator =(Editor&&) = delete;

public:
	static Editor& GetInstance()
	{
		static Editor instance;
		return instance;
	}

	void Topbar();
};