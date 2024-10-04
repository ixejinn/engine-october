#include "FixedUpdatable.h"

#include "../../Manager/ComponentManager.h"

namespace Manager
{
	extern ComponentManager& compMgr;
}

FixedUpdatable::~FixedUpdatable()
{
	Manager::compMgr.DeleteComponent<FixedUpdatable>(this);
}
