#pragma once

unsigned long DJB2(const char* a_Chars)
{
    unsigned long  hashValue = 5381L;
    int c;
    while((c = *a_Chars++))
    {
        hashValue = ((hashValue << 5) + hashValue) + c;
    }
    return hashValue;
}

unsigned long SDBM(const char* a_Chars)
{
    unsigned long hashValue = 0L;
    int c;
    while((c = *a_Chars++))
    {
        hashValue = c + (hashValue << 6) + (hashValue << 16) - hashValue;
    }
    return hashValue;
}

unsigned long g_CRC32Table[256];
bool g_CRC32TableReady = false;

bool GenerateCRC32Table()
{
    unsigned long polynomial = 0xEDB88320;
    unsigned long crcValue = 0L;
    for(int i = 0; i < 256; i++)
    {
        crcValue = static_cast<unsigned long>(i);
        for(int j = 8; j > 0; j--)
        {
            if(crcValue & 1)
            {
                crcValue = (crcValue >> 1) ^ polynomial;
            }
            else
            {
                crcValue >>= 1;
            }
        }
        g_CRC32Table[i] = crcValue;            
    }
    return true;
}

void UpdateCrc32(unsigned long &a_Crc32, const unsigned char a_Char)
{
    a_Crc32 = ((a_Crc32) >> 8) ^ g_CRC32Table[a_Char ^ (a_Crc32 & 0x000000FF)];
}

unsigned long Crc32(const char* a_Chars)
{
    if(!g_CRC32TableReady)
    {
        g_CRC32TableReady = GenerateCRC32Table();
    }
    
    unsigned long hashValue = 0L;
    int c;
    while((c = *a_Chars++))
    {
        UpdateCrc32(hashValue, static_cast<unsigned char>(c));
    }
    
    return hashValue;
}
