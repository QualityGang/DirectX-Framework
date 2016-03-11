#include "stdafx.h"
#include "SpriteBatch.h"

#include <algorithm>

#include "D3D11Renderer.h"

#include "SpriteVS.h"
#include "SpritePS.h"


#define NUM_VERTICES 4


SpriteBatch::StaticInit SpriteBatch::__static_init;

ID3D11Buffer		*SpriteBatch::VertexBuffer, *SpriteBatch::InstanceBuffer, *SpriteBatch::ConstantBuffer;
ID3D11VertexShader	*SpriteBatch::VertexShader;
ID3D11PixelShader	*SpriteBatch::PixelShader;
ID3D11InputLayout	*SpriteBatch::InputLayout;

bool SpriteBatch::Immediate = false;


SpriteBatch::SpriteBatch() : spriteList(BATCH_SIZE)
{
}

SpriteBatch::~SpriteBatch()
{
}

SpriteBatch::StaticInit::StaticInit()
{
	// Create shaders
	D3D11Renderer::CreateVertexShader(g_SpriteVS, sizeof(g_SpriteVS), &VertexShader);
	D3D11Renderer::CreatePixelShader(g_SpritePS, sizeof(g_SpritePS), &PixelShader);

	// Create input layout
	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",	  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",	  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD",	  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	D3D11Renderer::CreateInputLayout(g_SpriteVS, sizeof(g_SpriteVS), elements, _countof(elements), &InputLayout);

	// Create vertex buffer
	SpriteVertex v[NUM_VERTICES] = 
	{
		// x     y      u     v
		{ 0.0f, 1.0f,  0.0f, 1.0f },
		{ 0.0f, 0.0f,  0.0f, 0.0f },
		{ 1.0f, 1.0f,  1.0f, 1.0f },
		{ 1.0f, 0.0f,  1.0f, 0.0f }
	};

	D3D11Renderer::CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof(SpriteVertex) * NUM_VERTICES, D3D11_USAGE_IMMUTABLE, v, &VertexBuffer);

	// Create instance buffer
	D3D11Renderer::CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof(SpriteInstance) * BATCH_SIZE, D3D11_USAGE_DYNAMIC, nullptr, &InstanceBuffer);

	// Create constant buffer
	static_assert(sizeof(XMMATRIX) % 16 == 0, "constant buffer must be alligned(16)");
	D3D11Renderer::CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(XMMATRIX), D3D11_USAGE_DYNAMIC, nullptr, &ConstantBuffer);
}

SpriteBatch::StaticInit::~StaticInit()
{
	SafeRelease(VertexBuffer);
	SafeRelease(InstanceBuffer);
	SafeRelease(ConstantBuffer);
	SafeRelease(VertexShader);
	SafeRelease(PixelShader);
	SafeRelease(InputLayout);
}

void SpriteBatch::begin(const RenderTarget &renderTarget, SpriteSortMode sortMode,
	ID3D11BlendState *bs, ID3D11SamplerState *ss, ID3D11DepthStencilState *dss, ID3D11RasterizerState *rs,
	std::function<void()> setCustomShaders)
{
	if (beginEndPair)
	{
		error_stream << "begin-end must be paired" << std::endl;
		return;
	}

	beginEndPair = true;

	if (Immediate)
	{
		error_stream << "only one sprite batch can use immediate at once" << std::endl;
		return;
	}

	this->renderTarget = &renderTarget;
	this->sortMode = sortMode;
	this->blendState = bs;
	this->samplerState = ss;
	this->depthStencilState = dss;
	this->rasterizerState = rs;
	this->setCustomShaders = setCustomShaders;

	spriteList.clear();

	if (sortMode == SpriteSortMode::Immediate)
	{
		Immediate = true;
		prepare();
	}
}

