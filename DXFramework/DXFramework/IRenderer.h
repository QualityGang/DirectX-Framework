#pragma once
class IRenderer
{
public:
	IRenderer(int width, int height);
	virtual ~IRenderer();

	virtual void resize(int width, int height) = 0;
	virtual void present() = 0;

	virtual void createTexture2D() = 0;
	virtual void createVertexBuffer() = 0;
	virtual void createIndexBuffer() = 0;
	virtual void createConstBuffer() = 0;
	virtual void createPixelShader() = 0;
	virtual void createInputLayout() = 0;
	virtual void createSamplerState() = 0;
	virtual void createBlendState() = 0;
	virtual void createDepthStencilState() = 0;
	virtual void createRasterizerState() = 0;

	virtual void setTexture() = 0;
	virtual void setVertexBuffer() = 0;
	virtual void setConstBuffer() = 0;
	virtual void setVertexShader() = 0;
	virtual void setPixelShader() = 0;
	virtual void setInputLayout() = 0;
	virtual void setSamplerState() = 0;
	virtual void setBlendState() = 0;
	virtual void setDepthStencilState() = 0;
	virtual void setRasterizerState() = 0;
	virtual void setViewport() = 0;
	virtual void setPrimitiveTopology() = 0;
	virtual void drawIndexed() = 0;

	virtual void map() = 0;
	virtual void unmap() = 0;
};

