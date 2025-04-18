 /* 
 
 SDL2_gfxPrimitives.c: graphics primitives for SDL2 renderers
 
 Copyright (C) 2012  Andreas Schiffler
 
 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source
 distribution.
 
 Andreas Schiffler -- aschiffler at ferzkopp dot net
 
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>
 
 #include "SDL2_gfxPrimitives.h"
 #include "SDL2_rotozoom.h"
 #include "SDL2_gfxPrimitives_font.h"
 
 /* ---- Structures */
 
 typedef struct {
         Sint16 x, y;
         int dx, dy, s1, s2, swapdir, error;
         Uint32 count;
 } SDL2_gfxBresenhamIterator;
 
 typedef struct {
         SDL_Renderer *renderer;
         int u, v;               /* delta x , delta y */
         int ku, kt, kv, kd;     /* loop constants */
         int oct2;
         int quad4;
         Sint16 last1x, last1y, last2x, last2y, first1x, first1y, first2x, first2y, tempx, tempy;
 } SDL2_gfxMurphyIterator;
 
 /* ---- Pixel */
 
 int pixel(SDL_Renderer *renderer, Sint16 x, Sint16 y)
 {
         return SDL_RenderDrawPoint(renderer, x, y);
 }
 
 int pixelColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return pixelRGBA(renderer, x, y, c[0], c[1], c[2], c[3]);
 }
 
 int pixelRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
         result |= SDL_RenderDrawPoint(renderer, x, y);
         return result;
 }
 
 int pixelRGBAWeight(SDL_Renderer * renderer, Sint16 x, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint32 weight)
 {
         /*
         * Modify Alpha by weight 
         */
         Uint32 ax = a;
         ax = ((ax * weight) >> 8);
         if (ax > 255) {
                 a = 255;
         } else {
                 a = (Uint8)(ax & 0x000000ff);
         }
 
         return pixelRGBA(renderer, x, y, r, g, b, a);
 }
 
 /* ---- Hline */
 
 int hline(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y)
 {
         return SDL_RenderDrawLine(renderer, x1, y, x2, y);;
 }
 
 
 int hlineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return hlineRGBA(renderer, x1, x2, y, c[0], c[1], c[2], c[3]);
 }
 
 int hlineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 x2, Sint16 y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
         result |= SDL_RenderDrawLine(renderer, x1, y, x2, y);
         return result;
 }
 
 /* ---- Vline */
 
 int vlineColor(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return vlineRGBA(renderer, x, y1, y2, c[0], c[1], c[2], c[3]);
 }
 
 int vlineRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y1, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
         result |= SDL_RenderDrawLine(renderer, x, y1, x, y2);
         return result;
 }
 
 /* ---- Rectangle */
 
 int rectangleColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return rectangleRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3]);
 }
 
 int rectangleRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         Sint16 tmp;
         SDL_Rect rect;
 
         /*
         * Test for special cases of straight lines or single point 
         */
         if (x1 == x2) {
                 if (y1 == y2) {
                         return (pixelRGBA(renderer, x1, y1, r, g, b, a));
                 } else {
                         return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
                 }
         } else {
                 if (y1 == y2) {
                         return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
                 }
         }
 
         /*
         * Swap x1, x2 if required 
         */
         if (x1 > x2) {
                 tmp = x1;
                 x1 = x2;
                 x2 = tmp;
         }
 
         /*
         * Swap y1, y2 if required 
         */
         if (y1 > y2) {
                 tmp = y1;
                 y1 = y2;
                 y2 = tmp;
         }
 
         /* 
         * Create destination rect
         */      
         rect.x = x1;
         rect.y = y1;
         rect.w = x2 - x1;
         rect.h = y2 - y1;
         
         /*
         * Draw
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a); 
         result |= SDL_RenderDrawRect(renderer, &rect);
         return result;
 }
 
 /* ---- Rounded Rectangle */
 
 int roundedRectangleColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return roundedRectangleRGBA(renderer, x1, y1, x2, y2, rad, c[0], c[1], c[2], c[3]);
 }
 
 int roundedRectangleRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result = 0;
         Sint16 tmp;
         Sint16 w, h;
         Sint16 xx1, xx2;
         Sint16 yy1, yy2;
         
         /*
         * Check renderer
         */
         if (renderer == NULL)
         {
                 return -1;
         }
 
         /*
         * Check radius vor valid range
         */
         if (rad < 0) {
                 return -1;
         }
 
         /*
         * Special case - no rounding
         */
         if (rad <= 1) {
                 return rectangleRGBA(renderer, x1, y1, x2, y2, r, g, b, a);
         }
 
         /*
         * Test for special cases of straight lines or single point 
         */
         if (x1 == x2) {
                 if (y1 == y2) {
                         return (pixelRGBA(renderer, x1, y1, r, g, b, a));
                 } else {
                         return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
                 }
         } else {
                 if (y1 == y2) {
                         return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
                 }
         }
 
         /*
         * Swap x1, x2 if required 
         */
         if (x1 > x2) {
                 tmp = x1;
                 x1 = x2;
                 x2 = tmp;
         }
 
         /*
         * Swap y1, y2 if required 
         */
         if (y1 > y2) {
                 tmp = y1;
                 y1 = y2;
                 y2 = tmp;
         }
 
         /*
         * Calculate width&height 
         */
         w = x2 - x1;
         h = y2 - y1;
 
         /*
         * Maybe adjust radius
         */
         if ((rad * 2) > w)  
         {
                 rad = w / 2;
         }
         if ((rad * 2) > h)
         {
                 rad = h / 2;
         }
 
         /*
         * Draw corners
         */
         xx1 = x1 + rad;
         xx2 = x2 - rad;
         yy1 = y1 + rad;
         yy2 = y2 - rad;
         result |= arcRGBA(renderer, xx1, yy1, rad, 180, 270, r, g, b, a);
         result |= arcRGBA(renderer, xx2, yy1, rad, 270, 360, r, g, b, a);
         result |= arcRGBA(renderer, xx1, yy2, rad,  90, 180, r, g, b, a);
         result |= arcRGBA(renderer, xx2, yy2, rad,   0,  90, r, g, b, a);
 
         /*
         * Draw lines
         */
         if (xx1 <= xx2) {
                 result |= hlineRGBA(renderer, xx1, xx2, y1, r, g, b, a);
                 result |= hlineRGBA(renderer, xx1, xx2, y2, r, g, b, a);
         }
         if (yy1 <= yy2) {
                 result |= vlineRGBA(renderer, x1, yy1, yy2, r, g, b, a);
                 result |= vlineRGBA(renderer, x2, yy1, yy2, r, g, b, a);
         }
 
         return result;
 }
 
 /* ---- Rounded Box */
 
 int roundedBoxColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return roundedBoxRGBA(renderer, x1, y1, x2, y2, rad, c[0], c[1], c[2], c[3]);
 }
 
 int roundedBoxRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2,
         Sint16 y2, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         Sint16 w, h, r2, tmp;
         Sint16 cx = 0;
         Sint16 cy = rad;
         Sint16 ocx = (Sint16) 0xffff;
         Sint16 ocy = (Sint16) 0xffff;
         Sint16 df = 1 - rad;
         Sint16 d_e = 3;
         Sint16 d_se = -2 * rad + 5;
         Sint16 xpcx, xmcx, xpcy, xmcy;
         Sint16 ypcy, ymcy, ypcx, ymcx;
         Sint16 x, y, dx, dy;
 
         /* 
         * Check destination renderer 
         */
         if (renderer == NULL)
         {
                 return -1;
         }
 
         /*
         * Check radius vor valid range
         */
         if (rad < 0) {
                 return -1;
         }
 
         /*
         * Special case - no rounding
         */
         if (rad <= 1) {
                 return rectangleRGBA(renderer, x1, y1, x2, y2, r, g, b, a);
         }
 
         /*
         * Test for special cases of straight lines or single point 
         */
         if (x1 == x2) {
                 if (y1 == y2) {
                         return (pixelRGBA(renderer, x1, y1, r, g, b, a));
                 } else {
                         return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
                 }
         } else {
                 if (y1 == y2) {
                         return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
                 }
         }
 
         /*
         * Swap x1, x2 if required 
         */
         if (x1 > x2) {
                 tmp = x1;
                 x1 = x2;
                 x2 = tmp;
         }
 
         /*
         * Swap y1, y2 if required 
         */
         if (y1 > y2) {
                 tmp = y1;
                 y1 = y2;
                 y2 = tmp;
         }
 
         /*
         * Calculate width&height 
         */
         w = x2 - x1 + 1;
         h = y2 - y1 + 1;
 
         /*
         * Maybe adjust radius
         */
         r2 = rad + rad;
         if (r2 > w)  
         {
                 rad = w / 2;
                 r2 = rad + rad;
         }
         if (r2 > h)
         {
                 rad = h / 2;
         }
 
         /* Setup filled circle drawing for corners */
         x = x1 + rad;
         y = y1 + rad;
         dx = x2 - x1 - rad - rad;
         dy = y2 - y1 - rad - rad;
 
         /*
         * Set color
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /*
         * Draw corners
         */
         do {
                 xpcx = x + cx;
                 xmcx = x - cx;
                 xpcy = x + cy;
                 xmcy = x - cy;
                 if (ocy != cy) {
                         if (cy > 0) {
                                 ypcy = y + cy;
                                 ymcy = y - cy;
                                 result |= hline(renderer, xmcx, xpcx + dx, ypcy + dy);
                                 result |= hline(renderer, xmcx, xpcx + dx, ymcy);
                         } else {
                                 result |= hline(renderer, xmcx, xpcx + dx, y);
                         }
                         ocy = cy;
                 }
                 if (ocx != cx) {
                         if (cx != cy) {
                                 if (cx > 0) {
                                         ypcx = y + cx;
                                         ymcx = y - cx;
                                         result |= hline(renderer, xmcy, xpcy + dx, ymcx);
                                         result |= hline(renderer, xmcy, xpcy + dx, ypcx + dy);
                                 } else {
                                         result |= hline(renderer, xmcy, xpcy + dx, y);
                                 }
                         }
                         ocx = cx;
                 }
 
                 /*
                 * Update 
                 */
                 if (df < 0) {
                         df += d_e;
                         d_e += 2;
                         d_se += 2;
                 } else {
                         df += d_se;
                         d_e += 2;
                         d_se += 4;
                         cy--;
                 }
                 cx++;
         } while (cx <= cy);
 
         /* Inside */
         if (dx > 0 && dy > 0) {
                 result |= boxRGBA(renderer, x1, y1 + rad + 1, x2, y2 - rad, r, g, b, a);
         }
 
         return (result);
 }
 
 /* ---- Box */
 
 int boxColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return boxRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3]);
 }
 
 int boxRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         Sint16 tmp;
         SDL_Rect rect;
 
         /*
         * Test for special cases of straight lines or single point 
         */
         if (x1 == x2) {
                 if (y1 == y2) {
                         return (pixelRGBA(renderer, x1, y1, r, g, b, a));
                 } else {
                         return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
                 }
         } else {
                 if (y1 == y2) {
                         return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
                 }
         }
 
         /*
         * Swap x1, x2 if required 
         */
         if (x1 > x2) {
                 tmp = x1;
                 x1 = x2;
                 x2 = tmp;
         }
 
         /*
         * Swap y1, y2 if required 
         */
         if (y1 > y2) {
                 tmp = y1;
                 y1 = y2;
                 y2 = tmp;
         }
 
         /* 
         * Create destination rect
         */      
         rect.x = x1;
         rect.y = y1;
         rect.w = x2 - x1 + 1;
         rect.h = y2 - y1 + 1;
         
         /*
         * Draw
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a); 
         result |= SDL_RenderFillRect(renderer, &rect);
         return result;
 }
 
 /* ----- Line */
 
 int line(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2)
 {
         /*
         * Draw
         */
         return SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
 }
 
 int lineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return lineRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3]);
 }
 
 int lineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         /*
         * Draw
         */
         int result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a); 
         result |= SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
         return result;
 }
 
 /* ---- AA Line */
 
 #define AAlevels 256
 #define AAbits 8
 
 int _aalineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int draw_endpoint)
 {
         Sint32 xx0, yy0, xx1, yy1;
         int result;
         Uint32 intshift, erracc, erradj;
         Uint32 erracctmp, wgt, wgtcompmask;
         int dx, dy, tmp, xdir, y0p1, x0pxdir;
 
         /*
         * Keep on working with 32bit numbers 
         */
         xx0 = x1;
         yy0 = y1;
         xx1 = x2;
         yy1 = y2;
 
         /*
         * Reorder points to make dy positive 
         */
         if (yy0 > yy1) {
                 tmp = yy0;
                 yy0 = yy1;
                 yy1 = tmp;
                 tmp = xx0;
                 xx0 = xx1;
                 xx1 = tmp;
         }
 
         /*
         * Calculate distance 
         */
         dx = xx1 - xx0;
         dy = yy1 - yy0;
 
         /*
         * Adjust for negative dx and set xdir 
         */
         if (dx >= 0) {
                 xdir = 1;
         } else {
                 xdir = -1;
                 dx = (-dx);
         }
         
         /*
         * Check for special cases 
         */
         if (dx == 0) {
                 /*
                 * Vertical line 
                 */
                 if (draw_endpoint)
                 {
                         return (vlineRGBA(renderer, x1, y1, y2, r, g, b, a));
                 } else {
                         if (dy > 0) {
                                 return (vlineRGBA(renderer, x1, yy0, yy0+dy, r, g, b, a));
                         } else {
                                 return (pixelRGBA(renderer, x1, y1, r, g, b, a));
                         }
                 }
         } else if (dy == 0) {
                 /*
                 * Horizontal line 
                 */
                 if (draw_endpoint)
                 {
                         return (hlineRGBA(renderer, x1, x2, y1, r, g, b, a));
                 } else {
                         if (dx > 0) {
                                 return (hlineRGBA(renderer, xx0, xx0+dx, y1, r, g, b, a));
                         } else {
                                 return (pixelRGBA(renderer, x1, y1, r, g, b, a));
                         }
                 }
         } else if ((dx == dy) && (draw_endpoint)) {
                 /*
                 * Diagonal line (with endpoint)
                 */
                 return (lineRGBA(renderer, x1, y1, x2, y2,  r, g, b, a));
         }
 
 
         /*
         * Line is not horizontal, vertical or diagonal (with endpoint)
         */
         result = 0;
 
         /*
         * Zero accumulator 
         */
         erracc = 0;
 
         /*
         * # of bits by which to shift erracc to get intensity level 
         */
         intshift = 32 - AAbits;
 
         /*
         * Mask used to flip all bits in an intensity weighting 
         */
         wgtcompmask = AAlevels - 1;
 
         /*
         * Draw the initial pixel in the foreground color 
         */
         result |= pixelRGBA(renderer, x1, y1, r, g, b, a);
 
         /*
         * x-major or y-major? 
         */
         if (dy > dx) {
 
                 /*
                 * y-major.  Calculate 16-bit fixed point fractional part of a pixel that
                 * X advances every time Y advances 1 pixel, truncating the result so that
                 * we won't overrun the endpoint along the X axis 
                 */
                 /*
                 * Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy; 
                 */
                 erradj = ((dx << 16) / dy) << 16;
 
                 /*
                 * draw all pixels other than the first and last 
                 */
                 x0pxdir = xx0 + xdir;
                 while (--dy) {
                         erracctmp = erracc;
                         erracc += erradj;
                         if (erracc <= erracctmp) {
                                 /*
                                 * rollover in error accumulator, x coord advances 
                                 */
                                 xx0 = x0pxdir;
                                 x0pxdir += xdir;
                         }
                         yy0++;          /* y-major so always advance Y */
 
                         /*
                         * the AAbits most significant bits of erracc give us the intensity
                         * weighting for this pixel, and the complement of the weighting for
                         * the paired pixel. 
                         */
                         wgt = (erracc >> intshift) & 255;
                         result |= pixelRGBAWeight (renderer, xx0, yy0, r, g, b, a, 255 - wgt);
                         result |= pixelRGBAWeight (renderer, x0pxdir, yy0, r, g, b, a, wgt);
                 }
 
         } else {
 
                 /*
                 * x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
                 * that Y advances each time X advances 1 pixel, truncating the result so
                 * that we won't overrun the endpoint along the X axis. 
                 */
                 /*
                 * Not-so-portable version: erradj = ((Uint64)dy << 32) / (Uint64)dx; 
                 */
                 erradj = ((dy << 16) / dx) << 16;
 
                 /*
                 * draw all pixels other than the first and last 
                 */
                 y0p1 = yy0 + 1;
                 while (--dx) {
 
                         erracctmp = erracc;
                         erracc += erradj;
                         if (erracc <= erracctmp) {
                                 /*
                                 * Accumulator turned over, advance y 
                                 */
                                 yy0 = y0p1;
                                 y0p1++;
                         }
                         xx0 += xdir;    /* x-major so always advance X */
                         /*
                         * the AAbits most significant bits of erracc give us the intensity
                         * weighting for this pixel, and the complement of the weighting for
                         * the paired pixel. 
                         */
                         wgt = (erracc >> intshift) & 255;
                         result |= pixelRGBAWeight (renderer, xx0, yy0, r, g, b, a, 255 - wgt);
                         result |= pixelRGBAWeight (renderer, xx0, y0p1, r, g, b, a, wgt);
                 }
         }
 
         /*
         * Do we have to draw the endpoint 
         */
         if (draw_endpoint) {
                 /*
                 * Draw final pixel, always exactly intersected by the line and doesn't
                 * need to be weighted. 
                 */
                 result |= pixelRGBA (renderer, x2, y2, r, g, b, a);
         }
 
         return (result);
 }
 
 int aalineColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return _aalineRGBA(renderer, x1, y1, x2, y2, c[0], c[1], c[2], c[3], 1);
 }
 
 int aalineRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         return _aalineRGBA(renderer, x1, y1, x2, y2, r, g, b, a, 1);
 }
 
 /* ----- Circle */
 
 int circleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return ellipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
 }
 
 int circleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         return ellipseRGBA(renderer, x, y, rad, rad, r, g, b, a);
 }
 
 /* ----- Arc */
 
 int arcColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return arcRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3]);
 }
 
 /* TODO: rewrite algorithm; arc endpoints are not always drawn */
 int arcRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         Sint16 cx = 0;
         Sint16 cy = rad;
         Sint16 df = 1 - rad;
         Sint16 d_e = 3;
         Sint16 d_se = -2 * rad + 5;
         Sint16 xpcx, xmcx, xpcy, xmcy;
         Sint16 ypcy, ymcy, ypcx, ymcx;
         Uint8 drawoct;
         int startoct, endoct, oct, stopval_start = 0, stopval_end = 0;
         double dstart, dend, temp = 0.;
 
         /*
         * Sanity check radius 
         */
         if (rad < 0) {
                 return (-1);
         }
 
         /*
         * Special case for rad=0 - draw a point 
         */
         if (rad == 0) {
                 return (pixelRGBA(renderer, x, y, r, g, b, a));
         }
 
         // Octant labelling
         //      
         //  \ 5 | 6 /
         //   \  |  /
         //  4 \ | / 7
         //     \|/
         //------+------ +x
         //     /|\
         //  3 / | \ 0
         //   /  |  \
         //  / 2 | 1 \
         //      +y
 
         // Initially reset bitmask to 0x00000000
         // the set whether or not to keep drawing a given octant.
         // For example: 0x00111100 means we're drawing in octants 2-5
         drawoct = 0; 
 
         /*
         * Fixup angles
         */
         start %= 360;
         end %= 360;
         // 0 <= start & end < 360; note that sometimes start > end - if so, arc goes back through 0.
         while (start < 0) start += 360;
         while (end < 0) end += 360;
         start %= 360;
         end %= 360;
 
         // now, we find which octants we're drawing in.
         startoct = start / 45;
         endoct = end / 45;
         oct = startoct - 1; // we increment as first step in loop
 
         // stopval_start, stopval_end; 
         // what values of cx to stop at.
         do {
                 oct = (oct + 1) % 8;
 
                 if (oct == startoct) {
                         // need to compute stopval_start for this octant.  Look at picture above if this is unclear
                         dstart = (double)start;
                         switch (oct) 
                         {
                         case 0:
                         case 3:
                                 temp = sin(dstart * M_PI / 180.);
                                 break;
                         case 1:
                         case 6:
                                 temp = cos(dstart * M_PI / 180.);
                                 break;
                         case 2:
                         case 5:
                                 temp = -cos(dstart * M_PI / 180.);
                                 break;
                         case 4:
                         case 7:
                                 temp = -sin(dstart * M_PI / 180.);
                                 break;
                         }
                         temp *= rad;
                         stopval_start = (int)temp; // always round down
 
                         // This isn't arbitrary, but requires graph paper to explain well.
                         // The basic idea is that we're always changing drawoct after we draw, so we
                         // stop immediately after we render the last sensible pixel at x = ((int)temp).
 
                         // and whether to draw in this octant initially
                         if (oct % 2) drawoct |= (1 << oct); // this is basically like saying drawoct[oct] = true, if drawoct were a bool array
                         else             drawoct &= 255 - (1 << oct); // this is basically like saying drawoct[oct] = false
                 }
                 if (oct == endoct) {
                         // need to compute stopval_end for this octant
                         dend = (double)end;
                         switch (oct)
                         {
                         case 0:
                         case 3:
                                 temp = sin(dend * M_PI / 180);
                                 break;
                         case 1:
                         case 6:
                                 temp = cos(dend * M_PI / 180);
                                 break;
                         case 2:
                         case 5:
                                 temp = -cos(dend * M_PI / 180);
                                 break;
                         case 4:
                         case 7:
                                 temp = -sin(dend * M_PI / 180);
                                 break;
                         }
                         temp *= rad;
                         stopval_end = (int)temp;
 
                         // and whether to draw in this octant initially
                         if (startoct == endoct) {
                                 // note:      we start drawing, stop, then start again in this case
                                 // otherwise: we only draw in this octant, so initialize it to false, it will get set back to true
                                 if (start > end) {
                                         // unfortunately, if we're in the same octant and need to draw over the whole circle, 
                                         // we need to set the rest to true, because the while loop will end at the bottom.
                                         drawoct = 255;
                                 } else {
                                         drawoct &= 255 - (1 << oct);
                                 }
                         } 
                         else if (oct % 2) drawoct &= 255 - (1 << oct);
                         else                      drawoct |= (1 << oct);
                 } else if (oct != startoct) { // already verified that it's != endoct
                         drawoct |= (1 << oct); // draw this entire segment
                 }
         } while (oct != endoct);
 
         // so now we have what octants to draw and when to draw them. all that's left is the actual raster code.
 
         /*
         * Set color 
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /*
         * Draw arc 
         */
         do {
                 ypcy = y + cy;
                 ymcy = y - cy;
                 if (cx > 0) {
                         xpcx = x + cx;
                         xmcx = x - cx;
 
                         // always check if we're drawing a certain octant before adding a pixel to that octant.
                         if (drawoct & 4)  result |= pixel(renderer, xmcx, ypcy);
                         if (drawoct & 2)  result |= pixel(renderer, xpcx, ypcy);
                         if (drawoct & 32) result |= pixel(renderer, xmcx, ymcy);
                         if (drawoct & 64) result |= pixel(renderer, xpcx, ymcy);
                 } else {
                         if (drawoct & 96) result |= pixel(renderer, x, ymcy);
                         if (drawoct & 6)  result |= pixel(renderer, x, ypcy);
                 }
 
                 xpcy = x + cy;
                 xmcy = x - cy;
                 if (cx > 0 && cx != cy) {
                         ypcx = y + cx;
                         ymcx = y - cx;
                         if (drawoct & 8)   result |= pixel(renderer, xmcy, ypcx);
                         if (drawoct & 1)   result |= pixel(renderer, xpcy, ypcx);
                         if (drawoct & 16)  result |= pixel(renderer, xmcy, ymcx);
                         if (drawoct & 128) result |= pixel(renderer, xpcy, ymcx);
                 } else if (cx == 0) {
                         if (drawoct & 24)  result |= pixel(renderer, xmcy, y);
                         if (drawoct & 129) result |= pixel(renderer, xpcy, y);
                 }
 
                 /*
                 * Update whether we're drawing an octant
                 */
                 if (stopval_start == cx) {
                         // works like an on-off switch.  
                         // This is just in case start & end are in the same octant.
                         if (drawoct & (1 << startoct)) drawoct &= 255 - (1 << startoct);                
                         else                                               drawoct |= (1 << startoct);
                 }
                 if (stopval_end == cx) {
                         if (drawoct & (1 << endoct)) drawoct &= 255 - (1 << endoct);
                         else                                             drawoct |= (1 << endoct);
                 }
 
                 /*
                 * Update pixels
                 */
                 if (df < 0) {
                         df += d_e;
                         d_e += 2;
                         d_se += 2;
                 } else {
                         df += d_se;
                         d_e += 2;
                         d_se += 4;
                         cy--;
                 }
                 cx++;
         } while (cx <= cy);
 
         return (result);
 }
 
 /* ----- AA Circle */
 
 int aacircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return aaellipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
 }
 
 int aacircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         /*
         * Draw 
         */
         return aaellipseRGBA(renderer, x, y, rad, rad, r, g, b, a);
 }
 
 /* ----- Filled Circle */
 
 int filledCircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return filledEllipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3]);
 }
 
 int filledCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         Sint16 cx = 0;
         Sint16 cy = rad;
         Sint16 ocx = (Sint16) 0xffff;
         Sint16 ocy = (Sint16) 0xffff;
         Sint16 df = 1 - rad;
         Sint16 d_e = 3;
         Sint16 d_se = -2 * rad + 5;
         Sint16 xpcx, xmcx, xpcy, xmcy;
         Sint16 ypcy, ymcy, ypcx, ymcx;
 
         /*
         * Sanity check radius 
         */
         if (rad < 0) {
                 return (-1);
         }
 
         /*
         * Special case for rad=0 - draw a point 
         */
         if (rad == 0) {
                 return (pixelRGBA(renderer, x, y, r, g, b, a));
         }
 
         /*
         * Set color
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /*
         * Draw 
         */
         do {
                 xpcx = x + cx;
                 xmcx = x - cx;
                 xpcy = x + cy;
                 xmcy = x - cy;
                 if (ocy != cy) {
                         if (cy > 0) {
                                 ypcy = y + cy;
                                 ymcy = y - cy;
                                 result |= hline(renderer, xmcx, xpcx, ypcy);
                                 result |= hline(renderer, xmcx, xpcx, ymcy);
                         } else {
                                 result |= hline(renderer, xmcx, xpcx, y);
                         }
                         ocy = cy;
                 }
                 if (ocx != cx) {
                         if (cx != cy) {
                                 if (cx > 0) {
                                         ypcx = y + cx;
                                         ymcx = y - cx;
                                         result |= hline(renderer, xmcy, xpcy, ymcx);
                                         result |= hline(renderer, xmcy, xpcy, ypcx);
                                 } else {
                                         result |= hline(renderer, xmcy, xpcy, y);
                                 }
                         }
                         ocx = cx;
                 }
 
                 /*
                 * Update 
                 */
                 if (df < 0) {
                         df += d_e;
                         d_e += 2;
                         d_se += 2;
                 } else {
                         df += d_se;
                         d_e += 2;
                         d_se += 4;
                         cy--;
                 }
                 cx++;
         } while (cx <= cy);
 
         return (result);
 }
 
 /* ----- Ellipse */
 
 int ellipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return ellipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3]);
 }
 
 int ellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         int ix, iy;
         int h, i, j, k;
         int oh, oi, oj, ok;
         int xmh, xph, ypk, ymk;
         int xmi, xpi, ymj, ypj;
         int xmj, xpj, ymi, ypi;
         int xmk, xpk, ymh, yph;
 
         /*
         * Sanity check radii 
         */
         if ((rx < 0) || (ry < 0)) {
                 return (-1);
         }
 
         /*
         * Special case for rx=0 - draw a vline 
         */
         if (rx == 0) {
                 return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
         }
         /*
         * Special case for ry=0 - draw a hline 
         */
         if (ry == 0) {
                 return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
         }
 
         /*
         * Set color
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /*
         * Init vars 
         */
         oh = oi = oj = ok = 0xFFFF;
 
         /*
         * Draw 
         */
         if (rx > ry) {
                 ix = 0;
                 iy = rx * 64;
 
                 do {
                         h = (ix + 32) >> 6;
                         i = (iy + 32) >> 6;
                         j = (h * ry) / rx;
                         k = (i * ry) / rx;
 
                         if (((ok != k) && (oj != k)) || ((oj != j) && (ok != j)) || (k != j)) {
                                 xph = x + h;
                                 xmh = x - h;
                                 if (k > 0) {
                                         ypk = y + k;
                                         ymk = y - k;
                                         result |= pixel(renderer, xmh, ypk);
                                         result |= pixel(renderer, xph, ypk);
                                         result |= pixel(renderer, xmh, ymk);
                                         result |= pixel(renderer, xph, ymk);
                                 } else {
                                         result |= pixel(renderer, xmh, y);
                                         result |= pixel(renderer, xph, y);
                                 }
                                 ok = k;
                                 xpi = x + i;
                                 xmi = x - i;
                                 if (j > 0) {
                                         ypj = y + j;
                                         ymj = y - j;
                                         result |= pixel(renderer, xmi, ypj);
                                         result |= pixel(renderer, xpi, ypj);
                                         result |= pixel(renderer, xmi, ymj);
                                         result |= pixel(renderer, xpi, ymj);
                                 } else {
                                         result |= pixel(renderer, xmi, y);
                                         result |= pixel(renderer, xpi, y);
                                 }
                                 oj = j;
                         }
 
                         ix = ix + iy / rx;
                         iy = iy - ix / rx;
 
                 } while (i > h);
         } else {
                 ix = 0;
                 iy = ry * 64;
 
                 do {
                         h = (ix + 32) >> 6;
                         i = (iy + 32) >> 6;
                         j = (h * rx) / ry;
                         k = (i * rx) / ry;
 
                         if (((oi != i) && (oh != i)) || ((oh != h) && (oi != h) && (i != h))) {
                                 xmj = x - j;
                                 xpj = x + j;
                                 if (i > 0) {
                                         ypi = y + i;
                                         ymi = y - i;
                                         result |= pixel(renderer, xmj, ypi);
                                         result |= pixel(renderer, xpj, ypi);
                                         result |= pixel(renderer, xmj, ymi);
                                         result |= pixel(renderer, xpj, ymi);
                                 } else {
                                         result |= pixel(renderer, xmj, y);
                                         result |= pixel(renderer, xpj, y);
                                 }
                                 oi = i;
                                 xmk = x - k;
                                 xpk = x + k;
                                 if (h > 0) {
                                         yph = y + h;
                                         ymh = y - h;
                                         result |= pixel(renderer, xmk, yph);
                                         result |= pixel(renderer, xpk, yph);
                                         result |= pixel(renderer, xmk, ymh);
                                         result |= pixel(renderer, xpk, ymh);
                                 } else {
                                         result |= pixel(renderer, xmk, y);
                                         result |= pixel(renderer, xpk, y);
                                 }
                                 oh = h;
                         }
 
                         ix = ix + iy / ry;
                         iy = iy - ix / ry;
 
                 } while (i > h);
         }
 
         return (result);
 }
 
 /* ----- AA Ellipse */
 
 /* Windows targets do not have lrint, so provide a local inline version */
 #if defined(_MSC_VER)
 /* Detect 64bit and use intrinsic version */
 #ifdef _M_X64
 #include <emmintrin.h>
 static __inline long 
         lrint(float f) 
 {
         return _mm_cvtss_si32(_mm_load_ss(&f));
 }
 #elif defined(_M_IX86)
 __inline long int
         lrint (double flt)
 {       
         int intgr;
         _asm
         {
                 fld flt
                         fistp intgr
         };
         return intgr;
 }
 #elif defined(_M_ARM)
 #include <armintr.h>
 #pragma warning(push)
 #pragma warning(disable: 4716)
 __declspec(naked) long int
         lrint (double flt)
 {
         __emit(0xEC410B10); // fmdrr  d0, r0, r1
         __emit(0xEEBD0B40); // ftosid s0, d0
         __emit(0xEE100A10); // fmrs   r0, s0
         __emit(0xE12FFF1E); // bx     lr
 }
 #pragma warning(pop)
 #else
 #error lrint needed for MSVC on non X86/AMD64/ARM targets.
 #endif
 #endif
 
 int aaellipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return aaellipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3]);
 }
 
 int aaellipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         int i;
         int a2, b2, ds, dt, dxt, t, s, d;
         Sint16 xp, yp, xs, ys, dyt, od, xx, yy, xc2, yc2;
         float cp;
         double sab;
         Uint8 weight, iweight;
 
         /*
         * Sanity check radii 
         */
         if ((rx < 0) || (ry < 0)) {
                 return (-1);
         }
 
         /*
         * Special case for rx=0 - draw a vline 
         */
         if (rx == 0) {
                 return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
         }
         /*
         * Special case for ry=0 - draw an hline 
         */
         if (ry == 0) {
                 return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
         }
 
         /* Variable setup */
         a2 = rx * rx;
         b2 = ry * ry;
 
         ds = 2 * a2;
         dt = 2 * b2;
 
         xc2 = 2 * x;
         yc2 = 2 * y;
 
         sab = sqrt((double)(a2 + b2));
         od = (Sint16)lrint(sab*0.01) + 1; /* introduce some overdraw */
         dxt = (Sint16)lrint((double)a2 / sab) + od;
 
         t = 0;
         s = -2 * a2 * ry;
         d = 0;
 
         xp = x;
         yp = y - ry;
 
         /* Draw */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
 
         /* "End points" */
         result |= pixelRGBA(renderer, xp, yp, r, g, b, a);
         result |= pixelRGBA(renderer, xc2 - xp, yp, r, g, b, a);
         result |= pixelRGBA(renderer, xp, yc2 - yp, r, g, b, a);
         result |= pixelRGBA(renderer, xc2 - xp, yc2 - yp, r, g, b, a);
 
         for (i = 1; i <= dxt; i++) {
                 xp--;
                 d += t - b2;
 
                 if (d >= 0)
                         ys = yp - 1;
                 else if ((d - s - a2) > 0) {
                         if ((2 * d - s - a2) >= 0)
                                 ys = yp + 1;
                         else {
                                 ys = yp;
                                 yp++;
                                 d -= s + a2;
                                 s += ds;
                         }
                 } else {
                         yp++;
                         ys = yp + 1;
                         d -= s + a2;
                         s += ds;
                 }
 
                 t -= dt;
 
                 /* Calculate alpha */
                 if (s != 0) {
                         cp = (float) abs(d) / (float) abs(s);
                         if (cp > 1.0) {
                                 cp = 1.0;
                         }
                 } else {
                         cp = 1.0;
                 }
 
                 /* Calculate weights */
                 weight = (Uint8) (cp * 255);
                 iweight = 255 - weight;
 
                 /* Upper half */
                 xx = xc2 - xp;
                 result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
                 result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);
 
                 result |= pixelRGBAWeight(renderer, xp, ys, r, g, b, a, weight);
                 result |= pixelRGBAWeight(renderer, xx, ys, r, g, b, a, weight);
 
                 /* Lower half */
                 yy = yc2 - yp;
                 result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
                 result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);
 
                 yy = yc2 - ys;
                 result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, weight);
                 result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);
         }
 
         /* Replaces original approximation code dyt = abs(yp - yc); */
         dyt = (Sint16)lrint((double)b2 / sab ) + od;    
 
         for (i = 1; i <= dyt; i++) {
                 yp++;
                 d -= s + a2;
 
                 if (d <= 0)
                         xs = xp + 1;
                 else if ((d + t - b2) < 0) {
                         if ((2 * d + t - b2) <= 0)
                                 xs = xp - 1;
                         else {
                                 xs = xp;
                                 xp--;
                                 d += t - b2;
                                 t -= dt;
                         }
                 } else {
                         xp--;
                         xs = xp - 1;
                         d += t - b2;
                         t -= dt;
                 }
 
                 s += ds;
 
                 /* Calculate alpha */
                 if (t != 0) {
                         cp = (float) abs(d) / (float) abs(t);
                         if (cp > 1.0) {
                                 cp = 1.0;
                         }
                 } else {
                         cp = 1.0;
                 }
 
                 /* Calculate weight */
                 weight = (Uint8) (cp * 255);
                 iweight = 255 - weight;
 
                 /* Left half */
                 xx = xc2 - xp;
                 yy = yc2 - yp;
                 result |= pixelRGBAWeight(renderer, xp, yp, r, g, b, a, iweight);
                 result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, iweight);
 
                 result |= pixelRGBAWeight(renderer, xp, yy, r, g, b, a, iweight);
                 result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, iweight);
 
                 /* Right half */
                 xx = xc2 - xs;
                 result |= pixelRGBAWeight(renderer, xs, yp, r, g, b, a, weight);
                 result |= pixelRGBAWeight(renderer, xx, yp, r, g, b, a, weight);
 
                 result |= pixelRGBAWeight(renderer, xs, yy, r, g, b, a, weight);
                 result |= pixelRGBAWeight(renderer, xx, yy, r, g, b, a, weight);                
         }
 
         return (result);
 }
 
 /* ---- Filled Ellipse */
 
 int filledEllipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return filledEllipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3]);
 }
 
 int filledEllipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         int ix, iy;
         int h, i, j, k;
         int oh, oi, oj, ok;
         int xmh, xph;
         int xmi, xpi;
         int xmj, xpj;
         int xmk, xpk;
 
         /*
         * Sanity check radii 
         */
         if ((rx < 0) || (ry < 0)) {
                 return (-1);
         }
 
         /*
         * Special case for rx=0 - draw a vline 
         */
         if (rx == 0) {
                 return (vlineRGBA(renderer, x, y - ry, y + ry, r, g, b, a));
         }
         /*
         * Special case for ry=0 - draw a hline 
         */
         if (ry == 0) {
                 return (hlineRGBA(renderer, x - rx, x + rx, y, r, g, b, a));
         }
 
         /*
         * Set color
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /*
         * Init vars 
         */
         oh = oi = oj = ok = 0xFFFF;
 
         /*
         * Draw 
         */
         if (rx > ry) {
                 ix = 0;
                 iy = rx * 64;
 
                 do {
                         h = (ix + 32) >> 6;
                         i = (iy + 32) >> 6;
                         j = (h * ry) / rx;
                         k = (i * ry) / rx;
 
                         if ((ok != k) && (oj != k)) {
                                 xph = x + h;
                                 xmh = x - h;
                                 if (k > 0) {
                                         result |= hline(renderer, xmh, xph, y + k);
                                         result |= hline(renderer, xmh, xph, y - k);
                                 } else {
                                         result |= hline(renderer, xmh, xph, y);
                                 }
                                 ok = k;
                         }
                         if ((oj != j) && (ok != j) && (k != j)) {
                                 xmi = x - i;
                                 xpi = x + i;
                                 if (j > 0) {
                                         result |= hline(renderer, xmi, xpi, y + j);
                                         result |= hline(renderer, xmi, xpi, y - j);
                                 } else {
                                         result |= hline(renderer, xmi, xpi, y);
                                 }
                                 oj = j;
                         }
 
                         ix = ix + iy / rx;
                         iy = iy - ix / rx;
 
                 } while (i > h);
         } else {
                 ix = 0;
                 iy = ry * 64;
 
                 do {
                         h = (ix + 32) >> 6;
                         i = (iy + 32) >> 6;
                         j = (h * rx) / ry;
                         k = (i * rx) / ry;
 
                         if ((oi != i) && (oh != i)) {
                                 xmj = x - j;
                                 xpj = x + j;
                                 if (i > 0) {
                                         result |= hline(renderer, xmj, xpj, y + i);
                                         result |= hline(renderer, xmj, xpj, y - i);
                                 } else {
                                         result |= hline(renderer, xmj, xpj, y);
                                 }
                                 oi = i;
                         }
                         if ((oh != h) && (oi != h) && (i != h)) {
                                 xmk = x - k;
                                 xpk = x + k;
                                 if (h > 0) {
                                         result |= hline(renderer, xmk, xpk, y + h);
                                         result |= hline(renderer, xmk, xpk, y - h);
                                 } else {
                                         result |= hline(renderer, xmk, xpk, y);
                                 }
                                 oh = h;
                         }
 
                         ix = ix + iy / ry;
                         iy = iy - ix / ry;
 
                 } while (i > h);
         }
 
         return (result);
 }
 
 /* ----- Pie */
 
 /* TODO: rewrite algorithm; pie is not always accurate */
 int _pieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end,  Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 filled)
 {
         int result;
         double angle, start_angle, end_angle;
         double deltaAngle;
         double dr;
         int numpoints, i;
         Sint16 *vx, *vy;
 
         /*
         * Sanity check radii 
         */
         if (rad < 0) {
                 return (-1);
         }
 
         /*
         * Fixup angles
         */
         start = start % 360;
         end = end % 360;
 
         /*
         * Special case for rad=0 - draw a point 
         */
         if (rad == 0) {
                 return (pixelRGBA(renderer, x, y, r, g, b, a));
         }
 
         /*
         * Variable setup 
         */
         dr = (double) rad;
         deltaAngle = 3.0 / dr;
         start_angle = (double) start *(2.0 * M_PI / 360.0);
         end_angle = (double) end *(2.0 * M_PI / 360.0);
         if (start > end) {
                 end_angle += (2.0 * M_PI);
         }
 
         /* We will always have at least 2 points */
         numpoints = 2;
 
         /* Count points (rather than calculating it) */
         angle = start_angle;
         while (angle < end_angle) {
                 angle += deltaAngle;
                 numpoints++;
         }
 
         /* Allocate combined vertex array */
         vx = vy = (Sint16 *) malloc(2 * sizeof(Uint16) * numpoints);
         if (vx == NULL) {
                 return (-1);
         }
 
         /* Update point to start of vy */
         vy += numpoints;
 
         /* Center */
         vx[0] = x;
         vy[0] = y;
 
         /* First vertex */
         angle = start_angle;
         vx[1] = x + (int) (dr * cos(angle));
         vy[1] = y + (int) (dr * sin(angle));
 
         if (numpoints<3)
         {
                 result = lineRGBA(renderer, vx[0], vy[0], vx[1], vy[1], r, g, b, a);
         }
         else
         {
                 /* Calculate other vertices */
                 i = 2;
                 angle = start_angle;
                 while (angle < end_angle) {
                         angle += deltaAngle;
                         if (angle>end_angle)
                         {
                                 angle = end_angle;
                         }
                         vx[i] = x + (int) (dr * cos(angle));
                         vy[i] = y + (int) (dr * sin(angle));
                         i++;
                 }
 
                 /* Draw */
                 if (filled) {
                         result = filledPolygonRGBA(renderer, vx, vy, numpoints, r, g, b, a);
                 } else {
                         result = polygonRGBA(renderer, vx, vy, numpoints, r, g, b, a);
                 }
         }
 
         /* Free combined vertex array */
         free(vx);
 
         return (result);
 }
 
 int pieColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, 
         Sint16 start, Sint16 end, Uint32 color) 
 {
         Uint8 *c = (Uint8 *)&color; 
         return _pieRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3], 0);
 }
 
 int pieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad,
         Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         return _pieRGBA(renderer, x, y, rad, start, end, r, g, b, a, 0);
 }
 
 int filledPieColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return _pieRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3], 1);
 }
 
 int filledPieRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad,
         Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         return _pieRGBA(renderer, x, y, rad, start, end, r, g, b, a, 1);
 }
 
 /* ------ Trigon */
 
 int trigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
 {
         Sint16 vx[3]; 
         Sint16 vy[3];
 
         vx[0]=x1;
         vx[1]=x2;
         vx[2]=x3;
         vy[0]=y1;
         vy[1]=y2;
         vy[2]=y3;
 
         return(polygonColor(renderer,vx,vy,3,color));
 }
 
 int trigonRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
         Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         Sint16 vx[3]; 
         Sint16 vy[3];
 
         vx[0]=x1;
         vx[1]=x2;
         vx[2]=x3;
         vy[0]=y1;
         vy[1]=y2;
         vy[2]=y3;
 
         return(polygonRGBA(renderer,vx,vy,3,r,g,b,a));
 }                                
 
 /* ------ AA-Trigon */
 
 int aatrigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
 {
         Sint16 vx[3]; 
         Sint16 vy[3];
 
         vx[0]=x1;
         vx[1]=x2;
         vx[2]=x3;
         vy[0]=y1;
         vy[1]=y2;
         vy[2]=y3;
 
         return(aapolygonColor(renderer,vx,vy,3,color));
 }
 
 int aatrigonRGBA(SDL_Renderer * renderer,  Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
         Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         Sint16 vx[3]; 
         Sint16 vy[3];
 
         vx[0]=x1;
         vx[1]=x2;
         vx[2]=x3;
         vy[0]=y1;
         vy[1]=y2;
         vy[2]=y3;
 
         return(aapolygonRGBA(renderer,vx,vy,3,r,g,b,a));
 }                                  
 
 /* ------ Filled Trigon */
 
 int filledTrigonColor(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color)
 {
         Sint16 vx[3]; 
         Sint16 vy[3];
 
         vx[0]=x1;
         vx[1]=x2;
         vx[2]=x3;
         vy[0]=y1;
         vy[1]=y2;
         vy[2]=y3;
 
         return(filledPolygonColor(renderer,vx,vy,3,color));
 }
 
 int filledTrigonRGBA(SDL_Renderer * renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
         Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         Sint16 vx[3]; 
         Sint16 vy[3];
 
         vx[0]=x1;
         vx[1]=x2;
         vx[2]=x3;
         vy[0]=y1;
         vy[1]=y2;
         vy[2]=y3;
 
         return(filledPolygonRGBA(renderer,vx,vy,3,r,g,b,a));
 }
 
 /* ---- Polygon */
 
 int polygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return polygonRGBA(renderer, vx, vy, n, c[0], c[1], c[2], c[3]);
 }
 
 int polygon(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n)
 {
         /*
         * Draw 
         */
         int result;
         int i, nn;
         SDL_Point* points;
 
         /*
         * Vertex array NULL check 
         */
         if (vx == NULL) {
                 return (-1);
         }
         if (vy == NULL) {
                 return (-1);
         }
 
         /*
         * Sanity check 
         */
         if (n < 3) {
                 return (-1);
         }
 
         /*
         * Create array of points
         */
         nn = n + 1;
         points = (SDL_Point*)malloc(sizeof(SDL_Point) * nn);
         if (points == NULL)
         {
                 return -1;
         }
         for (i=0; i<n; i++)
         {
                 points[i].x = vx[i];
                 points[i].y = vy[i];
         }
         points[n].x = vx[0];
         points[n].y = vy[0];
 
         /*
         * Draw 
         */
         result |= SDL_RenderDrawLines(renderer, points, nn);
         free(points);
 
         return (result);
 }
 
 int polygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         /*
         * Draw 
         */
         int result;
         const Sint16 *x1, *y1, *x2, *y2;
 
         /*
         * Vertex array NULL check 
         */
         if (vx == NULL) {
                 return (-1);
         }
         if (vy == NULL) {
                 return (-1);
         }
 
         /*
         * Sanity check 
         */
         if (n < 3) {
                 return (-1);
         }
 
         /*
         * Pointer setup 
         */
         x1 = x2 = vx;
         y1 = y2 = vy;
         x2++;
         y2++;
 
         /*
         * Set color 
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a); 
 
         /*
         * Draw 
         */
         result |= polygon(renderer, vx, vy, n);
 
         return (result);
 }
 
 /* ---- AA-Polygon */
 
 int aapolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return aapolygonRGBA(renderer, vx, vy, n, c[0], c[1], c[2], c[3]);
 }
 
 int aapolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         int i;
         const Sint16 *x1, *y1, *x2, *y2;
 
         /*
         * Vertex array NULL check 
         */
         if (vx == NULL) {
                 return (-1);
         }
         if (vy == NULL) {
                 return (-1);
         }
 
         /*
         * Sanity check 
         */
         if (n < 3) {
                 return (-1);
         }
 
         /*
         * Pointer setup 
         */
         x1 = x2 = vx;
         y1 = y2 = vy;
         x2++;
         y2++;
 
         /*
         * Draw 
         */
         result = 0;
         for (i = 1; i < n; i++) {
                 result |= _aalineRGBA(renderer, *x1, *y1, *x2, *y2, r, g, b, a, 0);
                 x1 = x2;
                 y1 = y2;
                 x2++;
                 y2++;
         }
 
         result |= _aalineRGBA(renderer, *x1, *y1, *vx, *vy, r, g, b, a, 0);
 
         return (result);
 }
 
 /* ---- Filled Polygon */
 
 int _gfxPrimitivesCompareInt(const void *a, const void *b)
 {
         return (*(const int *) a) - (*(const int *) b);
 }
 
 static int *gfxPrimitivesPolyIntsGlobal = NULL;
 
 static int gfxPrimitivesPolyAllocatedGlobal = 0;
 
 int filledPolygonRGBAMT(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int **polyInts, int *polyAllocated)
 {
         int result;
         int i;
         int y, xa, xb;
         int miny, maxy;
         int x1, y1;
         int x2, y2;
         int ind1, ind2;
         int ints;
         int *gfxPrimitivesPolyInts = NULL;
         int *gfxPrimitivesPolyIntsNew = NULL;
         int gfxPrimitivesPolyAllocated = 0;
 
         /*
         * Vertex array NULL check 
         */
         if (vx == NULL) {
                 return (-1);
         }
         if (vy == NULL) {
                 return (-1);
         }
 
         /*
         * Sanity check number of edges
         */
         if (n < 3) {
                 return -1;
         }
 
         /*
         * Map polygon cache  
         */
         if ((polyInts==NULL) || (polyAllocated==NULL)) {
                 /* Use global cache */
                 gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
                 gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
         } else {
                 /* Use local cache */
                 gfxPrimitivesPolyInts = *polyInts;
                 gfxPrimitivesPolyAllocated = *polyAllocated;
         }
 
         /*
         * Allocate temp array, only grow array 
         */
         if (!gfxPrimitivesPolyAllocated) {
                 gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
                 gfxPrimitivesPolyAllocated = n;
         } else {
                 if (gfxPrimitivesPolyAllocated < n) {
                         gfxPrimitivesPolyIntsNew = (int *) realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
                         if (!gfxPrimitivesPolyIntsNew) {
                                 if (!gfxPrimitivesPolyInts) {
                                         free(gfxPrimitivesPolyInts);
                                         gfxPrimitivesPolyInts = NULL;
                                 }
                                 gfxPrimitivesPolyAllocated = 0;
                         } else {
                                 gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsNew;
                                 gfxPrimitivesPolyAllocated = n;
                         }
                 }
         }
 
         /*
         * Check temp array
         */
         if (gfxPrimitivesPolyInts==NULL) {        
                 gfxPrimitivesPolyAllocated = 0;
         }
 
         /*
         * Update cache variables
         */
         if ((polyInts==NULL) || (polyAllocated==NULL)) { 
                 gfxPrimitivesPolyIntsGlobal =  gfxPrimitivesPolyInts;
                 gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
         } else {
                 *polyInts = gfxPrimitivesPolyInts;
                 *polyAllocated = gfxPrimitivesPolyAllocated;
         }
 
         /*
         * Check temp array again
         */
         if (gfxPrimitivesPolyInts==NULL) {        
                 return(-1);
         }
 
         /*
         * Determine Y maxima 
         */
         miny = vy[0];
         maxy = vy[0];
         for (i = 1; (i < n); i++) {
                 if (vy[i] < miny) {
                         miny = vy[i];
                 } else if (vy[i] > maxy) {
                         maxy = vy[i];
                 }
         }
 
         /*
         * Draw, scanning y 
         */
         result = 0;
         for (y = miny; (y <= maxy); y++) {
                 ints = 0;
                 for (i = 0; (i < n); i++) {
                         if (!i) {
                                 ind1 = n - 1;
                                 ind2 = 0;
                         } else {
                                 ind1 = i - 1;
                                 ind2 = i;
                         }
                         y1 = vy[ind1];
                         y2 = vy[ind2];
                         if (y1 < y2) {
                                 x1 = vx[ind1];
                                 x2 = vx[ind2];
                         } else if (y1 > y2) {
                                 y2 = vy[ind1];
                                 y1 = vy[ind2];
                                 x2 = vx[ind1];
                                 x1 = vx[ind2];
                         } else {
                                 continue;
                         }
                         if ( ((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)) ) {
                                 gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
                         }           
                 }
 
                 qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);
 
                 /*
                 * Set color 
                 */
                 result = 0;
             result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
                 result |= SDL_SetRenderDrawColor(renderer, r, g, b, a); 
 
                 for (i = 0; (i < ints); i += 2) {
                         xa = gfxPrimitivesPolyInts[i] + 1;
                         xa = (xa >> 16) + ((xa & 32768) >> 15);
                         xb = gfxPrimitivesPolyInts[i+1] - 1;
                         xb = (xb >> 16) + ((xb & 32768) >> 15);
                         result |= hline(renderer, xa, xb, y);
                 }
         }
 
         return (result);
 }
 
 int filledPolygonColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return filledPolygonRGBAMT(renderer, vx, vy, n, c[0], c[1], c[2], c[3], NULL, NULL);
 }
 
 int filledPolygonRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         return filledPolygonRGBAMT(renderer, vx, vy, n, r, g, b, a, NULL, NULL);
 }
 
 /* ---- Textured Polygon */
 
 int _HLineTextured(SDL_Renderer *renderer, Sint16 x1, Sint16 x2, Sint16 y, SDL_Texture *texture, int texture_w, int texture_h, int texture_dx, int texture_dy)
 {
         Sint16 w;
         Sint16 xtmp;
         int result = 0;
         int texture_x_walker;    
         int texture_y_start;    
         SDL_Rect source_rect,dst_rect;
         int pixels_written,write_width;
 
         /*
         * Swap x1, x2 if required to ensure x1<=x2
         */
         if (x1 > x2) {
                 xtmp = x1;
                 x1 = x2;
                 x2 = xtmp;
         }
 
         /*
         * Calculate width to draw
         */
         w = x2 - x1 + 1;
 
         /*
         * Determine where in the texture we start drawing
         */
         texture_x_walker =   (x1 - texture_dx)  % texture_w;
         if (texture_x_walker < 0){
                 texture_x_walker = texture_w + texture_x_walker ;
         }
 
         texture_y_start = (y + texture_dy) % texture_h;
         if (texture_y_start < 0){
                 texture_y_start = texture_h + texture_y_start;
         }
 
         // setup the source rectangle; we are only drawing one horizontal line
         source_rect.y = texture_y_start;
         source_rect.x = texture_x_walker;
         source_rect.h = 1;
 
         // we will draw to the current y
         dst_rect.y = y;
         dst_rect.h = 1;
 
         // if there are enough pixels left in the current row of the texture
         // draw it all at once
         if (w <= texture_w -texture_x_walker){
                 source_rect.w = w;
                 source_rect.x = texture_x_walker;
                 dst_rect.x= x1;
                 dst_rect.w = source_rect.w;
                 result = (SDL_RenderCopy(renderer, texture, &source_rect, &dst_rect) == 0);
         } else { 
                 // we need to draw multiple times
                 // draw the first segment
                 pixels_written = texture_w  - texture_x_walker;
                 source_rect.w = pixels_written;
                 source_rect.x = texture_x_walker;
                 dst_rect.x= x1;
                 dst_rect.w = source_rect.w;
                 result |= (SDL_RenderCopy(renderer, texture, &source_rect, &dst_rect) == 0);
                 write_width = texture_w;
 
                 // now draw the rest
                 // set the source x to 0
                 source_rect.x = 0;
                 while (pixels_written < w){
                         if (write_width >= w - pixels_written) {
                                 write_width =  w - pixels_written;
                         }
                         source_rect.w = write_width;
                         dst_rect.x = x1 + pixels_written;
                         dst_rect.w = source_rect.w;
                         result |= (SDL_RenderCopy(renderer, texture, &source_rect, &dst_rect) == 0);
                         pixels_written += write_width;
                 }
         }
 
         return result;
 }
 
 int texturedPolygonMT(SDL_Renderer *renderer, const Sint16 * vx, const Sint16 * vy, int n, 
         SDL_Surface * texture, int texture_dx, int texture_dy, int **polyInts, int *polyAllocated)
 {
         int result;
         int i;
         int y, xa, xb;
         int minx,maxx,miny, maxy;
         int x1, y1;
         int x2, y2;
         int ind1, ind2;
         int ints;
         int *gfxPrimitivesPolyInts = NULL;
         int gfxPrimitivesPolyAllocated = 0;
         SDL_Texture *textureAsTexture = NULL;
 
         /*
         * Sanity check number of edges
         */
         if (n < 3) {
                 return -1;
         }
 
         /*
         * Map polygon cache  
         */
         if ((polyInts==NULL) || (polyAllocated==NULL)) {
                 /* Use global cache */
                 gfxPrimitivesPolyInts = gfxPrimitivesPolyIntsGlobal;
                 gfxPrimitivesPolyAllocated = gfxPrimitivesPolyAllocatedGlobal;
         } else {
                 /* Use local cache */
                 gfxPrimitivesPolyInts = *polyInts;
                 gfxPrimitivesPolyAllocated = *polyAllocated;
         }
 
         /*
         * Allocate temp array, only grow array 
         */
         if (!gfxPrimitivesPolyAllocated) {
                 gfxPrimitivesPolyInts = (int *) malloc(sizeof(int) * n);
                 gfxPrimitivesPolyAllocated = n;
         } else {
                 if (gfxPrimitivesPolyAllocated < n) {
                         gfxPrimitivesPolyInts = (int *) realloc(gfxPrimitivesPolyInts, sizeof(int) * n);
                         gfxPrimitivesPolyAllocated = n;
                 }
         }
 
         /*
         * Check temp array
         */
         if (gfxPrimitivesPolyInts==NULL) {        
                 gfxPrimitivesPolyAllocated = 0;
         }
 
         /*
         * Update cache variables
         */
         if ((polyInts==NULL) || (polyAllocated==NULL)) { 
                 gfxPrimitivesPolyIntsGlobal =  gfxPrimitivesPolyInts;
                 gfxPrimitivesPolyAllocatedGlobal = gfxPrimitivesPolyAllocated;
         } else {
                 *polyInts = gfxPrimitivesPolyInts;
                 *polyAllocated = gfxPrimitivesPolyAllocated;
         }
 
         /*
         * Check temp array again
         */
         if (gfxPrimitivesPolyInts==NULL) {        
                 return(-1);
         }
 
         /*
         * Determine X,Y minima,maxima 
         */
         miny = vy[0];
         maxy = vy[0];
         minx = vx[0];
         maxx = vx[0];
         for (i = 1; (i < n); i++) {
                 if (vy[i] < miny) {
                         miny = vy[i];
                 } else if (vy[i] > maxy) {
                         maxy = vy[i];
                 }
                 if (vx[i] < minx) {
                         minx = vx[i];
                 } else if (vx[i] > maxx) {
                         maxx = vx[i];
                 }
         }
 
     /* Create texture for drawing */
         textureAsTexture = SDL_CreateTextureFromSurface(renderer, texture);
         if (textureAsTexture == NULL)
         {
                 return -1;
         }
         SDL_SetTextureBlendMode(textureAsTexture, SDL_BLENDMODE_BLEND);
         
         /*
         * Draw, scanning y 
         */
         result = 0;
         for (y = miny; (y <= maxy); y++) {
                 ints = 0;
                 for (i = 0; (i < n); i++) {
                         if (!i) {
                                 ind1 = n - 1;
                                 ind2 = 0;
                         } else {
                                 ind1 = i - 1;
                                 ind2 = i;
                         }
                         y1 = vy[ind1];
                         y2 = vy[ind2];
                         if (y1 < y2) {
                                 x1 = vx[ind1];
                                 x2 = vx[ind2];
                         } else if (y1 > y2) {
                                 y2 = vy[ind1];
                                 y1 = vy[ind2];
                                 x2 = vx[ind1];
                                 x1 = vx[ind2];
                         } else {
                                 continue;
                         }
                         if ( ((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)) ) {
                                 gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
                         } 
                 }
 
                 qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _gfxPrimitivesCompareInt);
 
                 for (i = 0; (i < ints); i += 2) {
                         xa = gfxPrimitivesPolyInts[i] + 1;
                         xa = (xa >> 16) + ((xa & 32768) >> 15);
                         xb = gfxPrimitivesPolyInts[i+1] - 1;
                         xb = (xb >> 16) + ((xb & 32768) >> 15);
                         result |= _HLineTextured(renderer, xa, xb, y, textureAsTexture, texture->w, texture->h, texture_dx, texture_dy);
                 }
         }
 
         SDL_RenderPresent(renderer);
         SDL_DestroyTexture(textureAsTexture);
 
         return (result);
 }
 
 int texturedPolygon(SDL_Renderer *renderer, const Sint16 * vx, const Sint16 * vy, int n, SDL_Surface *texture, int texture_dx, int texture_dy)
 {
         /*
         * Draw
         */
         return (texturedPolygonMT(renderer, vx, vy, n, texture, texture_dx, texture_dy, NULL, NULL));
 }
 
 /* ---- Character */
 
 static SDL_Texture *gfxPrimitivesFont[256];
 
 static const unsigned char *currentFontdata = gfxPrimitivesFontdata;
 
 static Uint32 charWidth = 8;
 
 static Uint32 charHeight = 8;
 
 static Uint32 charWidthLocal = 8;
 
 static Uint32 charHeightLocal = 8;
 
 static Uint32 charPitch = 1;
 
 static Uint32 charRotation = 0;
 
 static Uint32 charSize = 8;
 
 void gfxPrimitivesSetFont(const void *fontdata, Uint32 cw, Uint32 ch)
 {
         int i;
 
         if ((fontdata) && (cw) && (ch)) {
                 currentFontdata = (unsigned char *)fontdata;
                 charWidth = cw;
                 charHeight = ch;
         } else {
                 currentFontdata = gfxPrimitivesFontdata;
                 charWidth = 8;
                 charHeight = 8;
         }
 
         charPitch = (charWidth+7)/8;
         charSize = charPitch * charHeight;
 
         /* Maybe flip width/height for rendering */
         if ((charRotation==1) || (charRotation==3))
         {
                 charWidthLocal = charHeight;
                 charHeightLocal = charWidth;
         }
         else
         {
                 charWidthLocal = charWidth;
                 charHeightLocal = charHeight;
         }
 
         /* Clear character cache */
         for (i = 0; i < 256; i++) {
                 if (gfxPrimitivesFont[i]) {
                         SDL_DestroyTexture(gfxPrimitivesFont[i]);
                         gfxPrimitivesFont[i] = NULL;
                 }
         }
 }
 
 void gfxPrimitivesSetFontRotation(Uint32 rotation)
 {
         int i;
 
         rotation = rotation & 3;
         if (charRotation != rotation)
         {
                 /* Store rotation */
                 charRotation = rotation;
 
                 /* Maybe flip width/height for rendering */
                 if ((charRotation==1) || (charRotation==3))
                 {
                         charWidthLocal = charHeight;
                         charHeightLocal = charWidth;
                 }
                 else
                 {
                         charWidthLocal = charWidth;
                         charHeightLocal = charHeight;
                 }
 
                 /* Clear character cache */
                 for (i = 0; i < 256; i++) {
                         if (gfxPrimitivesFont[i]) {
                                 SDL_DestroyTexture(gfxPrimitivesFont[i]);
                                 gfxPrimitivesFont[i] = NULL;
                         }
                 }
         }
 }
 
 int characterRGBA(SDL_Renderer *renderer, Sint16 x, Sint16 y, char c, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         SDL_Rect srect;
         SDL_Rect drect;
         int result;
         Uint32 ix, iy;
         const unsigned char *charpos;
         Uint8 *curpos;
         Uint8 patt, mask;
         Uint8 *linepos;
         Uint32 pitch;
         SDL_Surface *character;
         SDL_Surface *rotatedCharacter;
         Uint32 ci;
 
         /*
         * Setup source rectangle
         */
         srect.x = 0;
         srect.y = 0;
         srect.w = charWidthLocal;
         srect.h = charHeightLocal;
 
         /*
         * Setup destination rectangle
         */
         drect.x = x;
         drect.y = y;
         drect.w = charWidthLocal;
         drect.h = charHeightLocal;
 
         /* Character index in cache */
         ci = (unsigned char) c;
 
         /*
         * Create new charWidth x charHeight bitmap surface if not already present.
         * Might get rotated later.
         */
         if (gfxPrimitivesFont[ci] == NULL) {
                 /*
                 * Redraw character into surface
                 */
                 character =     SDL_CreateRGBSurface(SDL_SWSURFACE,
                         charWidth, charHeight, 32,
                         0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
                 if (character == NULL) {
                         return (-1);
                 }
 
                 charpos = currentFontdata + ci * charSize;
                                 linepos = (Uint8 *)character->pixels;
                 pitch = character->pitch;
 
                 /*
                 * Drawing loop 
                 */
                 patt = 0;
                 for (iy = 0; iy < charHeight; iy++) {
                         mask = 0x00;
                         curpos = linepos;
                         for (ix = 0; ix < charWidth; ix++) {
                                 if (!(mask >>= 1)) {
                                         patt = *charpos++;
                                         mask = 0x80;
                                 }
                                 if (patt & mask) {
                                         *(Uint32 *)curpos = 0xffffffff;
                                 } else {
                                         *(Uint32 *)curpos = 0;
                                 }
                                 curpos += 4;
                         }
                         linepos += pitch;
                 }
 
                 /* Maybe rotate and replace cached image */
                 if (charRotation>0)
                 {
                         rotatedCharacter = rotateSurface90Degrees(character, charRotation);
                         SDL_FreeSurface(character);
                         character = rotatedCharacter;
                 }
 
                 /* Convert temp surface into texture */
                 gfxPrimitivesFont[ci] = SDL_CreateTextureFromSurface(renderer, character);
                 SDL_FreeSurface(character);
 
                 /*
                 * Check pointer 
                 */
                 if (gfxPrimitivesFont[ci] == NULL) {
                         return (-1);
                 }
         }
 
         /*
         * Set color 
         */
         result = 0;
         result |= SDL_SetTextureColorMod(gfxPrimitivesFont[ci], r, g, b);
         result |= SDL_SetTextureAlphaMod(gfxPrimitivesFont[ci], a);
 
         /*
         * Draw texture onto destination 
         */
         result |= SDL_RenderCopy(renderer, gfxPrimitivesFont[ci], &srect, &drect);
 
         return (result);
 }
 
 
 int characterColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, char c, Uint32 color)
 {
         Uint8 *co = (Uint8 *)&color; 
         return characterRGBA(renderer, x, y, c, co[0], co[1], co[2], co[3]);
 }
 
 
 int stringColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, const char *s, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return stringRGBA(renderer, x, y, s, c[0], c[1], c[2], c[3]);
 }
 
 int stringRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, const char *s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result = 0;
         Sint16 curx = x;
         Sint16 cury = y;
         const char *curchar = s;
 
         while (*curchar && !result) {
                 result |= characterRGBA(renderer, curx, cury, *curchar, r, g, b, a);
                 switch (charRotation)
                 {
                 case 0:
                         curx += charWidthLocal;
                         break;
                 case 2:
                         curx -= charWidthLocal;
                         break;
                 case 1:
                         cury += charHeightLocal;
                         break;
                 case 3:
                         cury -= charHeightLocal;
                         break;
                 }
                 curchar++;
         }
 
         return (result);
 }
 
 /* ---- Bezier curve */
 
 double _evaluateBezier (double *data, int ndata, double t) 
 {
         double mu, result;
         int n,k,kn,nn,nkn;
         double blend,muk,munk;
 
         /* Sanity check bounds */
         if (t<0.0) {
                 return(data[0]);
         }
         if (t>=(double)ndata) {
                 return(data[ndata-1]);
         }
 
         /* Adjust t to the range 0.0 to 1.0 */ 
         mu=t/(double)ndata;
 
         /* Calculate interpolate */
         n=ndata-1;
         result=0.0;
         muk = 1;
         munk = pow(1-mu,(double)n);
         for (k=0;k<=n;k++) {
                 nn = n;
                 kn = k;
                 nkn = n - k;
                 blend = muk * munk;
                 muk *= mu;
                 munk /= (1-mu);
                 while (nn >= 1) {
                         blend *= nn;
                         nn--;
                         if (kn > 1) {
                                 blend /= (double)kn;
                                 kn--;
                         }
                         if (nkn > 1) {
                                 blend /= (double)nkn;
                                 nkn--;
                         }
                 }
                 result += data[k] * blend;
         }
 
         return (result);
 }
 
 int bezierColor(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint32 color)
 {
         Uint8 *c = (Uint8 *)&color; 
         return bezierRGBA(renderer, vx, vy, n, s, c[0], c[1], c[2], c[3]);
 }
 
 int bezierRGBA(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         int i;
         double *x, *y, t, stepsize;
         Sint16 x1, y1, x2, y2;
 
         /*
         * Sanity check 
         */
         if (n < 3) {
                 return (-1);
         }
         if (s < 2) {
                 return (-1);
         }
 
         /*
         * Variable setup 
         */
         stepsize=(double)1.0/(double)s;
 
         /* Transfer vertices into float arrays */
         if ((x=(double *)malloc(sizeof(double)*(n+1)))==NULL) {
                 return(-1);
         }
         if ((y=(double *)malloc(sizeof(double)*(n+1)))==NULL) {
                 free(x);
                 return(-1);
         }    
         for (i=0; i<n; i++) {
                 x[i]=(double)vx[i];
                 y[i]=(double)vy[i];
         }      
         x[n]=(double)vx[0];
         y[n]=(double)vy[0];
 
         /*
         * Set color 
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /*
         * Draw 
         */
         t=0.0;
         x1=(Sint16)lrint(_evaluateBezier(x,n+1,t));
         y1=(Sint16)lrint(_evaluateBezier(y,n+1,t));
         for (i = 0; i <= (n*s); i++) {
                 t += stepsize;
                 x2=(Sint16)_evaluateBezier(x,n,t);
                 y2=(Sint16)_evaluateBezier(y,n,t);
                 result |= line(renderer, x1, y1, x2, y2);
                 x1 = x2;
                 y1 = y2;
         }
 
         /* Clean up temporary array */
         free(x);
         free(y);
 
         return (result);
 }
 
 
 /* ---- Thick Line */
 
 int _bresenhamInitialize(SDL2_gfxBresenhamIterator *b, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2)
 {
         int temp;
 
         if (b==NULL) {
                 return(-1);
         }
 
         b->x = x1;
         b->y = y1;
 
         /* dx = abs(x2-x1), s1 = sign(x2-x1) */
         if ((b->dx = x2 - x1) != 0) {
                 if (b->dx < 0) {
                         b->dx = -b->dx;
                         b->s1 = -1;
                 } else {
                         b->s1 = 1;
                 }
         } else {
                 b->s1 = 0;      
         }
 
         /* dy = abs(y2-y1), s2 = sign(y2-y1)    */
         if ((b->dy = y2 - y1) != 0) {
                 if (b->dy < 0) {
                         b->dy = -b->dy;
                         b->s2 = -1;
                 } else {
                         b->s2 = 1;
                 }
         } else {
                 b->s2 = 0;      
         }
 
         if (b->dy > b->dx) {
                 temp = b->dx;
                 b->dx = b->dy;
                 b->dy = temp;
                 b->swapdir = 1;
         } else {
                 b->swapdir = 0;
         }
 
         b->count = (b->dx<0) ? 0 : (unsigned int)b->dx;
         b->dy <<= 1;
         b->error = b->dy - b->dx;
         b->dx <<= 1;    
 
         return(0);
 }
 
 
 int _bresenhamIterate(SDL2_gfxBresenhamIterator *b)
 {       
         if (b==NULL) {
                 return (-1);
         }
 
         /* last point check */
         if (b->count==0) {
                 return (2);
         }
 
         while (b->error >= 0) {
                 if (b->swapdir) {
                         b->x += b->s1;
                 } else  {
                         b->y += b->s2;
                 }
 
                 b->error -= b->dx;
         }
 
         if (b->swapdir) {
                 b->y += b->s2;
         } else {
                 b->x += b->s1;
         }
 
         b->error += b->dy;      
         b->count--;             
 
         /* count==0 indicates "end-of-line" */
         return ((b->count) ? 0 : 1);
 }
 
 
 void _murphyParaline(SDL2_gfxMurphyIterator *m, Sint16 x, Sint16 y, int d1)
 {
         int p;
         d1 = -d1;
 
         for (p = 0; p <= m->u; p++) {
 
                 pixel(m->renderer, x, y);
 
                 if (d1 <= m->kt) {
                         if (m->oct2 == 0) {
                                 x++;
                         } else {
                                 if (m->quad4 == 0) {
                                         y++;
                                 } else {
                                         y--;
                                 }
                         }
                         d1 += m->kv;
                 } else {        
                         x++;
                         if (m->quad4 == 0) {
                                 y++;
                         } else {
                                 y--;
                         }
                         d1 += m->kd;
                 }
         }
 
         m->tempx = x;
         m->tempy = y;
 }
 
 void _murphyIteration(SDL2_gfxMurphyIterator *m, Uint8 miter, 
         Uint16 ml1bx, Uint16 ml1by, Uint16 ml2bx, Uint16 ml2by, 
         Uint16 ml1x, Uint16 ml1y, Uint16 ml2x, Uint16 ml2y)
 {
         int atemp1, atemp2;
         int ftmp1, ftmp2;
         Uint16 m1x, m1y, m2x, m2y;      
         Uint16 fix, fiy, lax, lay, curx, cury;
         Sint16 px[4], py[4];
         SDL2_gfxBresenhamIterator b;
 
         if (miter > 1) {
                 if (m->first1x != -32768) {
                         fix = (m->first1x + m->first2x) / 2;
                         fiy = (m->first1y + m->first2y) / 2;
                         lax = (m->last1x + m->last2x) / 2;
                         lay = (m->last1y + m->last2y) / 2;
                         curx = (ml1x + ml2x) / 2;
                         cury = (ml1y + ml2y) / 2;
 
                         atemp1 = (fix - curx);
                         atemp2 = (fiy - cury);
                         ftmp1 = atemp1 * atemp1 + atemp2 * atemp2;
                         atemp1 = (lax - curx);
                         atemp2 = (lay - cury);
                         ftmp2 = atemp1 * atemp1 + atemp2 * atemp2;
 
                         if (ftmp1 <= ftmp2) {
                                 m1x = m->first1x;
                                 m1y = m->first1y;
                                 m2x = m->first2x;
                                 m2y = m->first2y;
                         } else {
                                 m1x = m->last1x;
                                 m1y = m->last1y;
                                 m2x = m->last2x;
                                 m2y = m->last2y;
                         }
 
                         atemp1 = (m2x - ml2x);
                         atemp2 = (m2y - ml2y);
                         ftmp1 = atemp1 * atemp1 + atemp2 * atemp2;
                         atemp1 = (m2x - ml2bx);
                         atemp2 = (m2y - ml2by);
                         ftmp2 = atemp1 * atemp1 + atemp2 * atemp2;
 
                         if (ftmp2 >= ftmp1) {
                                 ftmp1 = ml2bx;
                                 ftmp2 = ml2by;
                                 ml2bx = ml2x;
                                 ml2by = ml2y;
                                 ml2x = ftmp1;
                                 ml2y = ftmp2;
                                 ftmp1 = ml1bx;
                                 ftmp2 = ml1by;
                                 ml1bx = ml1x;
                                 ml1by = ml1y;
                                 ml1x = ftmp1;
                                 ml1y = ftmp2;
                         }
 
                         /*
                         * Lock the surface 
                         */
                         _bresenhamInitialize(&b, m2x, m2y, m1x, m1y);
                         do {
                                 pixel(m->renderer, b.x, b.y);
                         } while (_bresenhamIterate(&b)==0);
 
                         _bresenhamInitialize(&b, m1x, m1y, ml1bx, ml1by);
                         do {
                                 pixel(m->renderer, b.x, b.y);
                         } while (_bresenhamIterate(&b)==0);
 
                         _bresenhamInitialize(&b, ml1bx, ml1by, ml2bx, ml2by);
                         do {
                                 pixel(m->renderer, b.x, b.y);
                         } while (_bresenhamIterate(&b)==0);
 
                         _bresenhamInitialize(&b, ml2bx, ml2by, m2x, m2y);
                         do {
                                 pixel(m->renderer, b.x, b.y);
                         } while (_bresenhamIterate(&b)==0);
 
                         px[0] = m1x;
                         px[1] = m2x;
                         px[2] = ml1bx;
                         px[3] = ml2bx;
                         py[0] = m1y;
                         py[1] = m2y;
                         py[2] = ml1by;
                         py[3] = ml2by;                  
                         polygon(m->renderer, px, py, 4);                                                
                 }
         }
 
         m->last1x = ml1x;
         m->last1y = ml1y;
         m->last2x = ml2x;
         m->last2y = ml2y;
         m->first1x = ml1bx;
         m->first1y = ml1by;
         m->first2x = ml2bx;
         m->first2y = ml2by;
 }
 
 
 #define HYPOT(x,y) sqrt((double)(x)*(double)(x)+(double)(y)*(double)(y)) 
 
 void _murphyWideline(SDL2_gfxMurphyIterator *m, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint8 miter)
 {       
         float offset = (float)width / 2.f;
 
         Sint16 temp;
         Sint16 ptx, pty, ptxx, ptxy, ml1x, ml1y, ml2x, ml2y, ml1bx, ml1by, ml2bx, ml2by;
 
         int d0, d1;             /* difference terms d0=perpendicular to line, d1=along line */
 
         int q;                  /* pel counter,q=perpendicular to line */
         int tmp;
 
         int dd;                 /* distance along line */
         int tk;                 /* thickness threshold */
         double ang;             /* angle for initial point calculation */
         double sang, cang;
 
         /* Initialisation */
         m->u = x2 - x1; /* delta x */
         m->v = y2 - y1; /* delta y */
 
         if (m->u < 0) { /* swap to make sure we are in quadrants 1 or 4 */
                 temp = x1;
                 x1 = x2;
                 x2 = temp;
                 temp = y1;
                 y1 = y2;
                 y2 = temp;              
                 m->u *= -1;
                 m->v *= -1;
         }
 
         if (m->v < 0) { /* swap to 1st quadrant and flag */
                 m->v *= -1;
                 m->quad4 = 1;
         } else {
                 m->quad4 = 0;
         }
 
         if (m->v > m->u) {      /* swap things if in 2 octant */
                 tmp = m->u;
                 m->u = m->v;
                 m->v = tmp;
                 m->oct2 = 1;
         } else {
                 m->oct2 = 0;
         }
 
         m->ku = m->u + m->u;    /* change in l for square shift */
         m->kv = m->v + m->v;    /* change in d for square shift */
         m->kd = m->kv - m->ku;  /* change in d for diagonal shift */
         m->kt = m->u - m->kv;   /* diag/square decision threshold */
 
         d0 = 0;
         d1 = 0;
         dd = 0;
 
         ang = atan((double) m->v / (double) m->u);      /* calc new initial point - offset both sides of ideal */       
         sang = sin(ang);
         cang = cos(ang);
 
         if (m->oct2 == 0) {
                 ptx = x1 + (Sint16)lrint(offset * sang);
                 if (m->quad4 == 0) {
                         pty = y1 - (Sint16)lrint(offset * cang);
                 } else {
                         pty = y1 + (Sint16)lrint(offset * cang);
                 }
         } else {
                 ptx = x1 - (Sint16)lrint(offset * cang);
                 if (m->quad4 == 0) {
                         pty = y1 + (Sint16)lrint(offset * sang);
                 } else {
                         pty = y1 - (Sint16)lrint(offset * sang);
                 }
         }
 
         /* used here for constant thickness line */
         tk = (int) (4. * HYPOT(ptx - x1, pty - y1) * HYPOT(m->u, m->v));
 
         if (miter == 0) {
                 m->first1x = -32768;
                 m->first1y = -32768;
                 m->first2x = -32768;
                 m->first2y = -32768;
                 m->last1x = -32768;
                 m->last1y = -32768;
                 m->last2x = -32768;
                 m->last2y = -32768;
         }
         ptxx = ptx;
         ptxy = pty;
 
         for (q = 0; dd <= tk; q++) {    /* outer loop, stepping perpendicular to line */
 
                 _murphyParaline(m, ptx, pty, d1);       /* call to inner loop - right edge */
                 if (q == 0) {
                         ml1x = ptx;
                         ml1y = pty;
                         ml1bx = m->tempx;
                         ml1by = m->tempy;
                 } else {
                         ml2x = ptx;
                         ml2y = pty;
                         ml2bx = m->tempx;
                         ml2by = m->tempy;
                 }
                 if (d0 < m->kt) {       /* square move */
                         if (m->oct2 == 0) {
                                 if (m->quad4 == 0) {
                                         pty++;
                                 } else {
                                         pty--;
                                 }
                         } else {
                                 ptx++;
                         }
                 } else {        /* diagonal move */
                         dd += m->kv;
                         d0 -= m->ku;
                         if (d1 < m->kt) {       /* normal diagonal */
                                 if (m->oct2 == 0) {
                                         ptx--;
                                         if (m->quad4 == 0) {
                                                 pty++;
                                         } else {
                                                 pty--;
                                         }
                                 } else {
                                         ptx++;
                                         if (m->quad4 == 0) {
                                                 pty--;
                                         } else {
                                                 pty++;
                                         }
                                 }
                                 d1 += m->kv;
                         } else {        /* double square move, extra parallel line */
                                 if (m->oct2 == 0) {
                                         ptx--;
                                 } else {
                                         if (m->quad4 == 0) {
                                                 pty--;
                                         } else {
                                                 pty++;
                                         }
                                 }
                                 d1 += m->kd;
                                 if (dd > tk) {
                                         _murphyIteration(m, miter, ml1bx, ml1by, ml2bx, ml2by, ml1x, ml1y, ml2x, ml2y);
                                         return; /* breakout on the extra line */
                                 }
                                 _murphyParaline(m, ptx, pty, d1);
                                 if (m->oct2 == 0) {
                                         if (m->quad4 == 0) {
                                                 pty++;
                                         } else {
 
                                                 pty--;
                                         }
                                 } else {
                                         ptx++;
                                 }
                         }
                 }
                 dd += m->ku;
                 d0 += m->kv;
         }
 
         _murphyIteration(m, miter, ml1bx, ml1by, ml2bx, ml2by, ml1x, ml1y, ml2x, ml2y);
 }
 
 
 int thickLineColor(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint32 color)
 {       
         Uint8 *c = (Uint8 *)&color; 
         return thickLineRGBA(renderer, x1, y1, x2, y2, width, c[0], c[1], c[2], c[3]);
 }
 
 int thickLineRGBA(SDL_Renderer *renderer, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
 {
         int result;
         int wh;
         SDL2_gfxMurphyIterator m;
 
         if (renderer == NULL) {
                 return -1;
         }
         if (width < 1) {
                 return -1;
         }
 
         /* Special case: thick "point" */
         if ((x1 == x2) && (y1 == y2)) {
                 wh = width / 2;
                 return boxRGBA(renderer, x1 - wh, y1 - wh, x2 + width, y2 + width, r, g, b, a);         
         }
 
         /*
         * Set color
         */
         result = 0;
         result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
         result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);
 
         /* 
         * Draw
         */
         m.renderer = renderer;
         _murphyWideline(&m, x1, y1, x2, y2, width, 0);
         _murphyWideline(&m, x1, y1, x2, y2, width, 1);
 
         return(0);
 }