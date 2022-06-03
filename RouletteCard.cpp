#include "RouletteCard.h"
#include "PlayScene.h"
#include "Text.h"

void CRouletteCard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - ROULETTE_CARD_BBOX_WIDTH / 2;
	top = y - ROULETTE_CARD_BBOX_HEIGHT / 2;
	right = left + ROULETTE_CARD_BBOX_WIDTH;
	bottom = top + ROULETTE_CARD_BBOX_HEIGHT;
}

void CRouletteCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;

	if (GetTickCount64() - timer > ROULETTE_CARD_LIVING_TIME) {
		CGame* game = CGame::GetInstance();
		CPlayScene* scene = (CPlayScene*)game->GetCurrentScene();
		float cam_l, cam_t;
		float text_x, text_y;
		std::string content;
		game->GetCamPos(cam_l, cam_t);

		// Create text object for line 'COURSE CLEAR !'
		content = TEXT_CONTENT_0;
		CText * line_1 = new CText(
			cam_l + game->GetBackBufferWidth() / 2, 
			cam_t + game->GetBackBufferHeight() / 4, 
			OBJECT_TYPE_TEXT, 
			content);

		// Align text object to middle of the screen
		line_1->GetPosition(text_x, text_y);
		text_x -= (content.size() * CHARACTER_BBOX_WIDTH) / 2;
		line_1->SetPosition(text_x, text_y);

		// Add text object to scene
		scene->AddObjects(line_1);

		// Create text object for line 'YOU GOT A CARD'
		content = TEXT_CONTENT_1;
		CText* line_2 = new CText(
			cam_l + game->GetBackBufferWidth() / 2, 
			cam_t + game->GetBackBufferHeight() / 4, 
			OBJECT_TYPE_TEXT, 
			content);

		// Align text object to middle of the screen (include the card)
		line_2->GetPosition(text_x, text_y);
		text_x -= ((content.size() * CHARACTER_BBOX_WIDTH) / 2 + CHARACTER_BBOX_WIDTH * 2);
		text_y += CHARACTER_BBOX_HEIGHT * 3;
		line_2->SetPosition(text_x, text_y);

		// Add text object to scene
		scene->AddObjects(line_2);

		// Create Card Object


		// Adjust Card Object position

		
		// Add card object to scene


		Delete();
	}
}

void CRouletteCard::Render()
{
	int aniId = ID_ANI_ROULETTE_CARD_MUSROOM;
	switch (state) {
	case ROULETTE_CARD_STATE_MUSHROOM: aniId = ID_ANI_ROULETTE_CARD_MUSROOM; break;
	case ROULETTE_CARD_STATE_FLOWER: aniId = ID_ANI_ROULETTE_CARD_FLOWER; break;
	case ROULETTE_CARD_STATE_STAR: aniId = ID_ANI_ROULETTE_CARD_STAR; break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