void SpriteBatch::end()
{
	if (!beginEndPair)
	{
		error_stream << "begin-end must be paired" << std::endl;
		return;
	}

	beginEndPair = false;

	if (sortMode == SpriteSortMode::Immediate)
	{
		ID3D11ShaderResourceView *nullSrv = nullptr;
		D3D11Renderer::DeviceContext->PSSetShaderResources(0, 1, &nullSrv);
		Immediate = false;
		return;
	}

	// Sort the sprites
	switch (sortMode)
	{
		case SpriteSortMode::Texture:
			std::sort(spriteList.begin(), spriteList.end(), [](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.srv < sprite2.srv;
			});
			break;
	}

	prepare();

	// Draw batches
	UINT numSpritesToDraw = spriteList.size();
	UINT startIndex = 0;

	while (numSpritesToDraw > 0)
	{
		UINT count = getBatchSize(startIndex, std::min(startIndex + BATCH_SIZE, spriteList.size()));
		D3D11Renderer::DeviceContext->PSSetShaderResources(0, 1, &spriteList[startIndex].srv);

		while (count > 0)
		{
			if (count > BATCH_SIZE)
			{
				drawBatch(startIndex, BATCH_SIZE);
				numSpritesToDraw -= BATCH_SIZE;
				startIndex += BATCH_SIZE;
				count -= BATCH_SIZE;
			}
			else
			{
				drawBatch(startIndex, count);
				numSpritesToDraw -= count;
				startIndex += count;
				count = 0;
			}
		}
	}

	// Unbind the shader resource view (conflicts with render to texture)
	ID3D11ShaderResourceView *nullSrv = nullptr;
	D3D11Renderer::DeviceContext->PSSetShaderResources(0, 1, &nullSrv);
}

void SpriteBatch::prepare()
{
	// Update constant buffer
	XMMATRIX *constBuff;
	D3D11Renderer::Map(ConstantBuffer, D3D11_MAP_WRITE_DISCARD, (void**)&constBuff);

	if (constBuff)
	{
		*constBuff = renderTarget->getProjectionMatrix();
		D3D11Renderer::Unmap(ConstantBuffer);
	}

	// Configure pipeline
	ID3D11Buffer *buffers[] = { VertexBuffer, InstanceBuffer };
	UINT strides[] = { sizeof(SpriteVertex), sizeof(SpriteInstance) };
	UINT offsets[] = { 0, 0 };

	D3D11Renderer::DeviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	D3D11Renderer::DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);

	D3D11Renderer::DeviceContext->VSSetShader(VertexShader, nullptr, 0);
	D3D11Renderer::DeviceContext->PSSetShader(PixelShader, nullptr, 0);
	D3D11Renderer::DeviceContext->IASetInputLayout(InputLayout);

	D3D11Renderer::DeviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);
	D3D11Renderer::DeviceContext->PSSetSamplers(0, 1, &samplerState);
	D3D11Renderer::DeviceContext->OMSetDepthStencilState(depthStencilState, 1);
	D3D11Renderer::DeviceContext->RSSetState(rasterizerState);

	ID3D11RenderTargetView *rtv = renderTarget->getRenderTargetView();
	D3D11Renderer::DeviceContext->OMSetRenderTargets(1, &rtv, renderTarget->getDepthStencilView());
	D3D11Renderer::DeviceContext->RSSetViewports(1, &renderTarget->getViewport());
	D3D11Renderer::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (setCustomShaders)
		setCustomShaders();
}

void SpriteBatch::drawBatch(UINT startIndex, UINT count)
{
	// Update instance buffer
	SpriteInstance *instBuff;
	D3D11Renderer::Map(InstanceBuffer, D3D11_MAP_WRITE_DISCARD, (void**)&instBuff);

	if (instBuff)
	{
		for (UINT i = 0; i < count; i++)
		{
			const Sprite &sprite = spriteList[startIndex + i];

			XMVECTOR scaling = XMVectorSet(sprite.size.x, sprite.size.y, 1.0f, 1.0f);
			XMVECTOR origin = XMVectorSet(sprite.size.x / 2.0f, sprite.size.y / 2.0f, 0.0f, 0.0f);
			XMVECTOR translation = XMVectorSet(sprite.position.x, sprite.position.y, 0.0f, 0.0f);
			
			instBuff[i].worldMatrix = XMMatrixAffineTransformation2D(scaling, origin,
														-sprite.degrees, translation);
		}

		D3D11Renderer::Unmap(InstanceBuffer);
	}

	// Draw batch
	D3D11Renderer::DeviceContext->DrawInstanced(NUM_VERTICES, count, 0, 0);
}

UINT SpriteBatch::getBatchSize(UINT startIndex, UINT endIndex) const
{
	UINT count = 1;
	ID3D11ShaderResourceView *srv = spriteList[startIndex].srv;

	for (UINT i = startIndex + 1; i < endIndex; i++)
	{
		if (srv != spriteList[i].srv)
			break;
		
		count++;
	}

	return count;
}

void SpriteBatch::draw(const Sprite &sprite)
{
	if (!beginEndPair)
	{
		error_stream << "begin-end must be paired" << std::endl;
		return;
	}

	if (sortMode == SpriteSortMode::Immediate)
	{
		spriteList.insert(spriteList.begin(), sprite);
		D3D11Renderer::DeviceContext->PSSetShaderResources(0, 1, &sprite.srv);
		drawBatch(0, 1);

		return;
	}

	spriteList.push_back(sprite);
}