#include <lcom/lcf.h>
#include <stdint.h>

#include "mole.h"

static char kbd_keys [6] = {KBD_KEY_0, KBD_KEY_1, KBD_KEY_2, KBD_KEY_3, KBD_KEY_4, KBD_KEY_5};

Mole *createMole(int mole_index)
{
    //Allocating memory
    Mole *mole = (Mole *)malloc(sizeof(Mole));

    //Setting up mole variables
    mole->position = HIDED;
    mole->time_up = 0;
    mole->kbd_key = kbd_keys[mole_index-1];
    mole->x = STEP_FROM_X + (mole_index - 1 - 3 * (mole_index >> 2)) * MOLE_WIDHT;
    mole->y = STEP_FROM_Y + (mole_index >> 2) * MOLE_HEIGHT;

    //Loading xpm images for a mole
    xpm_image_t img;

    //hole widhtout a mole
    xpm_load(hole_xpm, XPM_8_8_8_8, &img);
    mole->hided = img;

    //mole up
    xpm_load(mole_up1_xpm, XPM_8_8_8_8, &img);
    mole->up_or_down_missed[0] = img;
    xpm_load(mole_up2_xpm, XPM_8_8_8_8, &img);
    mole->up_or_down_missed[1] = img;
    xpm_load(mole_up3_xpm, XPM_8_8_8_8, &img);
    mole->up_or_down_missed[2] = img;
    xpm_load(mole_up4_xpm, XPM_8_8_8_8, &img);
    mole->up_or_down_missed[3] = img;

    /*     //mole down miss
    xpm_load(mole_down_miss1_xpm, XPM_8_8_8_8, &img);
    mole->down_miss[0] = img;
    xpm_load(mole_down_miss2_xpm, XPM_8_8_8_8, &img);
    mole->down_miss[1] = img;
    xpm_load(mole_down_miss3_xpm, XPM_8_8_8_8, &img);
    mole->down_miss[2] = img;
    xpm_load(mole_down_miss4_xpm, XPM_8_8_8_8, &img);
    mole->down_miss[3] = img; */

    //mole down hit
    xpm_load(mole_down_hit1_xpm, XPM_8_8_8_8, &img);
    mole->down_hit[0] = img;
    xpm_load(mole_down_hit2_xpm, XPM_8_8_8_8, &img);
    mole->down_hit[1] = img;
    xpm_load(mole_down_hit3_xpm, XPM_8_8_8_8, &img);
    mole->down_hit[2] = img;
    xpm_load(mole_down_hit4_xpm, XPM_8_8_8_8, &img);
    mole->down_hit[3] = img;

    return mole;
}

void draw_mole(Mole *mole)
{
    //update mole position
    switch (mole->position)
    {
    case HIDED:
        mole->atual_img = mole->hided;
        break;
    case UP_1:
        mole->atual_img = mole->up_or_down_missed[0];
        break;
    case DOWN_MISSED_1:
        mole->atual_img = mole->up_or_down_missed[0];
        break;
    case UP_2:
        mole->atual_img = mole->up_or_down_missed[1];
        break;
    case DOWN_MISSED_2:
        mole->atual_img = mole->up_or_down_missed[1];
        break;
    case UP_3:
        mole->atual_img = mole->up_or_down_missed[2];
        break;
    case DOWN_MISSED_3:
        mole->atual_img = mole->up_or_down_missed[2];
        break;
    case UP_4:
        mole->atual_img = mole->up_or_down_missed[3];
        break;
    case DOWN_MISSED_4:
        mole->atual_img = mole->up_or_down_missed[3];
        break;
    case DOWN_HIT_1:
        mole->atual_img = mole->down_hit[0];
        break;
    case DOWN_HIT_2:
        mole->atual_img = mole->down_hit[1];
        break;
    case DOWN_HIT_3:
        mole->atual_img = mole->down_hit[2];
        break;
    case DOWN_HIT_4:
        mole->atual_img = mole->down_hit[3];
        break;
    default:
        break;
    }

    uint32_t *pixmap = (uint32_t *)mole->atual_img.bytes;
    // copy it to graphics memory
    vg_draw_xpm(pixmap, mole->atual_img, mole->x, mole->y);
}
