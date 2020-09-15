struct VertexIn
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float3 col : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 color:COLOR;
};

float4 PS(VertexOut pIn) : SV_Target
{
	return float4(pIn.color,1.0f);
	//return float4(1.0f,1.0f,0.0f,1.0f);
    //return pIn.color;
}

