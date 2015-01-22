
cbuffer ConstBuffer : register(b0)
{
	float4 vColor;
}

float4 main(float4 pos : SV_POSITION) : SV_Target
{
	return vColor;
}