#pragma once

#include "C3Sound.h"
#include "C3SoundBase.h"

 class  C3SoundGameObj
{
public:
	C3SoundGameObj(const char* pszObjName, C3GameObjectID gameObjectID);
	~C3SoundGameObj();
	bool LoadBank(const char* pszBankName);

	// ִ���¼�
	// pszEventName �¼�����
	// eCallBackType �ص�����
	// inCallBack �ص�����
	// pUserData �û�����
	virtual C3PlayingID PostEvent(const char* pszEventName, C3CallBackType eCallBackType = 0, C3CallbackFunc inCallBack = (C3CallbackFunc)0, void* pUserData = nullptr);

	virtual C3PlayingID PostEvent(C3EventID nEventID, C3CallBackType eCallBackType = 0, C3CallbackFunc inCallBack = (C3CallbackFunc)0, void* pUserData = nullptr);



	// ����RTPC����
	virtual void SetRTPCValue(C3RtpcID nRtcpID, C3RtpcValue nValue);
	virtual void SetRTPCValue(const char* pszRtcpName, C3RtpcValue nValue);

	// ������Դλ��
	virtual void SetPosition(const C3SoundPosition &nPos);

	// ���ö���Դλ��
	virtual void SetMultiplePosition(const C3SoundPosition *nPos, UInt16 nPosNum);

	// �¼��л�
	virtual void SetSwitch(const char* pszSwitchGropName, const char* pszSwitchStateName);

	virtual void SetSwitch(C3SwitchGroupID nSwitchGropID, C3SwitchStateID nSwitchStateID);

	// ����ֹͣ
	virtual void Stop();

	// ִ�д���
	virtual void PostTrigger(C3TriggerID nTriggerID);

	virtual void PostTrigger(const char* pszTriggerName);
	const char* GetGameObjectName() { return m_pszObjName; }
private:
	C3GameObjectID m_GameObjectID;
	const char* m_pszObjName;
	C3PlayingID m_PlayingID;
};