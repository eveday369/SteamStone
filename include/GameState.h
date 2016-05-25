#pragma once

#include "BaseApplication.h"
#include "stdafx.h"

class Game;
class CGameState
{
private:
	Game*				m_pParent;
	DWORD				m_dwPKey;
	Ogre::String		m_szPlayerName;
public:
	CGameState(Game* pParent)
	{
		m_pParent = pParent;
	}
	virtual ~CGameState()
	{
		m_pParent = NULL;
	}
	virtual void Run(const Ogre::FrameEvent &evt)			= 0;
	virtual void keyPressed(const OIS::KeyEvent &_arg)		= 0;
	virtual void keyReleased(const OIS::KeyEvent &_arg)		= 0;

	virtual bool CheckData()								= 0;

	inline DWORD		GetPKey()							{ return m_dwPKey; }
	inline void			SetPKey(DWORD pKey)					{ m_dwPKey = pKey; }
	inline Ogre::String	GetPlayerName()						{ return m_szPlayerName; }
	inline void			SetPlayerName(Ogre::String name)	{ m_szPlayerName = name; }
	inline Game*		GetGame()							{ return m_pParent; }
	inline void			SetGame(Game* p)					{ m_pParent = p; }
};