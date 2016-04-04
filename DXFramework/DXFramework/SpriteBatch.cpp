#include "stdafx.h"
#include "SpriteBatch.h"

#include <algorithm>

#include "D3D11Renderer.h"
#include "TextMetrics.h"

#include "string_ext.h"

#include "SpriteVS.h"
#include "SpritePS.h"

#include "Settings.h"


#define NUM_VERTICES 4
#define NUM_INDICES  6


SpriteBatch::StaticInit SpriteBatch::__static_init;

ID3D11Buffer		*SpriteBatch::VertexBuffer, *SpriteBatch::IndexBuffer, *SpriteBatch::ConstantBuffer;
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11Renderer::CreateInputLayout(g_SpriteVS, sizeof(g_SpriteVS), elements, _countof(elements), &InputLayout);

	// Create vertex buffer
	D3D11Renderer::CreateBuffer(D3D11_BIND_VERTEX_BUFFER, sizeof(SpriteVertex) * BATCH_SIZE * NUM_VERTICES,
								D3D11_USAGE_DYNAMIC, nullptr, &VertexBuffer);

	// Create index buffer
	WORD indices[BATCH_SIZE * NUM_INDICES];

	for (UINT i = 0; i < BATCH_SIZE; i++)
	{
		indices[i * NUM_INDICES + 0] = i * NUM_VERTICES + 0;
		indices[i * NUM_INDICES + 1] = i * NUM_VERTICES + 1;
		indices[i * NUM_INDICES + 2] = i * NUM_VERTICES + 2;

		indices[i * NUM_INDICES + 3] = i * NUM_VERTICES + 0;
		indices[i * NUM_INDICES + 4] = i * NUM_VERTICES + 2;
		indices[i * NUM_INDICES + 5] = i * NUM_VERTICES + 3;
	}

	D3D11Renderer::CreateBuffer(D3D11_BIND_INDEX_BUFFER, sizeof(WORD) * BATCH_SIZE * NUM_INDICES, D3D11_USAGE_IMMUTABLE, indices, &IndexBuffer);

	// Create constant buffer
	static_assert(sizeof(XMMATRIX) % 16 == 0, "constant buffer must be aligned(16)");
	D3D11Renderer::CreateBuffer(D3D11_BIND_CONSTANT_BUFFER, sizeof(XMMATRIX), D3D11_USAGE_DYNAMIC, nullptr, &ConstantBuffer);
}

SpriteBatch::StaticInit::~StaticInit()
{
	SafeRelease(VertexBuffer);
	SafeRelease(IndexBuffer);
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

	// Sort the sprites
	switch (sortMode)
	{
		case SpriteSortMode::Texture:
			std::sort(spriteList.begin(), spriteList.end(),
			[](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.srv < sprite2.srv;
			});
			break;
		case SpriteSortMode::BackToFront:
			std::sort(spriteList.begin(), spriteList.end(),
				[](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.depth > sprite2.depth;
			});
			break;
		case SpriteSortMode::FrontToBack:
			std::sort(spriteList.begin(), spriteList.end(),
				[](Sprite &sprite1, Sprite &sprite2)
			{
				return sprite1.depth < sprite2.depth;
			});
			break;
		case SpriteSortMode::Immediate:
			Immediate = false;
			finalize();
			return;
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

	finalize();
}

void SpriteBatch::prepare()
{
	// Update constant buffer
	XMMATRIX *constBuff;
	D3D11Renderer::Map(ConstantBuffer, D3D11_MAP_WRITE_DISCARD, (void**)&constBuff);

	if (constBuff)
	{
		*constBuff = renderTarget->getProjectionMatrix();
		*constBuff = XMMatrixTranspose(*constBuff);
		D3D11Renderer::Unmap(ConstantBuffer);
	}

	// Configure pipeline
	UINT stride = sizeof(SpriteVertex);
	UINT offset = 0;

	D3D11Renderer::DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	D3D11Renderer::DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
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
	D3D11Renderer::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (setCustomShaders)
		setCustomShaders();
}

void SpriteBatch::finalize()
{
	// Unbind the shader resource view (conflicts with render to texture)
	ID3D11ShaderResourceView *nullSrv = nullptr;
	D3D11Renderer::DeviceContext->PSSetShaderResources(0, 1, &nullSrv);
}

void SpriteBatch::drawBatch(UINT startIndex, UINT count)
{
	// Update vertex buffer
	SpriteVertex *vertBuff;
	D3D11Renderer::Map(VertexBuffer, D3D11_MAP_WRITE_DISCARD, (void**)&vertBuff);
	
	if (vertBuff)
	{
		for (UINT i = 0; i < count; i++)
		{
			const Sprite &sprite = spriteList[startIndex + i];

			XMFLOAT2 texSize;
			D3D11Renderer::GetTexture2DSize(sprite.srv, &texSize);
			
			if (texSize.x == 0 || texSize.y == 0)
			{
				error_stream << "invalid texture" << std::endl;
				continue;
			}

			float srcLeftNorm = sprite.src.x / texSize.x;
			float srcTopNorm = sprite.src.y / texSize.y;
			float srcRightNorm = srcLeftNorm + sprite.src.width / texSize.x;
			float srcBottomNorm = srcTopNorm + sprite.src.height / texSize.y;

			switch (sprite.effect)
			{
				case SpriteEffect::FlipHorizontal:
					std::swap(srcLeftNorm, srcRightNorm);
					break;
				case SpriteEffect::FlipVertical:
					std::swap(srcTopNorm, srcBottomNorm);
					break;
				case SpriteEffect::FlipBoth:
					std::swap(srcLeftNorm, srcRightNorm);
					std::swap(srcTopNorm, srcBottomNorm);
					break;
			}

			vertBuff[i * NUM_VERTICES + 0] = SpriteVertex(0.0f, 1.0f, srcLeftNorm,  srcBottomNorm, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a); // bot left;
			vertBuff[i * NUM_VERTICES + 1] = SpriteVertex(0.0f, 0.0f, srcLeftNorm,  srcTopNorm,    sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a); // top left
			vertBuff[i * NUM_VERTICES + 2] = SpriteVertex(1.0f, 0.0f, srcRightNorm, srcTopNorm,    sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a); // top right
			vertBuff[i * NUM_VERTICES + 3] = SpriteVertex(1.0f, 1.0f, srcRightNorm, srcBottomNorm, sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a); // bot right

			XMVECTOR scaling = XMVectorSet(sprite.dest.width, sprite.dest.height, 1.0f, 1.0f);
			XMVECTOR origin = XMVectorSet(sprite.dest.width / 2.0f, sprite.dest.height / 2.0f, 0.0f, 0.0f);
			XMVECTOR translation = XMVectorSet(sprite.dest.x, sprite.dest.y, sprite.depth, 0.0f);

			XMMATRIX worldMatrix = XMMatrixAffineTransformation2D(scaling, origin, -sprite.degrees, translation);
			worldMatrix = XMMatrixTranspose(worldMatrix);
				
			for (UINT j = 0; j < NUM_VERTICES; j++)
				vertBuff[i * NUM_VERTICES + j].world = worldMatrix;
		}

		D3D11Renderer::Unmap(VertexBuffer);
		D3D11Renderer::DeviceContext->DrawIndexed(count * NUM_INDICES, 0, 0);
	}
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
		if (spriteList.empty())
			spriteList.push_back(sprite);
		else
			spriteList[0] = sprite;

		D3D11Renderer::DeviceContext->PSSetShaderResources(0, 1, &sprite.srv);
		drawBatch(0, 1);

		return;
	}

	spriteList.push_back(sprite);
}

