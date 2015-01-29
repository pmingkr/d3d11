
#include "shader_common.hlsli"

// 셰이더 상수 cbuffer
cbuffer ConstBuffer : register(b0)
{
	float4 vColor;
}

struct Vertex
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

VSOutput main(Vertex vi)
{
	VSOutput vo;
	vo.position = vi.pos;
	vo.texcoord = vi.tex;
	return vo;
}
