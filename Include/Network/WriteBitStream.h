#pragma once

#include "RakNetIncludes.h"

template<typename TYPE>
void WriteBitStream(RakNet::BitStream &a_BitStream, const TYPE &a_Buffer);
