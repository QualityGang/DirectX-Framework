#pragma once
class FT
{
public:
	FT() = delete;
	~FT() = delete;

	static FT_Library Library;
private:
	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};