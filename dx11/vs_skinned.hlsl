
#include "shader_common.hlsli"
#include "vs_common.hlsli"

// 정점 셰이더 상수 cbuffer
cbuffer CB_VSSkinned : register(b1)
{
	float4x3 mWorlds[80];
}

struct Vertex
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
	uint4 blendidx : BLENDINDICES;
	float4 blendwgt : BLENDWEIGHT;
};

VSOutput main(Vertex vi)
{
	VSOutput vo;

	float3 pos = 0.0;
	float3 norm = 0.0;
	for (int i = 0; i < 4; i++)
	{
		pos += mul(vi.position, mWorlds[vi.blendidx[i]]) * vi.blendwgt[i];
		norm += mul(vi.normal, (float3x3)mWorlds[vi.blendidx[i]]) * vi.blendwgt[i];
	}

	vo.position = mul(float4(pos, 1.0), mVP);
	vo.color = saturate(normalize(norm).yyyy);
	vo.texcoord = vi.texcoord;
	return vo;
}
