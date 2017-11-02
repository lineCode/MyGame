#pragma once


typedef unsigned long		C3StateGroupID;
typedef unsigned long		C3PlayingID;
typedef unsigned long		C3StateID;
typedef unsigned long		C3PlayingID;
typedef float				C3RtpcValue;
typedef unsigned long       C3RtpcID;
typedef unsigned long       C3EventID;
typedef unsigned long		C3CallBackType;
typedef unsigned long		C3SwitchGroupID;
typedef unsigned long		C3SwitchStateID;
typedef unsigned long		C3TriggerID;
typedef unsigned short		UInt16;
typedef unsigned long		AkUniqueID;

typedef unsigned __int64	C3GameObjectID;			///< Unsigned 64-bit integer


													//CALLBACK

enum C3CallbackType
{
	C3_EndOfEvent = 0x0001,	///< Callback triggered when reaching the end of an event.
	C3_EndOfDynamicSequenceItem = 0x0002,	///< Callback triggered when reaching the end of a dynamic sequence item.
	C3_Marker = 0x0004,	///< Callback triggered when encountering a marker during playback.
	C3_Duration = 0x0008,	///< Callback triggered when the duration of the sound is known by the sound engine.

	C3_SpeakerVolumeMatrix = 0x0010,   ///< Callback triggered at each frame, letting the client modify the speaker volume matrix

	C3_Starvation = 0x0020,	///< Callback triggered when playback skips a frame due to stream starvation.

	C3_MusicPlaylistSelect = 0x0040,	///< Callback triggered when music playlist container must select the next item to play.
	C3_MusicPlayStarted = 0x0080,	///< Callback triggered when a "Play" or "Seek" command has been executed . Applies to objects of the Interactive-Music Hierarchy only.

	C3_MusicSyncBeat = 0x0100,	///< Enable notifications on Music Beat.
	C3_MusicSyncBar = 0x0200,	///< Enable notifications on Music Bar.
	C3_MusicSyncEntry = 0x0400,	///< Enable notifications on Music Entry Cue.
	C3_MusicSyncExit = 0x0800,	///< Enable notifications on Music Exit Cue.
	C3_MusicSyncGrid = 0x1000,	///< Enable notifications on Music Grid.
	C3_MusicSyncUserCue = 0x2000,	///< Enable notifications on Music Custom Cue.
	C3_MusicSyncPoint = 0x4000,	///< Enable notifications on Music switch transition synchronization point.
	C3_MusicSyncAll = 0x7f00,	///< Use this flag if you want to receive all notifications concerning AK_MusicSync registration.

	C3_MIDIEvent = 0x10000,	///< Enable notifications for MIDI events.

	C3_CallbackBits = 0xfffff,	///< Bitmask for all callback types.

								// Not callback types, but need to be part of same bitfield for PostEvent().
								C3_EnableGetSourcePlayPosition = 0x100000,	///< Enable play position information for use by GetSourcePlayPosition().
								C3_EnableGetMusicPlayPosition = 0x200000,	///< Enable play position information of music objects, queried via GetPlayingSegmentInfo().
								C3_EnableGetSourceStreamBuffering = 0x400000	///< Enable stream buffering information for use by GetSourceStreamBuffering(). 
};

struct C3CallbackInfo
{
	void *			pCookie;		///< User data, passed to PostEvent()
	C3GameObjectID	gameObjID;		///< Game object ID
};

struct C3EventCallbackInfo : public C3CallbackInfo
{
	C3PlayingID		playingID;		///< Playing ID of Event, returned by PostEvent()
	C3EventID		eventID;		///< Unique ID of Event, passed to PostEvent()
};


#define C3_CALLBACK( __TYPE__, __NAME__ ) typedef __TYPE__ ( *__NAME__ )

C3_CALLBACK(void, C3CallbackFunc)(
	C3CallbackType in_eType,
	C3CallbackInfo* in_pCallbackInfo
	);
													/// 3D vector.
struct C3Vector
{
	float		X;	///< X Position
	float		Y;	///< Y Position
	float		Z;	///< Z Position
};

// Position and orientation of game objects.
class C3SoundPosition
{
public:
	//
	// Getters.
	//

	/// Get position vector.
	inline const C3Vector & Position() const
	{
		return position;
	}

	/// Get orientation front vector.
	inline const C3Vector & OrientationFront() const
	{
		return orientationFront;
	}

	/// Get orientation top vector.
	inline const C3Vector & OrientationTop() const
	{
		return orientationTop;
	}

	//
	// Setters.
	//

	/// Set position and orientation. Orientation front and top should be orthogonal and normalized.
	inline void Set(
		const C3Vector & in_position,			///< Position vector.
		const C3Vector & in_orientationFront,	///< Orientation front
		const C3Vector & in_orientationTop		///< Orientation top
	)
	{
		position = in_position;
		orientationFront = in_orientationFront;
		orientationTop = in_orientationTop;
	}

	/// Set position and orientation. Orientation front and top should be orthogonal and normalized.
	inline void Set(
		float in_positionX,					///< Position x
		float in_positionY,					///< Position y
		float in_positionZ,					///< Position z
		float in_orientFrontX,				///< Orientation front x
		float in_orientFrontY,				///< Orientation front y
		float in_orientFrontZ,				///< Orientation front z
		float in_orientTopX,					///< Orientation top x
		float in_orientTopY,					///< Orientation top y
		float in_orientTopZ					///< Orientation top z
	)
	{
		position.X = in_positionX;
		position.Y = in_positionY;
		position.Z = in_positionZ;
		orientationFront.X = in_orientFrontX;
		orientationFront.Y = in_orientFrontY;
		orientationFront.Z = in_orientFrontZ;
		orientationTop.X = in_orientTopX;
		orientationTop.Y = in_orientTopY;
		orientationTop.Z = in_orientTopZ;
	}

	/// Set position.
	inline void SetPosition(
		const C3Vector & in_position			///< Position vector.
	)
	{
		position = in_position;
	}

	/// Set position.
	inline void SetPosition(
		float in_x,							///< x
		float in_y,							///< y
		float in_z							///< z
	)
	{
		position.X = in_x;
		position.Y = in_y;
		position.Z = in_z;
	}

	/// Set orientation. Orientation front and top should be orthogonal and normalized.
	inline void SetOrientation(
		const C3Vector & in_orientationFront,	///< Orientation front
		const C3Vector & in_orientationTop		///< Orientation top
	)
	{
		orientationFront = in_orientationFront;
		orientationTop = in_orientationTop;
	}

	/// Set orientation. Orientation front and top should be orthogonal and normalized.
	inline void SetOrientation(
		float in_orientFrontX,				///< Orientation front x
		float in_orientFrontY,				///< Orientation front y
		float in_orientFrontZ,				///< Orientation front z
		float in_orientTopX,					///< Orientation top x
		float in_orientTopY,					///< Orientation top y
		float in_orientTopZ					///< Orientation top z
	)
	{
		orientationFront.X = in_orientFrontX;
		orientationFront.Y = in_orientFrontY;
		orientationFront.Z = in_orientFrontZ;
		orientationTop.X = in_orientTopX;
		orientationTop.Y = in_orientTopY;
		orientationTop.Z = in_orientTopZ;
	}

private:
	C3Vector		orientationFront;	///< Orientation of the listener
	C3Vector		orientationTop;		///< Top orientation of the listener
	C3Vector		position;			///< Position of the listener
};


