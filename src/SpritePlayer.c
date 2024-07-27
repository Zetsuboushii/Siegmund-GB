#include "Banks/SetAutoBank.h"
#include "main.h"
#include "gb/gb.h"
#include "ZGBMain.h"
#include "Sprite.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "asm/sm83/types.h"

const UINT8 anim_walk[] = {2, 2, 1};
const UINT8 anim_idle[] = {2, 0, 1};
const UINT8 anim_duck[] = {1, 1};
const UINT8 anim_atck[] = {4, 3, 3, 3, 3};
const UINT8 anim_slsh[] = {4, 4, 5, 6, 7};

typedef enum {
    PLAYER_STATE_NORMAL,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_RUNNING,
    PLAYER_STATE_ATTACKING,
    PLAYER_STATE_DASHING
} PLAYER_STATE;

PLAYER_STATE player_state;
INT16 player_accel_y;
INT8 player_accel_x;

Sprite *slashing_sprite;

extern UINT16 reset_x;
extern UINT16 reset_y;
extern UINT8 level;

void START() {
    SetSpriteAnim(THIS, anim_idle, 3u);

    player_accel_y = 0;
    player_accel_x = 1;

    scroll_target = THIS;

    player_state = PLAYER_STATE_NORMAL;

    slashing_sprite = 0;
}

UINT8 tile_collision;

void CheckCollisionTile(Sprite *sprite, UINT8 idx) {
    if (tile_collision == 0u) {
        if (level == 1) {
        } else {
            level++;
            reset_x = 32;
            reset_y = 112;
            SetState(StateGame);
        }
    }
}

// TODO: Implement Dash Ability

void MovePlayer(Sprite *sprite, UINT8 idx) {
    if (KEY_PRESSED(J_RIGHT)) {
        tile_collision = TranslateSprite(sprite, (1 << delta_time) * player_accel_x, 0);
        THIS->mirror = NO_MIRROR;
        CheckCollisionTile(sprite, idx);
    } else if (KEY_PRESSED(J_LEFT)) {
        tile_collision = TranslateSprite(sprite, (-1 << delta_time) * player_accel_x, 0);
        THIS->mirror = V_MIRROR;
        CheckCollisionTile(sprite, idx);
    }
}

void UpdateAttackPosition() {
    SetSpriteAnim(slashing_sprite, anim_slsh, 15u);
    slashing_sprite->mirror = THIS->mirror;
    if (THIS->mirror == V_MIRROR)
        slashing_sprite->x = THIS->x - 16u;
    else
        slashing_sprite->x = THIS->x + 16u;
    slashing_sprite->y = THIS->y;
}

void UPDATE() {
    UINT8 i;
    Sprite *sprite;

    switch (player_state) {
        case PLAYER_STATE_NORMAL:
            MovePlayer(THIS, THIS_IDX);

            if (KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT)) {
                SetSpriteAnim(THIS, anim_walk, 10u * player_accel_x);
            } else {
                SetSpriteAnim(THIS, anim_idle, 3u);
            }

            if (KEY_TICKED(J_A)) {
                player_accel_y = -50;
                player_state = PLAYER_STATE_JUMPING;
            }

            if (KEY_PRESSED(J_DOWN)) {
                SetSpriteAnim(THIS, anim_duck, 15u);
            }

            if (KEY_PRESSED(J_B)) {
                player_accel_x = 2;
                player_state = PLAYER_STATE_RUNNING;
            }

            if ((player_accel_y >> 4) > 1) {
                player_state = PLAYER_STATE_JUMPING;
            }
            break;

        case PLAYER_STATE_JUMPING:
            MovePlayer(THIS, THIS_IDX);

            if (KEY_RELEASED(J_B)) {
                player_accel_x = 1;
                player_state = PLAYER_STATE_JUMPING;
            }
            break;

        case PLAYER_STATE_RUNNING:
            MovePlayer(THIS, THIS_IDX);

            if (KEY_RELEASED(J_B)) {
                player_accel_x = 1;
                player_state = PLAYER_STATE_NORMAL;
            }

            if (KEY_TICKED(J_A)) {
                player_accel_y = -50;
                player_state = PLAYER_STATE_JUMPING;
            }

            if ((player_accel_y >> 4) > 1) {
                player_state = PLAYER_STATE_JUMPING;
            }
            break;

        case PLAYER_STATE_ATTACKING:
            if (slashing_sprite->anim_frame == 3) {
                player_state = PLAYER_STATE_NORMAL;
                SpriteManagerRemoveSprite(slashing_sprite);
            } else {
                MovePlayer(THIS, THIS_IDX);
                UpdateAttackPosition();
            }
            break;
    }

    if (player_accel_y < 40) {
        player_accel_y += 2;
    }
    tile_collision = TranslateSprite(THIS, 0, player_accel_y >> 4);
    if (!tile_collision && delta_time != 0 && player_accel_y < 40) {
        player_accel_y = 0;
        if (player_state == PLAYER_STATE_JUMPING) {
            player_state = PLAYER_STATE_NORMAL;
        }

        CheckCollisionTile(THIS, THIS_IDX);
    }
    if (tile_collision) {
        player_accel_y = 0;
        if (player_state == PLAYER_STATE_JUMPING) {
            player_state = PLAYER_STATE_NORMAL;
        }

        CheckCollisionTile(THIS, THIS_IDX);
    }

    if (KEY_TICKED(J_B) && player_state != PLAYER_STATE_ATTACKING) {
        SetSpriteAnim(THIS, anim_atck, 3u);
        player_state = PLAYER_STATE_ATTACKING;

        slashing_sprite = SpriteManagerAdd(SpriteAttack, THIS->x, THIS->y);
        UpdateAttackPosition();
    }
}

void DESTROY() {
}
