#include "Banks/SetAutoBank.h"
#include "main.h"
#include "gb/gb.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "asm/sm83/types.h"

const UINT8 anim_skeleton_walk[] = {2, 0, 0};

typedef enum {
    SKELETON_STATE_NORMAL,
    SKELETON_STATE_FALLING
} SKELETON_STATE;

SKELETON_STATE skeleton_state;
INT16 skeleton_accel_y;

extern UINT16 reset_x;
extern UINT16 reset_y;
extern UINT8 level;

void START() {
    SetSpriteAnim(THIS, anim_skeleton_walk, 3u);

    skeleton_accel_y = 0;

    skeleton_state = SKELETON_STATE_NORMAL;

    THIS->mirror = V_MIRROR;
}

static UINT8 tile_collision;

void CheckSkeletonCollisionTile(Sprite *sprite, UINT8 idx) {
    if (tile_collision == 0u) {
        if (level == 1) {
        } else {
            level++;
        }
    }
}

void MoveSkeleton(Sprite *sprite, UINT8 idx) {
    UINT8 collision = 0;
    if (THIS->mirror == NO_MIRROR) {
        collision = TranslateSprite(sprite, (1 << delta_time), 0);
        if (collision) {
            THIS->mirror = V_MIRROR;
            CheckSkeletonCollisionTile(sprite, idx);
        }
    } else if (THIS->mirror == V_MIRROR) {
        collision = TranslateSprite(sprite, (-1 << delta_time), 0);
        if (collision) {
            THIS->mirror = NO_MIRROR;
            CheckSkeletonCollisionTile(sprite, idx);
        }
    }
    tile_collision = collision;
}

void UPDATE() {
    UINT8 i;
    Sprite *sprite;

    switch (skeleton_state) {
        case SKELETON_STATE_NORMAL:
            MoveSkeleton(THIS, THIS_IDX);

            SetSpriteAnim(THIS, anim_skeleton_walk, 10u);

            if ((skeleton_accel_y >> 4) > 1) {
                skeleton_state = SKELETON_STATE_FALLING;
            }
            break;

        case SKELETON_STATE_FALLING:
            MoveSkeleton(THIS, THIS_IDX);
            break;
    }

    if (skeleton_accel_y < 40) {
        skeleton_accel_y += 2;
    }
    tile_collision = TranslateSprite(THIS, 0, skeleton_accel_y >> 4);
    if (!tile_collision && delta_time != 0 && skeleton_accel_y < 40) {
        skeleton_accel_y = 0;
        if (skeleton_state == SKELETON_STATE_FALLING) {
            skeleton_state = SKELETON_STATE_NORMAL;
        }

        CheckSkeletonCollisionTile(THIS, THIS_IDX);
    }
    if (tile_collision) {
        skeleton_accel_y = 0;
        if (skeleton_state == SKELETON_STATE_FALLING) {
            skeleton_state = SKELETON_STATE_NORMAL;
        }

        CheckSkeletonCollisionTile(THIS, THIS_IDX);
    }
}

void DESTROY() {
}
