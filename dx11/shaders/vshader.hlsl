
cbuffer ConstBuffer : register(b0)
{
	float4 vColor;
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}
