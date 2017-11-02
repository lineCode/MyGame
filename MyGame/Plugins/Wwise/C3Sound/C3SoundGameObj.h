#pragma once

#include "C3Sound.h"
#include "C3SoundBase.h"

 class  C3SoundGameObj
{
public:
	C3SoundGameObj(const char* pszObjName, C3GameObjectID gameObjectID);
	~C3SoundGameObj();
	bool LoadBank(const char* pszBankName);

	// 执行事件
	// pszEventName 事件名称
	// eCallBackType 回调类型
	// inCallBack 回调函数
	// pUserData 用户数据
	virtual C3PlayingID PostEvent(const char* pszEventName, C3CallBackType eCallBackType = 0, C3CallbackFunc inCallBack = (C3CallbackFunc)0, void* pUserData = nullptr);

	virtual C3PlayingID PostEvent(C3EventID nEventID, C3CallBackType eCallBackType = 0, C3CallbackFunc inCallBack = (C3CallbackFunc)0, void* pUserData = nullptr);



	// 设置RTPC参数
	virtual void SetRTPCValue(C3RtpcID nRtcpID, C3RtpcValue nValue);
	virtual void SetRTPCValue(const char* pszRtcpName, C3RtpcValue nValue);

	// 设置声源位置
	virtual void SetPosition(const C3SoundPosition &nPos);

	// 设置多声源位置
	virtual void SetMultiplePosition(const C3SoundPosition *nPos, UInt16 nPosNum);

	// 事件切换
	virtual void SetSwitch(const char* pszSwitchGropName, const char* pszSwitchStateName);

	virtual void SetSwitch(C3SwitchGroupID nSwitchGropID, C3SwitchStateID nSwitchStateID);

	// 声音停止
	virtual void Stop();

	// 执行触发
	virtual void PostTrigger(C3TriggerID nTriggerID);

	virtual void PostTrigger(const char* pszTriggerName);
	const char* GetGameObjectName() { return m_pszObjName; }
private:
	C3GameObjectID m_GameObjectID;
	const char* m_pszObjName;
	C3PlayingID m_PlayingID;
};