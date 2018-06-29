#ifndef CTX_POOL_H
#define CTX_POOL_H

#include "dfcContext.h"
#include "dfcEntity.h"
#include "dfc3DEntity.h"
#include "dfcLabel.h"
#include "dfcSound.h"
#include "dfcMusic.h"
#include "game.h"

class CBall : public CEntity
{
public :
	CBall();
	IMPLEMENT_RTTI(Ball, Entity);

	virtual void Manage(u32 p_timeTick);

	void Appear();
	void Disappear();

	u32		m_idxBall;
	float 	m_incZoom;
};

class CHole : public  CEntity
{
public :
	CHole();
	IMPLEMENT_RTTI(Hole, Entity);
};

class CCtxPool : public CContext
{
public:
	CCtxPool(const char* p_pRes);
	~CCtxPool();

	virtual void Init(void);
	virtual void Activate(void);
	virtual void Deactivate(void);
	virtual void KeyManage(unsigned long p_nTimeTick);
	virtual void Manage(unsigned long p_nTimeTick);
	virtual CPhysObj*	UserInstall(const char *p_dataSource, const char* p_id);

	virtual void OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj);
	virtual void OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2);

	void SaveBallsPositions(void);
	void ReloadLastBallsPosition(void);

	u32 GetNbBallsChilds(void);


	CSound *m_pSoundCollideBalls;
	CSound *m_pSoundCollideWalls;
	CSound *m_pSoundWhiteFail;
	CSound *m_pSoundBallsScore;
	CMusic *m_pMusicIntro;
	CMusic *m_pMusicGame;

	CLabel	*m_pLevel;
	//Test Unit			//The test is Throw the ball at m_velStart in vector m_vecThrow and wait m_delay and see if the ball is insid or outside the table
#ifdef _UNIT_TEST
//	float	m_velStart;
//	float	m_angThrow;
//	u32		m_delay;
#endif

	s32		*m_posSaved;
	bool	*m_stateSaved;
	u32		m_nbCombines;
	u32		m_curPlayer;
	bool	m_isSecondSuccess;
	u32		*m_idxBallSuccess;
	u32		m_nbBallSuccess;

	bool	m_hasPendingReload;
	bool	m_isBallMoving;
};

#endif //CTX_POOL_H


//No Move	-> StartMove	-> EndAllMove
// Save		-> DoNothing	-> IsWhiteDead	-> ReloadPosition; (nbBall - 1) -> if(nbBall == 0)	-> goto GameOver
//																								|-> Continue;
//											|-> SavePosition; if(m_isSecondSuccess)	-> Score[m_curPlayer]+= (m_idxBallSuccess[0]+((m_nbBallSuccess-1)*10)*2)	-> if(AllEntered()	-> goto NextLevel()
//																					|-> Score[m_curPlayer]+= (m_idxBallSuccess[0]+((m_nbBallSuccess-1)*10))		->|					|-> Continue;
