#ifndef CTX_TITLE_H
#define CTX_TITLE_H

#include "dfcContext.h"
#include "dfcEntity.h"
#include "dfc3DEntity.h"
#include "dfcLabel.h"
#include "dfcSound.h"
#include "dfcMusic.h"
#include "game.h"

class CCtxTitle : public CContext
{
public:
	CCtxTitle(const char* p_pRes);
	~CCtxTitle();

	virtual void Init(void);
	virtual void Activate(void);
	virtual void Deactivate(void);
	virtual void Manage(unsigned long p_nTimeTick);
	virtual void Update(void);
	virtual CPhysObj*	UserInstall(char *p_dataSource, char* p_id);

	virtual void OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj);
	virtual void OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2);

	CMusic *m_pMusicTitle;
};

#endif //CTX_TITLE_H
