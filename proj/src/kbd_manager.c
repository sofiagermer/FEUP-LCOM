#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"

char kbd_manager(uint8_t scanCode) {
  char letter;

  switch (scanCode) {
    case A_BREAK:
      letter = 'A';
      break;
    case B_BREAK:
      letter = 'B';
      break;
    case C_BREAK:
      letter = 'C';
      break;
    case D_BREAK:
      letter = 'D';
      break;
    case E_BREAK:
      letter = 'E';
      break;
    case F_BREAK:
      letter = 'F';
      break;
    case G_BREAK:
      letter = 'G';
      break;
    case H_BREAK:
      letter = 'H';
      break;
    case I_BREAK:
      letter = 'I';
      break;
    case J_BREAK:
      letter = 'J';
      break;
    case K_BREAK:
      letter = 'K';
      break;
    case L_BREAK:
      letter = 'L';
      break;
    case M_BREAK:
      letter = 'M';
      break;
    case N_BREAK:
      letter = 'N';
      break;
    case O_BREAK:
      letter = 'O';
      break;
    case P_BREAK:
      letter = 'P';
      break;
    case Q_BREAK:
      letter = 'Q';
      break;
    case R_BREAK:
      letter = 'R';
      break;
    case S_BREAK:
      letter = 'S';
      break;
    case T_BREAK:
      letter = 'T';
      break;
    case U_BREAK:
      letter = 'U';
      break;
    case V_BREAK:
      letter = 'V';
      break;
    case W_BREAK:
      letter = 'W';
      break;
    case X_BREAK:
      letter = 'X';
      break;
    case Y_BREAK:
      letter = 'Y';
      break;
    case Z_BREAK:
      letter = 'Z';
      break;
    default:
      letter = '.';
      break;
  }

  return letter;
}
