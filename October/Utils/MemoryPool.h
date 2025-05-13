#pragma once
#include <stack>
#include <typeindex>	// std::type_index
#include <string>
#include <glm/vec2.hpp>

#include "../GameObject/GameObject.h"
#include "../Manager/GameObjectManager.h"
#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"

namespace Manager
{
	extern GameObjectManager& objMgr;
}

/*
* @tparam T: Gameplay component
*/
template <typename T>
class MemoryPool
{
private:
	std::stack<GameObject*> pool_{};

	int activeNum_{ 0 };
	int maxNum_{ 0 };

public:
	MemoryPool(int maxNum);
	~MemoryPool();

	T* Spawn(const glm::vec2& pos);
	void Release(GameObject* obj);

	void Clear();
};

template <typename T>
inline MemoryPool<T>::MemoryPool(int maxNum) : maxNum_(maxNum)
{
	for (int i = 0; i < maxNum_; i++)
	{
		GameObject* obj = Manager::objMgr.CreateObject();
		obj->AddComponent(typeid(T));
		obj->active_ = false;

		pool_.push(obj);
	}
}

template <typename T>
inline MemoryPool<T>::~MemoryPool()
{
	//Clear();
}

template <typename T>
inline T* MemoryPool<T>::Spawn(const glm::vec2& pos)
{
	if (activeNum_ >= maxNum_)
		return nullptr;

	GameObject* obj = pool_.top();
	pool_.pop();
	activeNum_++;

	static_cast<Transform*>(obj->GetComponent(typeid(Transform)))->SetPosition(pos);
	obj->active_ = true;
}

template<typename T>
inline void MemoryPool<T>::Release(GameObject* obj)
{
	static_cast<Rigidbody*>(obj->GetComponent(typeid(Rigidbody)))->ResetMotion();
	obj->active_ = false;
	pool_.push(obj);
	activeNum_--;
}

template <typename T>
inline void MemoryPool<T>::Clear()
{
	while (!pool_.empty())
	{
		Manager::objMgr.DeleteObject(pool_.top()->GetName());
		pool_.pop();
	}
}