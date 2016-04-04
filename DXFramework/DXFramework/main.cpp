#include "stdafx.h"

#include <thread>

#include "FI.h"

#include "Window.h"
#include "SpriteBatch.h"
#include "RenderTexture.h"

constexpr long double operator"" _deg(long double deg)
{
	return deg * (3.141592654 / 180.0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	Window window("Title", 800, 600);
	//window.addOnResizeListener(
	//[](Window *window, int width, int height) -> void
	//{
	//	window->resize(width, height);
	//});
	//window.setFullscreen(true);
	//window.removeOnResizeListener(0);

	SpriteBatch batch;

	ID3D11BlendState *bs;
	D3D11Renderer::CreateBSAlphaBlend(&bs);

	ID3D11SamplerState *ss;
	D3D11Renderer::CreateSSLinearClamp(&ss);

	ID3D11DepthStencilState *dss;
	D3D11Renderer::CreateDSSDepthDefault(&dss);

	ID3D11RasterizerState *rs;
	D3D11Renderer::CreateRSCullCounterClockwise(&rs);

	Bitmap bmp;
	FI::CreateBitmap("box.png", &bmp);

	Bitmap bmp2;
	FI::CreateBitmap("player.jpg", &bmp2);

	ID3D11ShaderResourceView *srv;
	bmp.getShaderResourceView(&srv);

	ID3D11ShaderResourceView *srv2;
	bmp2.getShaderResourceView(&srv2);

	Sprite sprite;
	sprite.dest.set(100, 200, 150, 150);
	sprite.src.set(0, 0, bmp.getWidth(), bmp.getHeight());
	sprite.degrees = (float)0.0_deg;
	sprite.depth = 0;
	sprite.effect = SpriteEffect::None;
	sprite.color.set(0, 0.25f, 0.75f, 1);
	sprite.srv = srv;

	Sprite sprite2;
	sprite2.dest.set(150, 400, 150, 150);
	sprite2.src.set(0, 0, bmp2.getWidth(), bmp2.getHeight());
	sprite2.degrees = (float)15.0_deg;
	sprite2.depth = 0;
	sprite2.effect = SpriteEffect::None;
	sprite2.color.set(1, 1, 1, 1);
	sprite2.srv = srv2;

	Text text;
	text.str = "Test\nString\nAWESOMENESS";
	text.position.x = 0;
	text.position.y = 0;
	text.position.z = 0;
	text.lineGap = 5;
	text.color.set(0, 0, 0, 1);

	Font font("arial.ttf", 86);
	FontAtlas atlas(FA_ALLLOWERCASE FA_ALLUPPERCASE, &font);

	Bitmap bmp3(675, 300);

	RenderTexture renderTexture(bmp3);
	ID3D11ShaderResourceView *srv3;
	renderTexture.getShaderResourceView(&srv3);

	Sprite sprite3;
	sprite3.dest.set(100, 50, renderTexture.getViewport().Width, renderTexture.getViewport().Height);
	sprite3.src.set(0, 0, renderTexture.getViewport().Width, renderTexture.getViewport().Height);
	sprite3.degrees = (float)0.0_deg;
	sprite3.depth = 0;
	sprite3.effect = SpriteEffect::None;
	sprite3.color.set(1, 1, 1, 1);
	sprite3.srv = srv3;

	std::thread renderThread(
	[&]() -> void
	{
		int width  = (int)window.getViewport().Width;
		int height = (int)window.getViewport().Height;

		window.addOnResizeListener(
		[&](Window *window, int w, int h) -> void
		{
			width = w;
			height = h;
		});

		while (window.getHandle())
		{
			using namespace std::literals;

			if (window.isKeyPressed(Key::Escape))
			{
				window.close();
				continue;
			}

			XMINT2 size;
			window.getSize(&size);

			while (window.isMinimized() || size.x == 0 || size.y == 0)
			{
				std::this_thread::sleep_for(50ms);
				window.getSize(&size);
			}

			if (width != window.getViewport().Width || height != window.getViewport().Height)
				window.resize(width, height);

			sprite.dest.x += 10;
			static float angle = 0;
			sprite.dest.y = sinf(angle) * 50 + 150;
			angle += XM_PI * 0.08f;

			renderTexture.clear(0, 1, 0, 1);
			batch.begin(renderTexture, SpriteSortMode::Deferred, bs, ss, dss, rs, nullptr);
			batch.drawText(text, atlas);
			batch.end();

			window.clear(1.0f, 0.0f, 0.0f, 0.0f);
			batch.begin(window, SpriteSortMode::Deferred, bs, ss, dss, rs, nullptr);
			batch.draw(sprite);
			batch.draw(sprite2);
			batch.draw(sprite3);
			batch.end();
			window.present();
		}
	});

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
		if (GetMessage(&msg, nullptr, 0, 0))
			DispatchMessage(&msg);

	renderThread.join();
	
	SafeRelease(bs);
	SafeRelease(ss);
	SafeRelease(dss);
	SafeRelease(rs);

	return msg.wParam;
}
