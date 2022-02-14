Shader "CustomDefault"
{
	Properties
	{
		_MainTex("Main Texture", 2D) = "grey" {}
		_NormalTex("Normal Map", 2D) = "bump" {}
		_TintColor("Tint Color", Color) = (1.0, 1.0, 1.0, 1.0)
		[MaterialToggle] _SkyReflections("Sky Reflections", Int) = 1
	}
	SubShader
	{
		Tags { "RenderType"="Opaque" }

		Pass
		{
			Tags { "LightMode"="ForwardBase"}

			CGPROGRAM
			#pragma vertex vert // Wie soll unsere Vertex Main Funktion heißen
			#pragma fragment frag // Wie soll unsere Fragment/Pixel Main Funktion heißen

			#include "UnityCG.cginc"
			#include "UnityLightingCommon.cginc"
			
			struct appdata
			{
				float4 pos    : POSITION;
				float2 uv     : TEXCOORD0;
				float3 normal : NORMAL;
				float4 tangent : TANGENT;
			};

			struct v2f
			{
				float4 pos	  : SV_POSITION;
				UNITY_FOG_COORDS(1)
				float2 uv	  : TEXCOORD0;
				float3 normal : NORMAL;
				float3 worldRefl : TEXCOODRD1;
				half3 tSpaceX : TEXCOORD2;
				half3 tSpaceY : TEXCOORD3;
				half3 tSpaceZ : TEXCOORD4;
			};

			sampler2D _MainTex;
			float4 _MainTex_ST;

			sampler2D _NormalTex;
			float4 _NormalTex_ST;

			float4 _TintColor;

			int _SkyReflections;

			// Vertex Main
			v2f vert(appdata v)
			{
				v2f o;
				o.pos = UnityObjectToClipPos(v.pos); // pos * World * View * Projection
				o.uv = TRANSFORM_TEX(v.uv, _MainTex); // TRANSFORM_TEX für Tiling und Offset notwendig

				float3 wNormal = UnityObjectToWorldNormal(v.normal);
				o.normal = wNormal;

				float3 wTangent = UnityObjectToWorldDir(v.tangent.xyz);

				half tangentSign = v.tangent.w * unity_WorldTransformParams.w;
				float3 wBitangent = cross(wNormal, wTangent) * tangentSign;

				o.tSpaceX = half3(wTangent.x, wBitangent.x, wNormal.x);
				o.tSpaceY = half3(wTangent.y, wBitangent.y, wNormal.y);
				o.tSpaceZ = half3(wTangent.z, wBitangent.z, wNormal.z);

				float3 worldPos = mul(unity_ObjectToWorld, v.pos).xyz;


				float3 worldViewDir = normalize(UnityWorldSpaceViewDir(worldPos));

				o.worldRefl = reflect(-worldViewDir, wNormal);

				return o;
			}

			// Fragment Main
			float4 frag(v2f i) : SV_TARGET
			{
				float4 color = tex2D(_MainTex, i.uv);
				color *= _TintColor;


				// Tangent normal (Unpack = 0,1 Range -> -1,1 Range umwandeln)
				half3 tNormal = UnpackNormal(tex2D(_NormalTex, i.uv));

				// World Normals aus unserer Normal Map
				half3 wNormal;
				wNormal.x = dot(i.tSpaceX, tNormal);
				wNormal.y = dot(i.tSpaceY, tNormal);
				wNormal.z = dot(i.tSpaceZ, tNormal);

				wNormal = normalize(wNormal);


				float lambert = saturate(dot(wNormal, _WorldSpaceLightPos0.xyz));
				float3 diffuse = lambert * _LightColor0;

				//float3 ambient = float3(0.4, 0.4, 0.4);
				float3 ambient = ShadeSH9(float4(wNormal, 1.));

				
				color.rgb *= diffuse + ambient;
				
				if (_SkyReflections == 1)
				{
					half4 skyData = UNITY_SAMPLE_TEXCUBE(unity_SpecCube0, i.worldRefl);

					half3 skyColor = DecodeHDR(skyData, unity_SpecCube0_HDR);

					color.rgb *= skyColor.rgb;
				}


				UNITY_APPLY_FOG(i.fogCoord, color);

				return color;
			}

			ENDCG
		}
	}
}