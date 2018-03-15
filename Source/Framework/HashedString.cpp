#include "HashedString.h"

#include "AutoPointer.h"
#include "HashFunctions.h"
#include "Math/RNG/CreateRandomNumberGenerator.h"
#include "Math/RNG/IRandomNumberGenerator.h"

#include <limits>
#include <stdint.h>

namespace
{
	AutoPointer<IRandomNumberGenerator> g_RandomNumberGenerator = CreateRandomNumberGenerator(982451653);	// http://primes.utm.edu/lists/small/millions/
}

const HashedString HashedString::Empty = HashedString("");

HashedString::HashedString(const char* a_String)
{
#ifdef _DEBUG
    m_String = std::string(a_String);
#endif
    m_Hash = CalculateHash(a_String);
}

HashedString::HashedString( const std::string &a_String)
{
#ifdef _DEBUG
    m_String = a_String;
#endif
    m_Hash = CalculateHash(a_String.c_str());
}

HashedString::HashedString(const HashedString& a_HashedString)
{
#ifdef _DEBUG
    m_String = a_HashedString.m_String;
#endif
    m_Hash = a_HashedString.m_Hash;
}

HashedString::HashedString(unsigned long a_Hash)
: m_Hash(a_Hash)
{
}

HashedString::~HashedString()
{

}

bool HashedString::operator == (const HashedString& a_Rhs) const
{
    return m_Hash == a_Rhs.m_Hash;
}

bool HashedString::operator != (const HashedString& a_Rhs) const
{
    return m_Hash != a_Rhs.m_Hash;
}

HashedString& HashedString::operator = (const HashedString& a_Rhs)
{
    m_Hash = a_Rhs.m_Hash;
#ifdef _DEBUG
    m_String = a_Rhs.m_String;
#endif
    return *this;
}

HashedString& HashedString::operator = (const std::string& a_Rhs)
{
#ifdef _DEBUG
    m_String = a_Rhs;
#endif
    m_Hash = CalculateHash(a_Rhs.c_str());
    return( *this );
}

HashedString& HashedString::operator = (const unsigned long a_Rhs)
{
	m_Hash = a_Rhs;
	return( *this );
}

bool HashedString::operator > (const HashedString& a_Rhs) const
{
    return(m_Hash > a_Rhs.m_Hash);
}

bool HashedString::operator < (const HashedString& a_Rhs) const
{
    return(m_Hash < a_Rhs.m_Hash);
}

HashedString::operator unsigned long()
{ 
	return m_Hash; 
}

unsigned long HashedString::CalculateHash(const char* pChars, eHashFunction hashFunc /*= kCRC32*/)
{
	unsigned long ulHash;

	switch( hashFunc )
	{
	case kCRC32: 
		{ 
			ulHash = Crc32(pChars);
			break;
		}
	case kDJB2: 
		{ 
			ulHash = DJB2(pChars); 
			break;
		} 
	case kSDBM: 
		{ 
			ulHash = SDBM(pChars); 
			break;
		} 
	}

	return ulHash;
}

HashedString GenerateRandomHashedString()
{
	const unsigned long hashValue = static_cast<unsigned long>(g_RandomNumberGenerator->NextUint32(0, std::numeric_limits<uint32_t>::max()));
	return HashedString(hashValue);
}
