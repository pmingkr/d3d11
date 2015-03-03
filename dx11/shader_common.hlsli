
// πˆ≈ÿΩ∫ ºŒ¿Ã¥ı -> «»ºø ºŒ¿Ã¥ı
struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

// ºŒ¿Ã¥ı ªÛºˆ cbuffer
cbuffer ConstBuffer : register(b0)
{
	float4 vColor;
	matrix mWVP;
}
