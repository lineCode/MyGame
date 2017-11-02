

struct BufferStruct {
	uint4 color;
};



// �߳���ߴ�
#define thread_group_size_x 4
#define thread_group_size_y 4
RWStructuredBuffer<BufferStruct> g_OutBuff;

/* ���ʾ�߳����е��߳�������������4x4x1 = 16���߳� */
// �ȼ��� [numthreads( 4, 4, 1 )]
[numthreads(thread_group_size_x, thread_group_size_y, 1)]
void CS(uint3 threadIDInGroup : SV_GroupThreadID, uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex, uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int N_THREAD_GROUPS_X = 16;  // ������� 16�� ����ô���ɵ� dispatch(16,16,1)

	int stride = thread_group_size_x * N_THREAD_GROUPS_X;
	// ��������ȣ������Ⱦ������ݿ�ȣ�û�б߾ࣩ
	int idx = dispatchThreadID.y * stride + dispatchThreadID.x;

	float4 color = float4(groupID.x, groupID.y, dispatchThreadID.x, dispatchThreadID.y);

	g_OutBuff[idx].color = color;
}