#pragma once

#include <sstream>
#include <fstream>
#include <chrono>

class Logger : public std::ostream
{
public:
	Logger();
	virtual ~Logger();

	virtual void sync() = 0;
protected:
	class Buffer : public std::stringbuf
	{
	public:
		Buffer(Logger *thisptr);
		virtual ~Buffer();

		int sync() override;
	private:
		Logger *thisptr;
		std::string tag;
	};

	Buffer buffer;
};

class VSLogger : public Logger
{
public:
	VSLogger(const std::string &tag);
	virtual ~VSLogger();

	void sync() override;
private:
	std::string tag;
};

class FileLogger : public Logger
{
public:
	FileLogger(const std::string &fileName);
	virtual ~FileLogger();

	void sync() override;
private:
	std::fstream file;
};

extern VSLogger vsout;
extern VSLogger debug_stream;
extern VSLogger warning_stream;
extern VSLogger error_stream;

extern FileLogger log_stream;

class FunctionTimer
{
public:
	FunctionTimer(const std::string &tag);
	virtual ~FunctionTimer();
private:
	std::chrono::high_resolution_clock::time_point timeStart;
	std::string tag;
};

#define THROW(str) do { error_stream << str << " " << __FILE__ << " " << __LINE__ << std::endl; throw std::exception(str); } while (false)