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
	strTmp += "��֧������";
	// Only works if device is created with the D3D10 or D3D11 debug layer, or when attached to PIX for Windows
	object->SetPrivateData(WKPDID_D3DDebugObjectName, strTmp.size(), strTmp.c_str());
#endif
}

void vaDirectXTools::NameObject(ID3D11Resource * resource, const char * permanentNameString)
{
#ifndef NDEBUG
	std::string strTmp = permanentNameString;
	strTmp += "��֧������";
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
	{	//��դ���׶� ʹ�õ�
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

		

		desc.FillMode = D3D11_FILL_SOLID;//���ģʽ
		desc.CullMode = D3D11_CULL_NONE;//ǰ�󶼲��޳�
		device->CreateRasterizerState(&desc, &g_RS_CullNone_Fill);



		desc.FillMode = D3D11_FILL_WIREFRAME; //�߿�ģʽ
		device->CreateRasterizerState(&desc, &g_RS_CullNone_Wireframe);

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;//�����޳�
		desc.FrontCounterClockwise = true;//��ʱ��Ϊ����
		device->CreateRasterizerState(&desc, &g_RS_CullCW_Fill);
		desc.FillMode = D3D11_FILL_WIREFRAME;
		device->CreateRasterizerState(&desc, &g_RS_CullCW_Wireframe);
 
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;//�����޳�  		//desc.CullMode = D3D11_CULL_FRONT;//�����޳�
		desc.FrontCounterClockwise = false;//��ʱ��Ϊ���� 	//desc.FrontCounterClockwise = true;//��ʱ��Ϊ����

		device->CreateRasterizerState(&desc, &g_RS_CullCCW_Fill);
		desc.FillMode = D3D11_FILL_WIREFRAME;
		device->CreateRasterizerState(&desc, &g_RS_CullCCW_Wireframe);
	}
	{	//��Ȳ���  ���Խ��Ϊtrue ˵�������ؿ���д���̨������
		//��Ȼ�����һ������δ��ͨ��ģ����ԣ���ô�������ֵҲ���ᱻд����Ȼ�������
		CD3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

		desc.DepthEnable = TRUE;//��Ȳ���
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;//ʼ�շ���true������ͨ����Ȳ���
		device->CreateDepthStencilState(&desc, &g_DSS_DepthPassAlways_DepthWrite);

		desc.DepthEnable = TRUE;
		desc.DepthFunc = D3D11_COMPARISON_LESS;//��<���������
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledL_DepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_GREATER;//��>���������
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledG_DepthWrite);


		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//��ֹд��Ȼ��� 
		desc.DepthFunc = D3D11_COMPARISON_LESS;//��<���������
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledL_NoDepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;//��<=���������
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledLE_NoDepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_GREATER;//��>���������
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledG_NoDepthWrite);

		desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;//��>=���������
		device->CreateDepthStencilState(&desc, &g_DSS_DepthEnabledGE_NoDepthWrite);

		desc.DepthEnable = FALSE;
		desc.DepthFunc = D3D11_COMPARISON_LESS;//�ر���Ȳ��ԣ���ֹд��Ȼ��� 
		device->CreateDepthStencilState(&desc, &g_DSS_DepthDisabled_NoDepthWrite);

		/************************************************************************/
		/*     	if ((StencilRef&StencilReadMask)Comparison(Value&StencilWriteMask))
		StencilRef �������õ�ֵ
		Valueģ�建�����е�ֵ
		*/
		/************************************************************************/
		//ֻ����ģ�����
		desc.StencilEnable = true;
		desc.StencilReadMask = 0xFF;//�������õ�ֵ���ֵλ�� ������Ϊ0xFF ˵���Ǳ���
		desc.StencilWriteMask = 0xFF;
		//ģ�����ʧ�ܺ�Ĳ�������������������Ϊ���ʧ�ܣ��򱣳ֲ��䣬��ΪKEEP
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;//���ֵ�ǰֵ ����
		//��Ȳ���ʧ�ܺ�Ĳ���
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//ģ�����ͨ����Ĳ���  
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;//����Ϊ�ο�ֵ
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;//��Զͨ��
		desc.BackFace = desc.FrontFace;
		device->CreateDepthStencilState(&desc, &g_DSS_DepthDisabled_StencilCreateMask);


		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;//��ȵ�ʱ��ͨ��
		desc.BackFace = desc.FrontFace;
		device->CreateDepthStencilState(&desc, &g_DSS_DepthDisabled_StencilUseMask);
	}

	{
		CD3D11_BLEND_DESC desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
		desc.RenderTarget[0].BlendEnable = false;
		device->CreateBlendState(&desc, &g_BS_Opaque);//��͸��

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //��ɫ���
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//͸�������

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		device->CreateBlendState(&desc, &g_BS_Additive);//ǰ��̨��ɫ���


		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		device->CreateBlendState(&desc, &g_BS_AlphaBlend);//����͸�����ں���ɫ


		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		device->CreateBlendState(&desc, &g_BS_PremultAlphaBlend); //��ǰ����ɫ����͸���Ȼ����Ŀ����ɫ

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		device->CreateBlendState(&desc, &g_BS_Mult);//Դ��ɫ*Ŀ����ɫ

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

