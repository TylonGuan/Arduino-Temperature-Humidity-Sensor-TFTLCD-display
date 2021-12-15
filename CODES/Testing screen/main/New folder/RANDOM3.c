
/*******************************************************************************
* image
* filename: unsaved
* name: RANDOM
*
* preset name: Color R5G6B5
* data block size: 32 bit(s), uint32_t
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
     const uint32_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>



static const uint32_t image_data_RANDOM[50] = {
    0x0000ffff, 0xffffffff, 0x500f500f, 0xffffffff, 0xffffffff, 
    0x0000ffff, 0xffffffff, 0xffff500f, 0x500f500f, 0xffffffff, 
    0xffffffff, 0x500f500f, 0x500f500f, 0xffff500f, 0x500fffff, 
    0xffffffff, 0xffff0000, 0x0000500f, 0x500f500f, 0x500f500f, 
    0xffffffff, 0xffffffff, 0x500f500f, 0x0000ffff, 0xffff500f, 
    0xffff500f, 0x500f500f, 0x500fffff, 0x00000000, 0xffffffff, 
    0x500f500f, 0xffff500f, 0x500fffff, 0xffff0000, 0x0000ffff, 
    0x500fffff, 0x500fffff, 0x500f500f, 0x500fffff, 0x0000ffff, 
    0x500f500f, 0x500fffff, 0xffffffff, 0x500f500f, 0x0000ffff, 
    0xffffffff, 0x500f500f, 0x500f500f, 0x500fffff, 0xffff0000
};
const tImage RANDOM = { image_data_RANDOM, 10, 10,
    32 };

