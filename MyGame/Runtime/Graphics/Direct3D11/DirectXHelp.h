#pragma once


extern ID3D11RasterizerState *      g_RS_CullNone_Fill ;
extern ID3D11RasterizerState *      g_RS_CullCCW_Fill ;
extern ID3D11RasterizerState *      g_RS_CullCW_Fill ;
extern  ID3D11RasterizerState *      g_RS_CullNone_Wireframe ;
extern  ID3D11RasterizerState *      g_RS_CullCW_Wireframe ;
extern  ID3D11RasterizerState *      g_RS_CullCCW_Wireframe ;

extern  ID3D11DepthStencilState *    g_DSS_DepthEnabledL_DepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthEnabledG_DepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthEnabledLE_NoDepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthEnabledL_NoDepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthEnabledGE_NoDepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthEnabledG_NoDepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthDisabled_NoDepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthPassAlways_DepthWrite ;
extern  ID3D11DepthStencilState *    g_DSS_DepthDisabled_StencilCreateMask ;
extern  ID3D11DepthStencilState *    g_DSS_DepthDisabled_StencilUseMask ;

extern  ID3D11BlendState *           g_BS_Opaque ;
extern  ID3D11BlendState *           g_BS_Additive ;
extern  ID3D11BlendState *           g_BS_AlphaBlend ;
extern  ID3D11BlendState *           g_BS_PremultAlphaBlend ;
extern  ID3D11BlendState *           g_BS_Mult ;

extern  ID3D11ShaderResourceView *   g_Texture2D_SRV_White1x1 ;
extern  ID3D11ShaderResourceView *   g_Texture2D_SRV_Noise2D ;
extern  ID3D11ShaderResourceView *   g_Texture2D_SRV_Noise3D ;

extern  ID3D11SamplerState *         g_samplerStatePointClamp ;
extern  ID3D11SamplerState *         g_samplerStatePointWrap ;
extern  ID3D11SamplerState *         g_samplerStatePointMirror ;
extern  ID3D11SamplerState *         g_samplerStateLinearClamp ;
extern  ID3D11SamplerState *         g_samplerStateLinearWrap ;
extern  ID3D11SamplerState *         g_samplerStateAnisotropicClamp ;
extern  ID3D11SamplerState *         g_samplerStateAnisotropicWrap ;



extern D3D11_BUFFER_DESC       g_Structured_Buffer_DESC;
