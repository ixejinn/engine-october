#include "EmptyState.h"

#include "../Manager/GameObjectManager.h"
#include "../Manager/ComponentManager.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
    extern ComponentManager& compMgr;
}


void EmptyState::Init()
{
}

void EmptyState::Update()
{
}

void EmptyState::Exit()
{
    Manager::compMgr.Clear();
    Manager::objMgr.Clear();
}
