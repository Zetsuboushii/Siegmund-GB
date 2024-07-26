#include "Banks/SetAutoBank.h"
#include "Scroll.h"
#include "Sprite.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Music.h"
#include "asm/sm83/types.h"


IMPORT_MAP(map);

const UINT8 collision_tiles[] = {1, 2, 3, 4, 5, 6, 7, 0};

UINT16 reset_x;
UINT16 reset_y;
UINT8 level;

void START() {
    Sprite *player_sprite;

    player_sprite = SpriteManagerAdd(SpritePlayer, 50, 50);

    InitScroll(BANK(map), &map, collision_tiles, 0);
}

void UPDATE() {
}
