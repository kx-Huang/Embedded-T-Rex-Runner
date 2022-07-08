#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>

// Colors for game
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F
#define YELLOW 0xFF00
#define BLACK 0x0000
#define PINK 0xF81A
#define BLUE_LIGHT 0x073F

/* Bitmaps */
extern const uint8_t dinoBitmaps[];
extern const uint8_t cactusBitmaps[];

/* Bitmap sizes for dinosaur */
extern const uint8_t dinoWidthPixels;
extern const uint8_t dinoHeightPixels;

/* Bitmap sizes for cactus */
extern const uint8_t cactusWidthPixels;
extern const uint8_t cactusHeightPixels;

#endif /* IMAGE_H_ */
