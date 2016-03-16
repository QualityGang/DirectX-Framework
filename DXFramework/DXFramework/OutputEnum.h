#pragma once

#include <vector>

#include "NonCopyable.h"

class OutputEnum : public NonCopyable
{
public:
	OutputEnum();
	virtual ~OutputEnum();

	struct Output
	{
		IDXGIOutput *dxgiOutput;
		RECT workArea;
		RECT desktopArea;
		HMONITOR monitor;
	};

	const Output& getOutput(HWND hwnd);
private:
	std::vector<Output> outputList;
};

