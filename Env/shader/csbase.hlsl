

struct BufferStruct {
	uint4 color;
};



// 线程组尺寸
#define thread_group_size_x 4
#define thread_group_size_y 4
RWStructuredBuffer<BufferStruct> g_OutBuff;

/* 这表示线程组中的线程数，本例中是4x4x1 = 16个线程 */
// 等价于 [numthreads( 4, 4, 1 )]
[numthreads(thread_group_size_x, thread_group_size_y, 1)]
void CS(uint3 threadIDInGroup : SV_GroupThreadID, uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex, uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int N_THREAD_GROUPS_X = 16;  // 假设等于 16， 是这么分派的 dispatch(16,16,1)

	int stride = thread_group_size_x * N_THREAD_GROUPS_X;
	// 缓冲区跨度，假设跨度就是数据宽度（没有边距）
	int idx = dispatchThreadID.y * stride + dispatchThreadID.x;

	float4 color = float4(groupID.x, groupID.y, dispatchThreadID.x, dispatchThreadID.y);

	g_OutBuff[idx].color = color;
}