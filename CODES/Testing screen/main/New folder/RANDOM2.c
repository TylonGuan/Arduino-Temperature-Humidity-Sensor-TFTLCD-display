
/*******************************************************************************
* image
* filename: unsaved
* name: RANDOM
*
* preset name: Color R5G6B5
* data block size: 16 bit(s), uint16_t
* RLE compression enabled: no
* conversion type: Color, not_used not_used
* bits per pixel: 16
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
*******************************************************************************/

/*
 typedef struct {
     const uint16_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>



static const uint16_t image_data_RANDOM[100] = {
    0x0000, 0xffff, 0xffff, 0xffff, 0x500f, 0x500f, 0xffff, 0xffff, 0xffff, 0xffff, 
    0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0x500f, 0x500f, 0x500f, 0xffff, 0xffff, 
    0xffff, 0xffff, 0x500f, 0x500f, 0x500f, 0x500f, 0xffff, 0x500f, 0x500f, 0xffff, 
    0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x500f, 0x500f, 0x500f, 0x500f, 0x500f, 
    0xffff, 0xffff, 0xffff, 0xffff, 0x500f, 0x500f, 0x0000, 0xffff, 0xffff, 0x500f, 
    0xffff, 0x500f, 0x500f, 0x500f, 0x500f, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 
    0x500f, 0x500f, 0xffff, 0x500f, 0x500f, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 
    0x500f, 0xffff, 0x500f, 0xffff, 0x500f, 0x500f, 0x500f, 0xffff, 0x0000, 0xffff, 
    0x500f, 0x500f, 0x500f, 0xffff, 0xffff, 0xffff, 0x500f, 0x500f, 0x0000, 0xffff, 
    0xffff, 0xffff, 0x500f, 0x500f, 0x500f, 0x500f, 0x500f, 0xffff, 0xffff, 0x0000
};
const tImage RANDOM = { image_data_RANDOM, 10, 10,
    16 };
