#pragma once
class Bitmap
{
public:
	Bitmap();
	Bitmap(UINT width, UINT height, const BYTE *pixels = nullptr);
	virtual ~Bitmap();

	Bitmap(const Bitmap &other);
	Bitmap(Bitmap &&other);

	Bitmap& operator=(const Bitmap &other);
	Bitmap& operator=(Bitmap &&other);

	void setPixel(UINT index, BYTE r, BYTE g, BYTE b, BYTE a);
	void setPixel(UINT x, UINT y, BYTE r, BYTE g, BYTE b, BYTE a);

	void getPixel(UINT index, XMUINT4 *color) const;
	void getPixel(UINT x, UINT y, XMUINT4 *color) const;

	UINT getWidth() const;
	UINT getHeight() const;
	void getTexture2D(ID3D11Texture2D **tex) const;
	void getShaderResourceView(ID3D11ShaderResourceView **srv) const;
private:
	bool updateTexture() const;

	UINT width, height;
	BYTE *buffer = nullptr;

	mutable bool dirty = true;
	
	mutable ID3D11Texture2D *texture = nullptr;
	mutable ID3D11ShaderResourceView *shaderResourceView = nullptr;
};

