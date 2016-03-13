#include "stdafx.h"

#include <thread>

#include "FI.h"

#include "Window.h"
#include "SpriteBatch.h"

constexpr long double operator"" _deg(long double deg)
{
	return deg * (3.141592654 / 180.0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	Window window("Title", 800, 600);
	SpriteBatch batch;

	ID3D11BlendState *bs;
	D3D11Renderer::CreateBSAlphaBlend(&bs);

	ID3D11SamplerState *ss;
	D3D11Renderer::CreateSSPointClamp(&ss);

	ID3D11DepthStencilState *dss;
	D3D11Renderer::CreateDSSDepthDefault(&dss);

	ID3D11RasterizerState *rs;
	D3D11Renderer::CreateRSCullCounterClockwise(&rs);

	Bitmap bmp;
	FI::CreateBitmap("box.png", &bmp);

	ID3D11ShaderResourceView *srv;
	bmp.getShaderResourceView(&srv);

	SpriteBatch::Sprite sprite;
	sprite.position.x = 100;
	sprite.position.y = 200;
	sprite.size.x = 150;
	sprite.size.y = 150;
	sprite.degrees = (float)0.0_deg;
	sprite.srv = srv;

	SpriteBatch::Sprite sprite2;
	sprite2.position.x = 150;
	sprite2.position.y = 400;
	sprite2.size.x = 150;
	sprite2.size.y = 150;
	sprite2.degrees = (float)15.0_deg;
	sprite2.srv = srv;

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

			XMINT2 size;
			window.getSize(&size);

			while (window.isMinimized() || size.x == 0 || size.y == 0)
				std::this_thread::sleep_for(50ms);

			if (width != window.getViewport().Width || height != window.getViewport().Height)
				window.resize(width, height);

			sprite.position.x += 10;
			static float angle = 0;
			sprite.position.y = sinf(angle) * 50 + 150;
			angle += XM_PI * 0.08f;

			window.clear(1.0f, 0.0f, 0.0f, 0.0f);
			batch.begin(window, SpriteSortMode::Deferred, bs, ss, dss, rs, nullptr);
			batch.draw(sprite);
			batch.draw(sprite2);
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