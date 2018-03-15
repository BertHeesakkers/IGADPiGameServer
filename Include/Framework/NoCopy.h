#pragma once

class NoCopy
{
public:
    NoCopy() {}
    virtual ~NoCopy() {}

private:
    NoCopy(const NoCopy&);
    const NoCopy& operator=(const NoCopy&);
};
