#include "Updatable.h"
#include "../../Manager/ComponentManager.h"

namespace Manager
{
	extern ComponentManager& compMgr;
}

Updatable::~Updatable()
{
	Manager::compMgr.DeleteComponent<Updatable>(this);
}
