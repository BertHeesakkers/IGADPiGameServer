#pragma once

enum ETravelResult
{
	ETravelResult_NoTokensLeft,
	ETravelResult_NoValidTravelOption,
	ETravelResult_DestinationNotFree,
	ETravelResult_CaughtSpy,

	ETravelResult_Success,

	ETravelResult_Count,
	ETravelResult_Invalid
};