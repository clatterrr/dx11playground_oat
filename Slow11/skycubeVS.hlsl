
cbuffer tcbuffer
{
	float4x4 MVP;
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProj;
	float4 gEyePos;
};
struct VertexIn
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float3 col : COLOR;
	float2 uv  :TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION; 
	float3 PosL : POSITION;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(float4(vIn.pos,1.0),MVP);

	vOut.posH = mul(float4(vIn.pos, 1.0), gWorld);
	//vOut.posH += gEyePos;
	vOut.posH = mul(vOut.posH, gView * gProj);
	vOut.posH = vOut.posH.xyww;
	vOut.PosL = vIn.pos;
	return vOut;
}