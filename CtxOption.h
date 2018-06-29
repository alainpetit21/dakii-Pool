#ifndef CTX_OPTION_H
#define CTX_OPTION_H

#include "dfcContext.h"
#include "dfcEntity.h"
#include "dfc3DEntity.h"
#include "dfcLabel.h"
#include "dfcSound.h"
#include "dfcMusic.h"
#include "game.h"

class CCtxOption : public CContext
{
public:
	CCtxOption(const char* p_pRes);
	~CCtxOption();

	virtual void Init(void);
	virtual void Activate(void);
	virtual void Deactivate(void);
	virtual void Manage(unsigned long p_nTimeTick);
	virtual void Update(void);

	CMusic *m_pMusicOption;
};

#endif //CTX_OPTION_H
