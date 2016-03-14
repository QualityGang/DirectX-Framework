#pragma once

#include <functional>
#include <vector>

#include "RenderTarget.h"


enum class SpriteSortMode
{
	Deferred,
	Immediate,
	Texture
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

	struct Sprite
	{
		XMFLOAT2 position;
		XMFLOAT2 size;
		float degrees;
		ID3D11ShaderResourceView *srv;
	};

	void draw(const Sprite &sprite);
private:
	const RenderTarget		 *renderTarget;
	ID3D11BlendState		 *blendState;
	ID3D11SamplerState		 *samplerState;
	ID3D11DepthStencilState	 *depthStencilState;
	ID3D11RasterizerState	 *rasterizerState;

	static ID3D11Buffer			*VertexBuffer, *InstanceBuffer, *ConstantBuffer;
	static ID3D11VertexShader	*VertexShader;
	static ID3D11PixelShader	*PixelShader;
	static ID3D11InputLayout	*InputLayout;

	std::vector<Sprite> spriteList;

	bool beginEndPair;
	SpriteSortMode sortMode;
	std::function<void()> setCustomShaders;

	static bool Immediate;

	void prepare();
	void drawBatch(UINT startIndex, UINT count);
	UINT getBatchSize(UINT startIndex, UINT endIndex) const;


	struct SpriteVertex
	{
		SpriteVertex() {}
		SpriteVertex(float x, float y, float u, float v) : position(x, y), texCoords(u, v) {}

		XMFLOAT2 position;
		XMFLOAT2 texCoords;
	};
	
	ALIGN(16)
	struct SpriteInstance : AlignedNew<16>
	{
		XMMATRIX worldMatrix;
	};

	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};

