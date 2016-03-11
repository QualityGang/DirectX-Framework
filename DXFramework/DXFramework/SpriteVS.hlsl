


struct VSInput
{
	float2 Position : POSITION;
	float2 TexCoord : TEXCOORD;

	row_major float4x4 World : WORLD;
};

struct VSOutput
{
	float2 TexCoord : TEXCOORD;
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
	output.Position = mul(input.World, output.Position);
	output.Position = mul(VP, output.Position);
	output.TexCoord = input.TexCoord;
	
	return output;
}