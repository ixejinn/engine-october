#include "LateUpdatable.h"

#include "../../Manager/ComponentManager.h"

namespace Manager
{
	extern ComponentManager& compMgr;
}

LateUpdatable::~LateUpdatable()
{
	Manager::compMgr.DeleteComponent<LateUpdatable>(this);
}
