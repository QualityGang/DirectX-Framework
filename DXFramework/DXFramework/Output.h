#pragma once
class Output
{
public:
	Output(IDXGIOutput *dxgiOutput);
	virtual ~Output();

	void getWorkArea(RECT *rect) const;
	void getDesktopArea(RECT *rect) const;

	void getDXGIOutput(IDXGIOutput *dxgiOutput) const;
private:
	IDXGIOutput *dxgiOutput;
	DXGI_OUTPUT_DESC desc;
	MONITORINFO monitorInfo;
};