void SpriteBatch::drawText(const Text &text, const FontAtlas &atlas)
{
	if (!beginEndPair)
	{
		error_stream << "begin-end must be paired" << std::endl;
		return;
	}

	const Font *font = atlas.getFont();
	
	if (!font)
		return;

	FT_Face face = font->getFace();

	FT_UInt prevGlyphIndex = 0;
	XMFLOAT2 pen(0, 0);
	
	std::vector<std::string> lines;
	stde::string_split(text.str, "\n", &lines);

	UINT currLine = 0;
	int maxAscent = TextMetrics::GetMaxAscent(lines[currLine].c_str(), atlas);
	int maxDescent = TextMetrics::GetMaxDescent(lines[currLine].c_str(), atlas);

	for (LPCSTR lpStr = text.str; *lpStr; lpStr++)
	{
		switch(*lpStr)
		{
			case ' ':
				pen.x += atlas.getCharInfo(' ')->advance.x;
				continue;
			case '\t':
				pen.x += atlas.getCharInfo(' ')->advance.x * 3;
				continue;
			case '\n':
				pen.x = 0;
				pen.y += text.lineGap + maxDescent + maxAscent;
				currLine++;
				maxAscent  = TextMetrics::GetMaxAscent(lines[currLine].c_str(), atlas);
				maxDescent = TextMetrics::GetMaxDescent(lines[currLine].c_str(), atlas);
				continue;
		}

		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (!charInfo)
		{
			error_stream << "Character missing from FontAtlas " << *lpStr << std::endl;
			pen.x += atlas.getCharInfo(' ')->advance.x;
			continue;
		}

		FT_UInt glyphIndex = FT_Get_Char_Index(face, *lpStr);
		pen.x += font->getKerning(prevGlyphIndex, glyphIndex);

		ID3D11ShaderResourceView *srv;
		atlas.getShaderResourceView(&srv);

		Sprite sprite;
		sprite.dest.x = text.position.x + pen.x + charInfo->rect.x;
		sprite.dest.y = text.position.y + pen.y + maxAscent - charInfo->rect.y;
		sprite.dest.width = (float)charInfo->rect.width;
		sprite.dest.height = (float)charInfo->rect.height;
		sprite.src.x = (float)charInfo->charPos.x;
		sprite.src.y = (float)charInfo->charPos.y;
		sprite.src.width = (float)charInfo->rect.width;
		sprite.src.height = (float)charInfo->rect.height;
		sprite.degrees = 0.0f;
		sprite.depth = text.position.z;
		sprite.effect = SpriteEffect::None;
		sprite.color = text.color;
		sprite.srv = srv;

		draw(sprite);

		pen.x += charInfo->advance.x;
		prevGlyphIndex = glyphIndex;
	}
}