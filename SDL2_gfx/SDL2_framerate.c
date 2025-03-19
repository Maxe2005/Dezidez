 /*
 
 SDL_framerate.c: framerate manager
 
 Copyright (C) 2001-2012  Andreas Schiffler
 
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
 
 #include "SDL2_framerate.h"
 
 Uint32 _getTicks()
 {
         Uint32 ticks = SDL_GetTicks();
 
         /* 
         * Since baseticks!=0 is used to track initialization
         * we need to ensure that the tick count is always >0 
         * since SDL_GetTicks may not have incremented yet and
         * return 0 depending on the timing of the calls.
         */
         if (ticks == 0) {
                 return 1;
         } else {
                 return ticks;
         }
 }
 
 void SDL_initFramerate(FPSmanager * manager)
 {
         /*
         * Store some sane values 
         */
         manager->framecount = 0;
         manager->rate = FPS_DEFAULT;
         manager->rateticks = (1000.0f / (float) FPS_DEFAULT);
         manager->baseticks = _getTicks();
         manager->lastticks = manager->baseticks;
 
 }
 
 int SDL_setFramerate(FPSmanager * manager, Uint32 rate)
 {
         if ((rate >= FPS_LOWER_LIMIT) && (rate <= FPS_UPPER_LIMIT)) {
                 manager->framecount = 0;
                 manager->rate = rate;
                 manager->rateticks = (1000.0f / (float) rate);
                 return (0);
         } else {
                 return (-1);
         }
 }
 
 int SDL_getFramerate(FPSmanager * manager)
 {
         if (manager == NULL) {
                 return (-1);
         } else {
                 return ((int)manager->rate);
         }
 }
 
 int SDL_getFramecount(FPSmanager * manager)
 {
         if (manager == NULL) {
                 return (-1);
         } else {
                 return ((int)manager->framecount);
         }
 }
 
 Uint32 SDL_framerateDelay(FPSmanager * manager)
 {
         Uint32 current_ticks;
         Uint32 target_ticks;
         Uint32 the_delay;
         Uint32 time_passed = 0;
 
         /*
         * No manager, no delay
         */
         if (manager == NULL) {
                 return 0;
         }
 
         /*
         * Initialize uninitialized manager 
         */
         if (manager->baseticks == 0) {
                 SDL_initFramerate(manager);
         }
 
         /*
         * Next frame 
         */
         manager->framecount++;
 
         /*
         * Get/calc ticks 
         */
         current_ticks = _getTicks();
         time_passed = current_ticks - manager->lastticks;
         manager->lastticks = current_ticks;
         target_ticks = manager->baseticks + (Uint32) ((float) manager->framecount * manager->rateticks);
 
         if (current_ticks <= target_ticks) {
                 the_delay = target_ticks - current_ticks;
                 SDL_Delay(the_delay);
         } else {
                 manager->framecount = 0;
                 manager->baseticks = _getTicks();
         }
 
         return time_passed;
 }