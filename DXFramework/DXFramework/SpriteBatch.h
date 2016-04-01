#pragma once

#include <functional>
#include <vector>

#include "RenderTarget.h"
#include "FontAtlas.h"
#include "Color.h"


enum class SpriteSortMode
{
	Deferred,
	Immediate,
	Texture,
	BackToFront,
	FrontToBack
};

enum class SpriteEffect
{
	None,
	FlipVertical,
	FlipHorizontal,
	FlipBoth
};

struct Sprite
{
	Rect<float> src;
	Rect<float> dest;
	float degrees;
	float depth;
	SpriteEffect effect;
	Color<float> color;
	ID3D11ShaderResourceView *srv;
};

struct Text
{
	LPCSTR str;
	XMFLOAT3 position;
	float lineGap;
	Color<float> color;
};

class SpriteBatch : public NonCopyable
{
public:
	SpriteBatch();
	virtual ~SpriteBatch();

	void begin(const RenderTarget &renderTarget, SpriteSortMode sortMode, 
		ID3D11BlendState *bs, ID3D11SamplerState *ss, ID3D11DepthStencilState *dss, ID3D11RasterizerState *rs,
		std::function<void()> setCustomShaders);
	void end();

	void draw(const Sprite &sprite);
	void drawText(const Text &text, const FontAtlas &atlas);
private:
	const RenderTarget		 *renderTarget;
	ID3D11BlendState		 *blendState;
	ID3D11SamplerState		 *samplerState;
	ID3D11DepthStencilState	 *depthStencilState;
	ID3D11RasterizerState	 *rasterizerState;

	static ID3D11Buffer			*VertexBuffer, *IndexBuffer, *ConstantBuffer;
	static ID3D11VertexShader	*VertexShader;
	static ID3D11PixelShader	*PixelShader;
	static ID3D11InputLayout	*InputLayout;

	std::vector<Sprite> spriteList;

	bool beginEndPair;
	SpriteSortMode sortMode;
	std::function<void()> setCustomShaders;

	static bool Immediate;

	void prepare();
	void finalize();

	void drawBatch(UINT startIndex, UINT count);
	UINT getBatchSize(UINT startIndex, UINT endIndex) const;

	static int GetMaxAscent(LPCSTR str, const FontAtlas &atlas);
	static int GetMaxDescent(LPCSTR str, const FontAtlas &atlas);

	ALIGN(16)
	struct SpriteVertex : AlignedNew<16>
	{
		SpriteVertex(float x, float y, float u, float v,
			         float r, float g, float b, float a)
			: position(x, y), texCoords(u, v), color(r, g, b, a) {}

		XMFLOAT2 position;
		XMFLOAT2 texCoords;
		XMFLOAT4 color;
		XMMATRIX world;
	};

	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};

