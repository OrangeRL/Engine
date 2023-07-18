#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); //1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//�ʏ�
    //float4 texcolor = tex.Sample(smp, input.uv);
	//Uv���炵
	//float4 texcolor = tex.Sample(smp , input.uv + float2(0.5,0));

	////���ςڂ���
 //   float shiftPixelU = 1.0f / 1280.0f;
 //   float shiftPixelV = 1.0f / 720.0f;
	
 //   float4 color = { 0, 0, 0, 0 };
 //   int shiftValueX = 4;
 //   int shiftValueY = 4;
	
 //   for (int i = 0; i < shiftValueX; i++)
 //   {
 //       for (int j = 0; j < shiftValueY; j++)
 //       {
	
 //           color += tex.Sample(smp, input.uv + float2(-shiftPixelU * j, -shiftPixelV * i));
 //           color += tex.Sample(smp, input.uv + float2(0, -shiftPixelV * i));
 //           color += tex.Sample(smp, input.uv + float2(shiftPixelU * j, -shiftPixelV * i));
	
 //           color += tex.Sample(smp, input.uv + float2(-shiftPixelU * j, 0));
 //           color += tex.Sample(smp, input.uv + float2(0, 0));
 //           color += tex.Sample(smp, input.uv + float2(shiftPixelU * j, 0));
	
 //           color += tex.Sample(smp, input.uv + float2(-shiftPixelU * j, shiftPixelV * i));
 //           color += tex.Sample(smp, input.uv + float2(0, shiftPixelV * i));
 //           color += tex.Sample(smp, input.uv + float2(shiftPixelU * j, shiftPixelV * i));
 //       }
 //   }

 //   color.rgb /= shiftValueX * shiftValueY * 9;
 //   color.a = 1;
	
	//return color;
	
	//gara
    
    float4 colortex0 = tex0.Sample(smp, input.uv);
    float4 colortex1 = tex1.Sample(smp, input.uv);
    float4 color = colortex0;
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        color = colortex1;
    }

	//�ʏ�
    return float4(color.rgb, 1);
	//���x2�{
	//return float4(texcolor.rgb * 2.0f, 1);
	//�F���]
	//return float4(1 - texcolor.rgb , 1);
	
}