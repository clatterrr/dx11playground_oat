
cbuffer tcbuffer
{
	float4x4 MVP;
};
struct VertexIn
{
	float4 pos : POSITION;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(vIn.pos,MVP);
	return vOut;
}