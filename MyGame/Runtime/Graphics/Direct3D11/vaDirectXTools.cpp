#include "vaDirectXTools.h"
#include "DeviceManager.h"
#include "BaseFunc.h"

ID3D11RasterizerState *      g_RS_CullNone_Fill = NULL;
ID3D11RasterizerState *      g_RS_CullCCW_Fill = NULL;
ID3D11RasterizerState *      g_RS_CullCW_Fill = NULL;
ID3D11RasterizerState *      g_RS_CullNone_Wireframe = NULL;
ID3D11RasterizerState *      g_RS_CullCW_Wireframe = NULL;
ID3D11RasterizerState *      g_RS_CullCCW_Wireframe = NULL;

ID3D11DepthStencilState *    g_DSS_DepthEnabledL_DepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthEnabledG_DepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthEnabledLE_NoDepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthEnabledL_NoDepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthEnabledGE_NoDepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthEnabledG_NoDepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthDisabled_NoDepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthPassAlways_DepthWrite = NULL;
ID3D11DepthStencilState *    g_DSS_DepthDisabled_StencilCreateMask = NULL;
ID3D11DepthStencilState *    g_DSS_DepthDisabled_StencilUseMask = NULL;


ID3D11BlendState *           g_BS_Opaque = NULL;
ID3D11BlendState *           g_BS_Additive = NULL;
ID3D11BlendState *           g_BS_AlphaBlend = NULL;
ID3D11BlendState *           g_BS_PremultAlphaBlend = NULL;
ID3D11BlendState *           g_BS_Mult = NULL;

ID3D11ShaderResourceView *   g_Texture2D_SRV_White1x1 = NULL;
ID3D11ShaderResourceView *   g_Texture2D_SRV_Noise2D = NULL;
ID3D11ShaderResourceView *   g_Texture2D_SRV_Noise3D = NULL;


ID3D11SamplerState *         g_samplerStatePointClamp = NULL;
ID3D11SamplerState *         g_samplerStatePointWrap = NULL;
ID3D11SamplerState *         g_samplerStatePointMirror = NULL;
ID3D11SamplerState *         g_samplerStateLinearClamp = NULL;
ID3D11SamplerState *         g_samplerStateLinearWrap = NULL;
ID3D11SamplerState *         g_samplerStateAnisotropicClamp = NULL;
ID3D11SamplerState *         g_samplerStateAnisotropicWrap = NULL;

void vaDirectXTools::NameObject(ID3D11DeviceChild * object, const char * permanentNameString)
{
#ifndef NDEBUG
	std::string strTmp = permanentNameString;
	strTmp += "我支持中文";
	// Only works if device is created with the D3D10 or D3D11 debug layer, or when attached to PIX for Windows
	object->SetPrivateData(WKPDID_D3DDebugObjectName, strTmp.size(), strTmp.c_str());
#endif
}

void vaDirectXTools::NameObject(ID3D11Resource * resource, const char * permanentNameString)
{
#ifndef NDEBUG
	std::string strTmp = permanentNameString;
	strTmp += "我支持中文";
	// Only works if device is created with the D3D10 or D3D11 debug layer, or when attached to PIX for Windows
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, strTmp.size(), strTmp.c_str());
#endif
}


vaDirectXTools::vaDirectXTools()
{

}


vaDirectXTools::~vaDirectXTools()
{
	vaDirectXTools_OnDeviceDestroyed();
}

