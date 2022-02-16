struct PS_IN
{
    float4 pos   : POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;

};

float4 PS(PS_IN input) : SV_TARGET
{
    return float4(0.0, 1.0, 0.0, 1.0);
}