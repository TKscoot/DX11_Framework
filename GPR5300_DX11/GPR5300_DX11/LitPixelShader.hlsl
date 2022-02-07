struct PS_IN
{
    float4 pos : POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

struct Light
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float intensity;
};

Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
SamplerState texSampler0 : register(s0);
SamplerState texSampler1 : register(s1);
SamplerState texSampler2 : register(s2);

cbuffer PerFrameConstBuf 
{
    Light light;
    float time;
    float pad0;
    float pad1;
    float pad2;
};

cbuffer MatConstBuf
{
    float4 defaultAlbedo;
    bool isTextureBound;
    float pad3;
    float pad4;
    float pad5;
};

float4 PS(PS_IN input) : SV_TARGET
{
    float2 scrolledUV = input.texCoords;
    
    scrolledUV.y += sin(time * 0.00035f) * 0.2;
    
    float4 albedo = defaultAlbedo;
    
    if (isTextureBound)
    {
        float4 color1 = tex0.Sample(texSampler0, input.texCoords);
        float4 color2 = tex1.Sample(texSampler1, scrolledUV);
        float4 blendMask = tex2.Sample(texSampler2, scrolledUV);
        
        //albedo = lerp(color1, color2, blendMask);
        
        albedo = color1;
    }
    
    // Lambert Term für Diffuse Beleuchtung
    float lambert = saturate(dot(-light.direction, normalize(input.normal)));
    float3 diffuse = lambert * (float3) light.diffuse * albedo.rgb * light.intensity;
    
    // Blinn Term für Spekulare Beleuchtung und Reflexe
    float3 reflectDir = reflect(-light.direction, normalize(input.normal));
    float3 specular = pow(saturate(dot(input.viewDirection, reflectDir)), 4.0) 
                      * light.specular.xyz * light.intensity;

    float3 finalColor = (float3) light.ambient + diffuse + specular;
    
    return float4(finalColor.rgb, albedo.a);
}