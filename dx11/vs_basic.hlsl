
#include "shader_common.hlsli"
#include "vs_common.hlsli"

// 정점 셰이더 상수 cbuffer
cbuffer CB_VSBasic : register(b1)
{
	float4x4 mWorld;
}

struct Vertex
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
};

VSOutput main(Vertex vi)
{
	VSOutput vo;
	vo.position = mul(mul(vi.position, mWorld), mVP);
	vo.color = saturate(normalize(mul(vi.normal, (float3x3)mWorld)).yyyy);
	vo.texcoord = vi.texcoord;
	return vo;
}
