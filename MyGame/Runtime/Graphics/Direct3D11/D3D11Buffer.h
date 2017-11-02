#pragma once
#include "dxbase.h"
#include <memory>
/************************************************************************/
/*       
typedef enum D3D11_USAGE {
  D3D11_USAGE_DEFAULT    = 0,		// GPU ��д copy ���Գ�ʼ����
  D3D11_USAGE_IMMUTABLE  = 1,	//GPU ���� ����ʱ�����ʼ��
  D3D11_USAGE_DYNAMIC    = 2,		//GPU�� CPU д��  map unmap
  D3D11_USAGE_STAGING    = 3		//CPU GPU�����Զ�д copy
} D3D11_USAGE;

�����ϸ���Լ��� D3D11_CPU_ACCESS_FLAG ��Ȼ������ʧ��
enum D3D11_CPU_ACCESS_FLAG
{
D3D11_CPU_ACCESS_WRITE	= 0x10000L,
D3D11_CPU_ACCESS_READ	= 0x20000L
} 	D3D11_CPU_ACCESS_FLAG;

                                                              */
/************************************************************************/
class D3D11Buffer
{
public:
	D3D11Buffer();
	virtual ~D3D11Buffer();
	virtual void CreateRHI() = 0;
	virtual ID3D11Buffer*GetResource() { return m_pResource; }
	virtual ID3D11Buffer**GetResourcePPtr() { return &m_pResource; }
	static void Init();
protected:
	ID3D11Buffer* m_pResource;
	D3D11_BUFFER_DESC mBufferDesc;
	D3D11_SUBRESOURCE_DATA mInitData;
};

typedef std::shared_ptr<class D3D11VertexBuffer> D3D11VertexBufferPtr;
class D3D11VertexBuffer :public D3D11Buffer
{
public:
	/// Creator. Returns NULL if error.
	static D3D11VertexBufferPtr Create(UINT ByteWidth, D3D11_USAGE Usage, UINT CPUAccessFlags,void* pSysMem =nullptr);

	virtual ~D3D11VertexBuffer();

	virtual void CreateRHI();
private:
	D3D11VertexBuffer();
};


//structured buffer. 
struct C_D3D11_STRUCTURED_BUFFER_DESC : public D3D11_BUFFER_DESC
{
	C_D3D11_STRUCTURED_BUFFER_DESC()
	{}
	explicit C_D3D11_STRUCTURED_BUFFER_DESC(const D3D11_BUFFER_DESC& o) :
		D3D11_BUFFER_DESC(o)
	{}
	explicit C_D3D11_STRUCTURED_BUFFER_DESC(
		UINT byteWidth, //���
		UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		UINT cpuaccessFlags = 0,
		UINT structureByteStride = 0//�ṹ���С
	)
	{
		ByteWidth = byteWidth;
		Usage = usage;
		BindFlags = bindFlags;
		CPUAccessFlags = cpuaccessFlags;
		MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;//����Ϊ���ֵ
		StructureByteStride = structureByteStride;//�ṹ���С
	}
	~C_D3D11_STRUCTURED_BUFFER_DESC() {}
	operator const D3D11_BUFFER_DESC&() const { return *this; }
};
