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



// 创建一个声音对象
// pszObjName 为对象名称
SOUND_API C3SoundGameObj* CreateASoundGameObj(const char* pszObjName);

// 销毁一个声音对象
SOUND_API void ClearASoundGameObj(C3SoundGameObj* pObj);

SOUND_API bool BindBank(C3SoundGameObj* pObj, const char* pszBankName);


// 设置听源位置
SOUND_API void SetListenerPosition(C3SoundPosition nPosition);



// 设置状态
// pszStateGropName 状态组名称
// pszStateName 状态名
SOUND_API void SetState(const char* pszStateGropName, const char* pszStateName);

// 暂停全局声音
SOUND_API void PauseSound();

// 恢复全局声音
SOUND_API void ResumeSound();

// 停止全局声音
SOUND_API void StopAll();

// 停止声音
// nID 所要停止的声音id
SOUND_API void StopPlayID(C3PlayingID nID);