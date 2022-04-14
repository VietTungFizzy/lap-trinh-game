#include "ScoreText.h"
#include "Sprites.h"
#include "debug.h"
void CScoreText::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(spriteId)->Draw(x, y);
}

void CScoreText::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	vy *= SCORE_TEXT_SPEED_REDUCTION_COEFFICIENT;
	if (abs(vy) < SCORE_TEXT_EPSILON) isDeleted = true;
}
