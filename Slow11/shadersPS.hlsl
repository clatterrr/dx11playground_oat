struct VertexOut
{
	float4 posH : SV_POSITION;
    float4 posM :POSITION;
	float3 color: COLOR;
	float3 normal : NORMAL;
	float2 uv   : TEXCOORD;
};

Texture2D g_Tex:register(t0);
SamplerState g_sam:register(s0);

struct Light
{
	float4 Ambient;
	float4 Diffuse;
    float4 Position;
	float3 Direction;
	float Pad;
};

cbuffer ConstantBufferPerPixel
{
	Light light;
};
float4 PS(VertexOut pIn) : SV_Target
{
     float3 lightamb = float3(1.0f,1.0f,1.0f) ;
     float3 lightpos = float3(-5.0f, 5.0f, 0.0f);
   float3 lightdiff = float3(1.0f, .5f, 1.0f)*1.0f;
    float3 texColor = g_Tex.Sample(g_sam, pIn.uv);
    float3 ambient = lightamb * texColor;
    float3 lightDirection = normalize(lightpos - pIn.posM.xyz);
    float3 normal = normalize(pIn.normal);
  //  float nDotL = max(dot(lightDirection, normal), 0.0);
    float nDotL = dot(lightDirection, normal);
    float3 diffuse = texColor * lightdiff * nDotL;
    return float4((diffuse + ambient)*1.5f,0.0f);
}

