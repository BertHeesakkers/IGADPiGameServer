#include "GetShipSize.h"

#include <assert.h>

unsigned short GetShipSize(EShip a_Ship)
{
	unsigned short size = 0;
	switch (a_Ship)
	{
		case EShip_AircraftCarrier:
		{
			size = 5;
		}
		break;
		case EShip_Battleship:
		{
			size = 4;
		}
		break;
		case EShip_Submarine:
		{
			size = 3;
		}
		break;
		case EShip_Destroyer:
		{
			size = 3;
		}
		break;
		case EShip_PatrolBoat:
		{
			size = 2;
		}
		break;
		default:
		{
			assert(false);
		}
		break;
	}
	return size;
}