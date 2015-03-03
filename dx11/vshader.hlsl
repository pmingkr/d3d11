
#include "shader_common.hlsli"

struct Vertex
{
	float4 pos : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
};

VSOutput main(Vertex vi)
{
	VSOutput vo;
	vo.position = mul(vi.pos, mWVP);
	vo.color = saturate(vi.norm.yyyy);
	vo.texcoord = vi.tex;
	return vo;
}
