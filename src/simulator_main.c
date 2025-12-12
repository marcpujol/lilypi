/**
 * @file simulator_main.c
 * Main file for simulator
 */

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "lv_conf_sim.h"
#include "lvgl.h"
#include "hal/hal.h"

/* Import the demo widgets function */
extern void lv_demo_widgets(void);

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("Starting LilyPi Simulator...\n");

    /* Initialize LVGL */
    lv_init();

    /* Initialize the HAL (display, input devices, tick) for LVGL */
    /* Using 320x480 to match the LilyPi display size */
    sdl_hal_init(320, 480);

    /* Run the demo widgets (same as on the real device) */
    lv_demo_widgets();

    printf("Simulator running. Close the window to exit.\n");

    /* Main loop */
    SDL_Event event;
    bool quit = false;

    while(!quit) {
        /* Handle SDL events */
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        /* Periodically call the lv_task handler (LVGL 7.x) */
        lv_task_handler();

        /* Sleep to avoid consuming too much CPU */
        usleep(5000); /* 5ms */
    }

    printf("Simulator exiting...\n");
    SDL_Quit();

    return 0;
}
