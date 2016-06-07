

Texture2D Texture    : register(t0);
SamplerState Sampler : register(s0);

struct PSInput
{
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

float4 main(PSInput input) : SV_Target
{
	return Texture.Sample(Sampler, input.TexCoord) * input.Color;
}