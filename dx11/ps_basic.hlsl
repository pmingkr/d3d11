
#include "shader_common.hlsli"

// ÇÈ¼¿ ¼ÎÀÌ´õ »ó¼ö cbuffer
cbuffer CB_PSBasic : register(b0)
{
	float4 vColor;
}

Texture2D tex : register ( t0 );
SamplerState sam : register ( s0 );

float4 main(VSOutput vo) : SV_Target
{
	return tex.Sample(sam, vo.texcoord) * (vo.color + vColor);
}