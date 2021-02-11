


uint16_t vg_get_hres();

uint16_t vg_get_vres();

int (vbe_get_mode_info_remade)(uint16_t mode, vbe_mode_info_t *vmi_p);

void *vggg_init(unsigned short mode);

int vggg_exit();

int square_draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int vg_draw_hlineee(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int vg_paint_pixelll(uint16_t x_coord, uint16_t y_coord, uint32_t color);

int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

void(vg_draw_xpm)(uint8_t *pixmap, xpm_image_t img, uint16_t x, uint16_t y);
