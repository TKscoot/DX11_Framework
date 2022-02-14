// Vertex input from Vertex Buffer
struct VS_IN
{
    float4 pos : POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer ConstBufPerObject
{
    float4x4 WorldViewProjection;
    float4x4 World;
    float3 CamPos;
    float padding;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 texCoords : TEXCOORD;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT output;
    
    output.pos = mul(input.pos, WorldViewProjection);
    
    output.texCoords = input.pos;
    
    return output;
}