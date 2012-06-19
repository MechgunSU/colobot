// * This file is part of the COLOBOT source code
// * Copyright (C) 2001-2008, Daniel ROUX & EPSITEC SA, www.epsitec.ch
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.

// camera.h

#pragma once


#include "common/event.h"
#include "math/point.h"
#include "graphics/d3d/d3dengine.h"


class CInstanceManager;
class CD3DEngine;
class CTerrain;
class CWater;
class CObject;


enum CameraType
{
	CAMERA_NULL		= 0,	// camera undefined
	CAMERA_FREE		= 1,	// camera free (never in principle)
	CAMERA_EDIT		= 2,	// camera while editing a program
	CAMERA_ONBOARD		= 3,	// camera on board a robot
	CAMERA_BACK		= 4,	// camera behind a robot
	CAMERA_FIX		= 5,	// static camera following robot
	CAMERA_EXPLO		= 6,	// camera steady after explosion
	CAMERA_SCRIPT		= 7,	// camera during a film script
	CAMERA_INFO		= 8,	// camera for displaying information
	CAMERA_VISIT		= 9,	// visit instead of an error
	CAMERA_DIALOG		= 10,	// camera for dialogue
	CAMERA_PLANE		= 11,	// static camera height
};

enum CameraSmooth
{
	CS_NONE			= 0,	// sharp
	CS_NORM			= 1,	// normal
	CS_HARD			= 2,	// hard
	CS_SPEC			= 3,	// special
};

enum CenteringPhase
{
	CP_NULL			= 0,
	CP_START		= 1,
	CP_WAIT			= 2,
	CP_STOP			= 3,
};

enum CameraEffect
{
	CE_NULL			= 0,	// no effect
	CE_TERRAFORM		= 1,	// digging in
	CE_CRASH		= 2,	// Vehicle driving is severely
	CE_EXPLO		= 3,	// explosion
	CE_SHOT			= 4,	// not mortal shot
	CE_VIBRATION		= 5,	// vibration during construction
	CE_PET			= 6,	// spleen reactor
};

enum OverEffect
{
	OE_NULL			= 0,	// no effect
	OE_BLOOD		= 1,	// flash red
	OE_FADEINw		= 2,	// white -> nothing
	OE_FADEOUTw		= 3,	// nothing -> white
	OE_FADEOUTb		= 4,	// nothing -> blue
	OE_BLITZ		= 5,	// lightning
};



class CCamera
{
public:
	CCamera(CInstanceManager* iMan);
	~CCamera();

	bool		EventProcess(const Event &event);

	void		Init(Math::Vector eye, Math::Vector lookat, float delay);

	void		SetObject(CObject* object);
	CObject*	RetObject();

	void		SetType(CameraType type);
	CameraType	RetType();

	void		SetSmooth(CameraSmooth type);
	CameraSmooth RetSmoth();

	void		SetDist(float dist);
	float		RetDist();

	void		SetFixDirection(float angle);
	float		RetFixDirection();

	void		SetRemotePan(float value);
	float		RetRemotePan();

	void		SetRemoteZoom(float value);
	float		RetRemoteZoom();

	void		StartVisit(Math::Vector goal, float dist);
	void		StopVisit();

	void		RetCamera(Math::Vector &eye, Math::Vector &lookat);

	bool		StartCentering(CObject *object, float angleH, float angleV, float dist, float time);
	bool		StopCentering(CObject *object, float time);
	void		AbortCentering();

	void		FlushEffect();
	void		StartEffect(CameraEffect effect, Math::Vector pos, float force);

	void		FlushOver();
	void		SetOverBaseColor(D3DCOLORVALUE color);
	void		StartOver(OverEffect effect, Math::Vector pos, float force);

	void		FixCamera();
	void		SetScriptEye(Math::Vector eye);
	void		SetScriptLookat(Math::Vector lookat);

	void		SetEffect(bool bEnable);
	void		SetCameraScroll(bool bScroll);
	void		SetCameraInvertX(bool bInvert);
	void		SetCameraInvertY(bool bInvert);

	float		RetMotorTurn();
	D3DMouse	RetMouseDef(Math::Point pos);

protected:
	bool		EventMouseMove(const Event &event);
	void		EventMouseWheel(int dir);
	bool		EventFrame(const Event &event);
	bool		EventFrameFree(const Event &event);
	bool		EventFrameEdit(const Event &event);
	bool		EventFrameDialog(const Event &event);
	bool		EventFrameBack(const Event &event);
	bool		EventFrameFix(const Event &event);
	bool		EventFrameExplo(const Event &event);
	bool		EventFrameOnBoard(const Event &event);
	bool		EventFrameInfo(const Event &event);
	bool		EventFrameVisit(const Event &event);
	bool		EventFrameScript(const Event &event);

