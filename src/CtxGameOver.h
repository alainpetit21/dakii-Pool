#ifndef CTX_GAMEOVER_H
#define CTX_GAMEOVER_H

#include "dfcContext.h"
#include "dfcEntity.h"
#include "dfc3DEntity.h"
#include "dfcLabel.h"
#include "dfcSound.h"
#include "dfcMusic.h"
#include "game.h"

class CCtxGameOver : public CContext
{
public:
	CCtxGameOver(const char* p_pRes);
	~CCtxGameOver();

	virtual void Init(void);
	virtual void Activate(void);
	virtual void Deactivate(void);
	virtual void Manage(unsigned long p_nTimeTick);
	virtual void Update(void);
	virtual CPhysObj*	UserInstall(char *p_dataSource, char* p_id);

	virtual void OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj);
	virtual void OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2);
};

#endif //CTX_GAMEOVER_H
