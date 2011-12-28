/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DWT_H
#define AVCODEC_DWT_H

#include <stdint.h>

typedef int DWTELEM;
typedef short IDWTELEM;

typedef struct {
    IDWTELEM *b0;
    IDWTELEM *b1;
    IDWTELEM *b2;
    IDWTELEM *b3;
    int y;
} DWTCompose;


typedef struct slice_buffer_s {
    IDWTELEM * * line; 
    IDWTELEM * * data_stack; 
    int data_stack_top;
    int line_count;
    int line_width;
    int data_count;
    IDWTELEM * base_buffer; 
} slice_buffer;

typedef struct DWTContext {
    void (*vertical_compose97i)(IDWTELEM *b0, IDWTELEM *b1, IDWTELEM *b2, IDWTELEM *b3, IDWTELEM *b4, IDWTELEM *b5, int width);
    void (*horizontal_compose97i)(IDWTELEM *b, int width);
    void (*inner_add_yblock)(const uint8_t *obmc, const int obmc_stride, uint8_t * * block, int b_w, int b_h, int src_x, int src_y, int src_stride, slice_buffer * sb, int add, uint8_t * dst8);
} DWTContext;

#define MAX_DECOMPOSITIONS 8

#define DWT_97 0
#define DWT_53 1

#define liftS lift
#if 1
#define W_AM 3
#define W_AO 0
#define W_AS 1

#undef liftS
#define W_BM 1
#define W_BO 8
#define W_BS 4

#define W_CM 1
#define W_CO 0
#define W_CS 0

#define W_DM 3
#define W_DO 4
#define W_DS 3
#elif 0
#define W_AM 55
#define W_AO 16
#define W_AS 5

#define W_BM 3
#define W_BO 32
#define W_BS 6

#define W_CM 127
#define W_CO 64
#define W_CS 7

#define W_DM 7
#define W_DO 8
#define W_DS 4
#elif 0
#define W_AM 97
#define W_AO 32
#define W_AS 6

#define W_BM 63
#define W_BO 512
#define W_BS 10

#define W_CM 13
#define W_CO 8
#define W_CS 4

#define W_DM 15
#define W_DO 16
#define W_DS 5

#else

#define W_AM 203
#define W_AO 64
#define W_AS 7

#define W_BM 217
#define W_BO 2048
#define W_BS 12

#define W_CM 113
#define W_CO 64
#define W_CS 7

#define W_DM 227
#define W_DO 128
#define W_DS 9
#endif

#define slice_buffer_get_line(slice_buf, line_num) ((slice_buf)->line[line_num] ? (slice_buf)->line[line_num] : ff_slice_buffer_load_line((slice_buf), (line_num)))


void ff_slice_buffer_init(slice_buffer * buf, int line_count, int max_allocated_lines, int line_width, IDWTELEM * base_buffer);
void ff_slice_buffer_release(slice_buffer * buf, int line);
void ff_slice_buffer_flush(slice_buffer * buf);
void ff_slice_buffer_destroy(slice_buffer * buf);
IDWTELEM * ff_slice_buffer_load_line(slice_buffer * buf, int line);

void ff_snow_vertical_compose97i(IDWTELEM *b0, IDWTELEM *b1, IDWTELEM *b2, IDWTELEM *b3, IDWTELEM *b4, IDWTELEM *b5, int width);
void ff_snow_horizontal_compose97i(IDWTELEM *b, int width);
void ff_snow_inner_add_yblock(const uint8_t *obmc, const int obmc_stride, uint8_t * * block, int b_w, int b_h, int src_x, int src_y, int src_stride, slice_buffer * sb, int add, uint8_t * dst8);

int ff_w53_32_c(void *v, uint8_t * pix1, uint8_t * pix2, int line_size, int h);
int ff_w97_32_c(void *v, uint8_t * pix1, uint8_t * pix2, int line_size, int h);

void ff_spatial_dwt(int *buffer, int width, int height, int stride, int type, int decomposition_count);

void ff_spatial_idwt_buffered_init(DWTCompose *cs, slice_buffer * sb, int width, int height, int stride_line, int type, int decomposition_count);
void ff_spatial_idwt_buffered_slice(DWTContext *dsp, DWTCompose *cs, slice_buffer * slice_buf, int width, int height, int stride_line, int type, int decomposition_count, int y);
void ff_spatial_idwt_init(DWTCompose *cs, IDWTELEM *buffer, int width, int height, int stride, int type, int decomposition_count);
void ff_spatial_idwt_slice(DWTCompose *cs, IDWTELEM *buffer, int width, int height, int stride, int type, int decomposition_count, int y);
void ff_spatial_idwt(IDWTELEM *buffer, int width, int height, int stride, int type, int decomposition_count);

void ff_dwt_init(DWTContext *c);
void ff_dwt_init_x86(DWTContext *c);

#endif 