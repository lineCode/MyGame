#pragma once

#include "C3SoundBase.h"

#ifdef SOUND_EXPORTS
#define SOUND_API __declspec(dllexport)
#else
#define SOUND_API __declspec(dllimport)
#endif

class  C3SoundGameObj;

SOUND_API bool InitSoundEngine(const char* pszBaseResPath, const char* pszLanguage);

SOUND_API void ShutDownSoundEngine();

SOUND_API void ProcressSound();



// ����һ����������
// pszObjName Ϊ��������
SOUND_API C3SoundGameObj* CreateASoundGameObj(const char* pszObjName);

// ����һ����������
SOUND_API void ClearASoundGameObj(C3SoundGameObj* pObj);

SOUND_API bool BindBank(C3SoundGameObj* pObj, const char* pszBankName);


// ������Դλ��
SOUND_API void SetListenerPosition(C3SoundPosition nPosition);



// ����״̬
// pszStateGropName ״̬������
// pszStateName ״̬��
SOUND_API void SetState(const char* pszStateGropName, const char* pszStateName);

// ��ͣȫ������
SOUND_API void PauseSound();

// �ָ�ȫ������
SOUND_API void ResumeSound();

// ֹͣȫ������
SOUND_API void StopAll();

// ֹͣ����
// nID ��Ҫֹͣ������id
SOUND_API void StopPlayID(C3PlayingID nID);