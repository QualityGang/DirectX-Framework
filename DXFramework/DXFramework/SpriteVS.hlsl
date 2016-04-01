


struct VSInput
{
	float2   Position : POSITION;
	float2   TexCoord : TEXCOORD;
	float4   Color    : COLOR;
	float4x4 World    : TEXCOORD1;
};

struct VSOutput
{
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
	float4 Position : SV_Position;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix VP;
}

VSOutput main(VSInput input)
{
	VSOutput output;
	output.Position = float4(input.Position, 0.0f, 1.0f);
	output.Position = mul(output.Position, input.World);
	output.Position = mul(output.Position, VP);
	output.Color = input.Color;
	output.TexCoord = input.TexCoord;

	return output;
}