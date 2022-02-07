// Vertex input from Vertex Buffer
struct VS_IN
{
    float4 pos   : POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
};

// Send this to the Pixel Shader
struct VS_OUT
{
    float4 pos   : SV_POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

cbuffer ConstBufPerObject
{
    float4x4 WorldViewProjection;
    float4x4 World;
    float3 CamPos;
    float padding;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT output;
    output.pos = mul(input.pos, WorldViewProjection);
    output.texCoords = input.texCoords;
    output.normal = mul(input.normal, (float3x3) World);
    
    float4 worldPos = mul(input.pos, World);
    output.viewDirection = normalize(CamPos.xyz - worldPos.xyz);
    
    return output;
}