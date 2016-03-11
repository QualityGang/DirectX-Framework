#include "stdafx.h"
#include "Logger.h"

#include <Windows.h>

Logger::Logger() : buffer(this), std::ostream(&buffer)
{
}

Logger::~Logger()
{
}

Logger::Buffer::Buffer(Logger *thisptr) : thisptr(thisptr)
{
}

Logger::Buffer::~Buffer()
{
}

int Logger::Buffer::sync()
{
	thisptr->sync();
	str("");
	return 0;
}

VSLogger::VSLogger(const std::string &tag = "") : tag(tag)
{
}

VSLogger::~VSLogger()
{
}

void VSLogger::sync()
{
	std::string s;
	
	if (!tag.empty())
	{
		s += tag + ": ";
	}

	OutputDebugString((s + buffer.str()).c_str());
}

FileLogger::FileLogger(const std::string &fileName) : file(fileName, std::ios::out | std::ios::app)
{
}

FileLogger::~FileLogger()
{
}

void FileLogger::sync()
{
	file << buffer.str();
	file.flush();
}


VSLogger vsout;
VSLogger debug_stream("DEBUG");
VSLogger warning_stream("WARNING");
VSLogger error_stream("ERROR");

FileLogger log_stream("log.txt");


FunctionTimer::FunctionTimer(const std::string &tag) : timeStart(std::chrono::high_resolution_clock::now()), tag(tag)
{
}

FunctionTimer::~FunctionTimer()
{
	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart).count();
	debug_stream << tag << " -> " << duration << std::endl;
}