#include "Utils.h"

bool CheckEpsilon(float val, float epsilon)
{
	if (val < -epsilon || val > epsilon)
		return false;
	return true;
}