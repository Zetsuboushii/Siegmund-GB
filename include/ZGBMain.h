#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateGame)\
STATE_DEF_END

#define SPRITES \
_SPRITE_COLOR(SpritePlayer, player)\
_SPRITE_COLOR(SpriteAttack, player)\
_SPRITE_COLOR(SpriteSkeleton, skeleton)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif