
/*******************************************************************************
* image
* filename: unsaved
* name: RANDOM
*
* preset name: Color A8R8G8B8
* data block size: 32 bit(s), uint32_t
* RLE compression enabled: no
* conversion type: Monochrome, Edge 128
* bits per pixel: 24
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



static const uint32_t image_data_RANDOM[80] = {
    0x000000ff, 0xffffffff, 0xffffffff, 0x00000000, 0x0000ffff, 0xffffffff, 0xffffffff, 0xffff0000, 
    0x000000ff, 0xffffffff, 0xffffffff, 0xffffff00, 0x00000000, 0x00000000, 0xffffffff, 0xffff0000, 
    0xffffffff, 0xffff0000, 0x00000000, 0x00000000, 0x0000ffff, 0xff000000, 0x000000ff, 0xffff0000, 
    0xffffffff, 0xffffffff, 0xff000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00ffffff, 0xffffff00, 0x00000000, 
    0xffffff00, 0x00000000, 0x00000000, 0x000000ff, 0xffff0000, 0x00000000, 0xffffffff, 0xffff0000, 
    0x00000000, 0x0000ffff, 0xff000000, 0x000000ff, 0xffffffff, 0xff000000, 0x000000ff, 0xffff0000, 
    0x000000ff, 0xffff0000, 0x00ffffff, 0x00000000, 0x00000000, 0x00ffffff, 0x000000ff, 0xffff0000, 
    0x00000000, 0x00000000, 0x00ffffff, 0xffffffff, 0xffff0000, 0x00000000, 0x000000ff, 0xffff0000, 
    0xffffffff, 0xffff0000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0xffffff00, 0x00000000
};
const tImage RANDOM = { image_data_RANDOM, 10, 10,
    32 };

