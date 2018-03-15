#pragma once

#include "../NoCopy.h"

#include <future>
#include <string>
#include <thread>

class Pipe;
struct PipeCreateParameters;

class PipeServer : public NoCopy
{
public:
	PipeServer();
	~PipeServer();

	void Initialize(const PipeCreateParameters &a_Parameters);
	std::future<std::string> ReadData();

private:
	static void WaitForPipeInput(Pipe *a_Pipe, std::promise<std::string> &&a_Result);

	Pipe *m_Pipe = nullptr;
	std::thread m_ReadThread;
};
