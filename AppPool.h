#ifndef _POOL_APP_H_
#define _POOL_APP_H_

#include "dfcApp.h"
#include "dfcFont.h"


#define CTX_TITLE		0
#define CTX_OPTION		1
#define CTX_GAMEOVER	2
#define CTX_POOL		3
#define MAX_CTX			4

class CAppPool : public CApp
{
public:
	CAppPool();
	virtual ~CAppPool(){};

	virtual void OnPostCreate(void);

	s32		m_curLevel;
	u32		m_nbBallLeft;
	u32		m_nScore[2];
	u32		m_nHighScore;
};

extern CAppPool *g_TheAppPool;

#endif //_POOL_APP_H_