	void		SetViewTime(const Math::Vector &vEyePt, const Math::Vector &vLookatPt, float rTime);
	bool		IsCollision(Math::Vector &eye, Math::Vector lookat);
	bool		IsCollisionBack(Math::Vector &eye, Math::Vector lookat);
	bool		IsCollisionFix(Math::Vector &eye, Math::Vector lookat);

	Math::Vector	ExcludeTerrain(Math::Vector eye, Math::Vector lookat, float &angleH, float &angleV);
	Math::Vector	ExcludeObject(Math::Vector eye, Math::Vector lookat, float &angleH, float &angleV);

	void		SetViewParams(const Math::Vector &eye, const Math::Vector &lookat, const Math::Vector &up);
	void		EffectFrame(const Event &event);
	void		OverFrame(const Event &event);

protected:
	CInstanceManager* m_iMan;
	CD3DEngine*	m_engine;
	CTerrain*	m_terrain;
	CWater*		m_water;

	CameraType	m_type;			// the type of camera (CAMERA *)
	CameraSmooth	m_smooth;		// type of smoothing
	CObject*	m_cameraObj;		// object linked to the camera

	float		m_eyeDistance;		// distance between the eyes
	float		m_initDelay;		// time of initial centering

	Math::Vector	m_actualEye;		// current eye
	Math::Vector	m_actualLookat;		// aim current
	Math::Vector	m_finalEye;		// final eye
	Math::Vector	m_finalLookat;		// aim final
	Math::Vector	m_normEye;		// normal eye
	Math::Vector	m_normLookat;		// aim normal
	float		m_focus;

	bool		m_bRightDown;
	Math::Point		m_rightPosInit;
	Math::Point		m_rightPosCenter;
	Math::Point		m_rightPosMove;

	Math::Vector	m_eyePt;		// CAMERA_FREE: eye
	float		m_directionH;		// CAMERA_FREE: horizontal direction
	float		m_directionV;		// CAMERA_FREE: vertical direction
	float		m_heightEye;		// CAMERA_FREE: height above the ground
	float		m_heightLookat;		// CAMERA_FREE: height above the ground
	float		m_speed;		// CAMERA_FREE: speed of movement

	float		m_backDist;		// CAMERA_BACK: distance
	float		m_backMin;		// CAMERA_BACK: distance minimal
	float		m_addDirectionH;	// CAMERA_BACK: additional direction
	float		m_addDirectionV;	// CAMERA_BACK: additional direction
	bool		m_bTransparency;

	float		m_fixDist;		// CAMERA_FIX: distance
	float		m_fixDirectionH;	// CAMERA_FIX: direction
	float		m_fixDirectionV;	// CAMERA_FIX: direction

	Math::Vector	m_visitGoal;		// CAMERA_VISIT: target position
	float		m_visitDist;		// CAMERA_VISIT: distance
	float		m_visitTime;		// CAMERA_VISIT: relative time
	CameraType	m_visitType;		// CAMERA_VISIT: initial type
	float		m_visitDirectionH;	// CAMERA_VISIT: direction
	float		m_visitDirectionV;	// CAMERA_VISIT: direction

	float		m_editHeight;		// CAMERA_EDIT: height

	float		m_remotePan;
	float		m_remoteZoom;

	Math::Point		m_mousePos;
	float		m_mouseDirH;
	float		m_mouseDirV;
	float		m_mouseMarging;

	float		m_motorTurn;

	CenteringPhase m_centeringPhase;
	float		m_centeringAngleH;
	float		m_centeringAngleV;
	float		m_centeringDist;
	float		m_centeringCurrentH;
	float		m_centeringCurrentV;
	float		m_centeringTime;
	float		m_centeringProgress;

	CameraEffect m_effectType;
	Math::Vector	m_effectPos;
	float		m_effectForce;
	float		m_effectProgress;
	Math::Vector	m_effectOffset;

	OverEffect	m_overType;
	float		m_overForce;
	float		m_overTime;
	D3DCOLORVALUE m_overColorBase;
	D3DCOLORVALUE m_overColor;
	int			m_overMode;
	float		m_overFadeIn;
	float		m_overFadeOut;

	Math::Vector	m_scriptEye;
	Math::Vector	m_scriptLookat;

	bool		m_bEffect;		// shocks if explosion?
	bool		m_bCameraScroll;	// scroll in the edges?
	bool		m_bCameraInvertX;	// X inversion in the edges?
	bool		m_bCameraInvertY;	// Y inversion in the edges?
};