void vaDirectXTools::vaDirectXTools_OnDeviceCreated()
{
	ID3D11Device* device = g_objDeviceManager.GetDevice();
	{	//光栅化阶段 使用的
		CD3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
/*
		FillMode = D3D11_FILL_SOLID;
		CullMode = D3D11_CULL_BACK;
		FrontCounterClockwise = FALSE;
		DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
		DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
		SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		DepthClipEnable = TRUE;
		ScissorEnable = FALSE;
		MultisampleEnable = FALSE;
		AntialiasedLineEnable = FALSE;
		*/

		

		desc.FillMode = D3D11_FILL_SOLID;//填充模式
		desc.CullMode = D3D11_CULL_NONE;//前后都不剔除
		device->CreateRasterizerState(&desc, &g_RS_CullNone_Fill);



		desc.FillMode = D3D11_FILL_WIREFRAME; //线框模式
		device->CreateRasterizerState(&desc, &g_RS_CullNone_Wireframe);

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;//背面剔除
		desc.FrontCounterClockwise = true;//逆时针为正面
		device->CreateRasterizerState(&desc, &g_RS_CullCW_Fill);
		desc.FillMode = D3D11_FILL_WIREFRAME;
		device->CreateRasterizerState(&desc, &g_RS_CullCW_Wireframe);
 
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;//背面剔除  		//desc.CullMode = D3D11_CULL_FRONT;//正面剔除
		desc.FrontCounterClockwise = false;//逆时针为背面 	//desc.FrontCounterClockwise = true;//逆时针为正面

		device->CreateRasterizerState(&desc, &g_RS_CullCCW_Fill);
		desc.FillMode = D3D11_FILL_WIREFRAME;
		device->CreateRasterizerState(&desc, &g_RS_CullCCW_Wireframe);
	}
	{	//深度测试  测试结果为true 说明该像素可以写入后台缓冲区
		//当然，如果一个像素未能通过模板测试，那么它的深度值也不会被写入深度缓冲区。
		CD3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

		desc.DepthEnable = TRUE;//深度测试
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;//始终返回true。总是通过深度测试
		device->CreateDepthStencilState(&desc, &g_DSS_DepthPassAlways_DepthWrite);

		desc.DepthEnable = TRUE;
		desc.DepthFunc = D3D11_COMPARISON_LESS;//用<运算符代替
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledL_DepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_GREATER;//用>运算符代替
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledG_DepthWrite);


		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//禁止写深度缓冲 
		desc.DepthFunc = D3D11_COMPARISON_LESS;//用<运算符代替
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledL_NoDepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;//用<=运算符代替
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledLE_NoDepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_GREATER;//用>运算符代替
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledG_NoDepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;//用>=运算符代替
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledGE_NoDepthWrite);

		desc.DepthEnable = FALSE;
		desc.DepthFunc = D3D11_COMPARISON_LESS;//关闭深度测试，禁止写深度缓冲 
		device->CreateDepthStencilState(&desc, &g_DSS_DepthDisabled_NoDepthWrite);

		/************************************************************************/
		/*     	if ((StencilRef&StencilReadMask)Comparison(Value&StencilWriteMask))
		StencilRef 程序设置的值
		Value模板缓冲区中的值
		*/
		/************************************************************************/
		//只开启模板测试
		desc.StencilEnable = true;
		desc.StencilReadMask = 0xFF;//程序设置的值如该值位于 ，掩码为0xFF 说明是本身
		desc.StencilWriteMask = 0xFF;
		//模板测试失败后的操作，比如我们想设置为如果失败，则保持不变，则为KEEP
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;//保持当前值 不变
		//深度测试失败后的操作
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//模板测试通过后的操作  
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;//更新为参考值
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;//永远通过
		desc.BackFace = desc.FrontFace;
		device->CreateDepthStencilState(&desc, &g_DSS_DepthDisabled_StencilCreateMask);


		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;//相等的时候通过
		desc.BackFace = desc.FrontFace;
		device->CreateDepthStencilState(&desc, &g_DSS_DepthDisabled_StencilUseMask);
	}

	{
		CD3D11_BLEND_DESC desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
		desc.RenderTarget[0].BlendEnable = false;
		device->CreateBlendState(&desc, &g_BS_Opaque);//不透明

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //颜色相加
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//透明度相加

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		device->CreateBlendState(&desc, &g_BS_Additive);//前后台颜色相加


		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		device->CreateBlendState(&desc, &g_BS_AlphaBlend);//根据透明度融合颜色


		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		device->CreateBlendState(&desc, &g_BS_PremultAlphaBlend); //当前的颜色根据透明度混合上目标颜色

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		device->CreateBlendState(&desc, &g_BS_Mult);//源颜色*目标颜色

	}

	{  // samplers
		CD3D11_SAMPLER_DESC desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		device->CreateSamplerState(&desc, &g_samplerStatePointClamp);
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		device->CreateSamplerState(&desc, &g_samplerStatePointWrap);
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		device->CreateSamplerState(&desc, &g_samplerStatePointMirror);


		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		device->CreateSamplerState(&desc, &g_samplerStateLinearClamp);
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		device->CreateSamplerState(&desc, &g_samplerStateLinearWrap);

		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		device->CreateSamplerState(&desc, &g_samplerStateAnisotropicClamp);
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		device->CreateSamplerState(&desc, &g_samplerStateAnisotropicWrap);


	}
}

void vaDirectXTools::vaDirectXTools_OnDeviceDestroyed()
{
	SAFE_RELEASE(g_RS_CullNone_Fill);
	SAFE_RELEASE(g_RS_CullCCW_Fill);
	SAFE_RELEASE(g_RS_CullCW_Fill);
	SAFE_RELEASE(g_RS_CullNone_Wireframe);
	SAFE_RELEASE(g_RS_CullCW_Wireframe);
	SAFE_RELEASE(g_RS_CullCCW_Wireframe);


	SAFE_RELEASE(g_DSS_DepthEnabledL_DepthWrite);
	SAFE_RELEASE(g_DSS_DepthEnabledG_DepthWrite);
	SAFE_RELEASE(g_DSS_DepthEnabledL_NoDepthWrite);
	SAFE_RELEASE(g_DSS_DepthEnabledLE_NoDepthWrite);
	SAFE_RELEASE(g_DSS_DepthEnabledG_NoDepthWrite);
	SAFE_RELEASE(g_DSS_DepthEnabledGE_NoDepthWrite);
	SAFE_RELEASE(g_DSS_DepthDisabled_NoDepthWrite);
	SAFE_RELEASE(g_DSS_DepthPassAlways_DepthWrite);
	SAFE_RELEASE(g_DSS_DepthDisabled_StencilCreateMask);
	SAFE_RELEASE(g_DSS_DepthDisabled_StencilUseMask);

	SAFE_RELEASE(g_BS_Opaque);
	SAFE_RELEASE(g_BS_Additive);
	SAFE_RELEASE(g_BS_AlphaBlend);
	SAFE_RELEASE(g_BS_PremultAlphaBlend);
	SAFE_RELEASE(g_BS_Mult);

	SAFE_RELEASE(g_Texture2D_SRV_White1x1);
	SAFE_RELEASE(g_Texture2D_SRV_Noise2D);
	SAFE_RELEASE(g_Texture2D_SRV_Noise3D);

	SAFE_RELEASE(g_samplerStatePointClamp);
	SAFE_RELEASE(g_samplerStatePointWrap);
	SAFE_RELEASE(g_samplerStatePointMirror);
	SAFE_RELEASE(g_samplerStateLinearClamp);
	SAFE_RELEASE(g_samplerStateLinearWrap);
	SAFE_RELEASE(g_samplerStateAnisotropicClamp);
	SAFE_RELEASE(g_samplerStateAnisotropicWrap);

}

