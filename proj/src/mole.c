#include <lcom/lcf.h>
#include <stdint.h>

#include "mole.h"
#include "xpm_coordinates.h"

static char kbd_keys [6] = {KBD_KEY_0, KBD_KEY_1, KBD_KEY_2, KBD_KEY_3, KBD_KEY_4, KBD_KEY_5};

Mole *createMole(int index)
{
    //Allocating memory
    Mole *mole = (Mole *)malloc(sizeof(Mole));

    //Setting up mole variables
    mole->position = HIDED;
    mole->time_up = 0;
    mole->kbd_key = kbd_keys[index];
    
    if(index<3) {
        mole->x = FIRST_MOLE_X + index * (MOLE_WIDTH + MOLE_STEP_COL);
        mole->y = FIRST_MOLE_Y;
    }
    else {
        index -= 3;
        mole->x = FIRST_MOLE_X + index * (MOLE_WIDTH + MOLE_STEP_COL);
        mole->y = FIRST_MOLE_Y + MOLE_HEIGHT + MOLE_STEP_LINE;
    }
    

    //Loading xpm images for a mole. NOTE: this assumes the sprites are being loaded in the same order as the enum
    //hole
    xpm_load(hole_xpm, XPM_8_8_8_8, &(mole->sprites[0]));
    //mole up
    xpm_load(mole_up1_xpm, XPM_8_8_8_8, &(mole->sprites[1]));
    xpm_load(mole_up2_xpm, XPM_8_8_8_8, &(mole->sprites[2]));
    xpm_load(mole_up3_xpm, XPM_8_8_8_8, &(mole->sprites[3]));
    xpm_load(mole_up4_xpm, XPM_8_8_8_8, &(mole->sprites[4]));
    //mole down miss
    xpm_load(mole_down_miss4_xpm, XPM_8_8_8_8, &(mole->sprites[5]));
    xpm_load(mole_down_miss3_xpm, XPM_8_8_8_8, &(mole->sprites[6]));
    xpm_load(mole_down_miss2_xpm, XPM_8_8_8_8, &(mole->sprites[7]));
    xpm_load(mole_down_miss1_xpm, XPM_8_8_8_8, &(mole->sprites[8]));
    //mole down hit
    xpm_load(mole_down_hit4_xpm, XPM_8_8_8_8, &(mole->sprites[9]));
    xpm_load(mole_down_hit3_xpm, XPM_8_8_8_8, &(mole->sprites[10]));
    xpm_load(mole_down_hit2_xpm, XPM_8_8_8_8, &(mole->sprites[11]));
    xpm_load(mole_down_hit1_xpm, XPM_8_8_8_8, &(mole->sprites[12]));

    return mole;
}

void draw_mole(Mole *mole)
{
    xpm_image_t current_img = mole->sprites[(int) mole->position];
    vg_draw_xpm((uint32_t *) current_img.bytes, current_img, mole->x, mole->y);
}

void draw_all_moles(Mole* moles, int num_moles)
{
    for (int i = 0; i < num_moles; i++)
    {
        draw_mole(&moles[i]);
    }
}


bool check_over_mole(Mole *mole, int cursor_x, int cursor_y)
{
    int xi, xf, yi, yf;
    xi = mole->x + 42;
    yi = mole->y;
    xf = xi + 90;
    yf = yi + 60;
    if (xi <= cursor_x && cursor_x <= xf && yi <= cursor_y && cursor_y <= yf)
    {
        return true;
    }
    return false;
}


void reset_moles(Mole* moles, int num_moles) {
    for (int i = 0; i < num_moles; i++) {
        moles[i].position = HIDED;
        moles[i].time_up = 0;
    }
}

