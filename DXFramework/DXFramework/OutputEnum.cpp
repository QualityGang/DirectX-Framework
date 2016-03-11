#include "stdafx.h"
#include "OutputEnum.h"

#include "D3D11Renderer.h"


OutputEnum::OutputEnum()
{
	IDXGIAdapter *dxgiAdapter;
	D3D11Renderer::GetDXGIAdapter(&dxgiAdapter);

	UINT index = 0;

	while (true)
	{
		IDXGIOutput *dxgiOutput;
		HRESULT hr = dxgiAdapter->EnumOutputs(index, &dxgiOutput);

		if (hr == DXGI_ERROR_NOT_FOUND)
			break;
		else if (FAILED(hr))
		{
			PrintError(hr);
			return;
		}

		DXGI_OUTPUT_DESC desc;
		hr = dxgiOutput->GetDesc(&desc);

		if (FAILED(hr))
		{
			PrintError(hr);
			return;
		}
		
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);

		if (!GetMonitorInfo(desc.Monitor, &monitorInfo))
		{
			error_stream << "Failed to obtain monitor info" << std::endl;
			return;
		}

		Output output;
		output.dxgiOutput = dxgiOutput;
		output.desktopArea = monitorInfo.rcMonitor;
		output.workArea = monitorInfo.rcWork;
		output.monitor = desc.Monitor;
		outputList.push_back(output);

		index++;
	}

	SafeRelease(dxgiAdapter);
}

OutputEnum::~OutputEnum()
{
	for (const Output &output : outputList)
	{
		IDXGIOutput *dxgiOutput = output.dxgiOutput;
		SafeRelease(dxgiOutput);
	}
}

const OutputEnum::Output& OutputEnum::getOutput(HWND hwnd)
{
	HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

	for (const Output &output : outputList)
		if (monitor == output.monitor)
			return output;

	throw std::exception();
}