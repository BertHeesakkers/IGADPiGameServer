#pragma once

enum ETravelOption
{
	ETravelOption_Taxi,
	ETravelOption_Bus,
	ETravelOption_Underground,
	ETravelOption_Ferry,
	ETravelOption_Black = ETravelOption_Ferry,

	ETravelOption_Count,
	ETravelOption_Invalid
};