#include <cstdio>

#include "Hud.h"
#include "Text.h"
#include "Card.h"
#include "PowerMeter.h"
#include "Decorated_Obj.h"

#include "GlobalState.h"
#include "AssetIDs.h"

#define BUFFER_MAX_SIZE 100

std::string CHud::convertIntToString(int length, int content)
{
	char buffer[BUFFER_MAX_SIZE];

	sprintf_s(buffer, "%0*d", length, content);
	return std::string(buffer);
}

void CHud::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - HUD_BBOX_WIDTH / 2;
	top = y - HUD_BBOX_HEIGHT / 2;
	right = left + HUD_BBOX_WIDTH;
	bottom = top + HUD_BBOX_HEIGHT;
}

void CHud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGlobalState* gt = CGlobalState::GetInstance();
	std::string t;

	t = std::to_string(gt->lives);
	((CText*)livesText)->UpdateText(t);

	t = std::to_string(gt->world);
	((CText*)worldText)->UpdateText(t);

	t = std::to_string(gt->coin);
	((CText*)coinText)->UpdateText(t);

	t = convertIntToString(HUD_POINT_SIZE, gt->point);
	((CText*)pointText)->UpdateText(t);

	t = convertIntToString(HUD_TIME_SIZE, gt->time);
	((CText*)timeText)->UpdateText(t);

	for (int i = 0; i < HUD_AMOUNT_OF_CARD; i++) {
		cards[i]->SetState(gt->cards[i]);
	}
}

void CHud::Render()
{
	// Render background first
	background->Render();

	CSprites* sprites = CSprites::GetInstance();
	// Render HUD Skeleton
	sprites->Get(ID_SPRITE_HUD_SKELETON)->Draw(x - HUD_SKELETON_OFFSET_X, y);

	// Render HUD Components
	livesText->Render();
	worldText->Render();
	coinText->Render();
	pointText->Render();
	timeText->Render();
	powerMeter->Render();
	
	sprites->Get(ID_SPRITE_HUD_M_BUTTON)->Draw(
		x - HUD_BBOX_WIDTH / 2 + HUD_LOCAL_COORDINATE_OFFSET + HUD_M_BUTTON_OFFSET_X,
		y - HUD_BBOX_HEIGHT / 2 + HUD_LOCAL_COORDINATE_OFFSET + HUD_M_BUTTON_OFFSET_Y);

	for (int i = 0; i < HUD_AMOUNT_OF_CARD; i++) {
		cards[i]->Render();
	}

	// RenderBoundingBox();
}

CHud::CHud(float x, float y, int type)
{
	this->x = x;
	this->y = y;
	this->type = type;
	float base_x = this->x - HUD_BBOX_WIDTH / 2 + HUD_LOCAL_COORDINATE_OFFSET;
	float base_y = this->y - HUD_BBOX_HEIGHT / 2 + HUD_LOCAL_COORDINATE_OFFSET;
	CGlobalState* gt = CGlobalState::GetInstance();
	
	std::string t = std::to_string(gt->lives);
	livesText = new CText(base_x + HUD_LIVES_TEXT_OFFSET_X, base_y + HUD_LIVES_TEXT_OFFSET_Y, OBJECT_TYPE_TEXT, t);

	t = std::to_string(gt->world);
	worldText = new CText(base_x + HUD_WORLD_TEXT_OFFSET_X, base_y + HUD_WORLD_TEXT_OFFSET_Y, OBJECT_TYPE_TEXT, t);

	t = std::to_string(gt->coin);
	coinText = new CText(base_x + HUD_COIN_TEXT_OFFSET_X, base_y + HUD_COIN_TEXT_OFFSET_Y, OBJECT_TYPE_TEXT, t);

	t = convertIntToString(HUD_POINT_SIZE, gt->point);
	pointText = new CText(base_x + HUD_POINT_TEXT_OFFSET_X, base_y + HUD_POINT_TEXT_OFFSET_Y, OBJECT_TYPE_TEXT, t);

	t = convertIntToString(HUD_TIME_SIZE, gt->time);
	timeText = new CText(base_x + HUD_TIME_TEXT_OFFSET_X, base_y + HUD_TIME_TEXT_OFFSET_Y, OBJECT_TYPE_TEXT, t);

	for (int i = 0; i < HUD_AMOUNT_OF_CARD; i++) {
		cards[i] = new CCard(base_x + HUD_CARDS_OFFSET_X + CARD_BBOX_WIDTH * i, base_y + HUD_CARDS_OFFSET_Y, OBJECT_TYPE_CARD);
		cards[i]->SetState(gt->cards[i]);
	}

	powerMeter = new CPowerMeter(base_x + HUD_POWER_METER_OFFSET_X, base_y + HUD_POWER_METER_OFFSET_Y, OBJECT_TYPE_POWER_METER);

	background = new CDecoratedObject(base_x - HUD_BACKGROUND_OFFSET_X, base_y, ID_SPRITE_HUD_BACKGROUND, OBJECT_TYPE_DECORATED, HUD_BACKGROUND_CELL_LENGTH, HUD_BACKGROUND_CELL_HEIGHT);
}

void CHud::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	float base_x = this->x - HUD_BBOX_WIDTH / 2 + HUD_LOCAL_COORDINATE_OFFSET;
	float base_y = this->y - HUD_BBOX_HEIGHT / 2 + HUD_LOCAL_COORDINATE_OFFSET;
	livesText->SetPosition(base_x + HUD_LIVES_TEXT_OFFSET_X, base_y + HUD_LIVES_TEXT_OFFSET_Y);
	worldText->SetPosition(base_x + HUD_WORLD_TEXT_OFFSET_X, base_y + HUD_WORLD_TEXT_OFFSET_Y);
	coinText->SetPosition(base_x + HUD_COIN_TEXT_OFFSET_X, base_y + HUD_COIN_TEXT_OFFSET_Y);
	pointText->SetPosition(base_x + HUD_POINT_TEXT_OFFSET_X, base_y + HUD_POINT_TEXT_OFFSET_Y);
	timeText->SetPosition(base_x + HUD_TIME_TEXT_OFFSET_X, base_y + HUD_TIME_TEXT_OFFSET_Y);
	for (int i = 0; i < HUD_AMOUNT_OF_CARD; i++) {
		cards[i]->SetPosition(base_x + HUD_CARDS_OFFSET_X + CARD_BBOX_WIDTH * i, base_y + HUD_CARDS_OFFSET_Y);
	}
	powerMeter->SetPosition(base_x + HUD_POWER_METER_OFFSET_X, base_y + HUD_POWER_METER_OFFSET_Y);
	background->SetPosition(base_x - HUD_BACKGROUND_OFFSET_X, base_y);
}
