
// ���ؽ� ���̴� -> �ȼ� ���̴�
struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

// ���̴� ��� cbuffer
cbuffer ConstBuffer : register(b0)
{
	float4 vColor;
	matrix mWVP;
}
