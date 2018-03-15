#pragma once

#include "../NoCopy.h"

class Pipe;
struct PipeCreateParameters;

class PipeClient : public NoCopy
{
public:
	PipeClient();
	~PipeClient();

	void Initialize(const PipeCreateParameters &a_Parameters);
	void Start();

private:
	Pipe *m_Pipe = nullptr;
};
