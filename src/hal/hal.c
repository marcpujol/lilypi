/**
 * @file hal.c
 * Hardware Abstraction Layer for SDL2
 */

#include "hal.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

/* Display driver */
static lv_disp_buf_t disp_buf;
static lv_color_t buf1[320 * 10];
static lv_disp_drv_t disp_drv;

/* Input device (mouse) */
static lv_indev_drv_t indev_drv;

/* SDL */
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static int32_t display_width;
static int32_t display_height;

/* Tick interface */
static uint32_t sdl_tick_get(void);

/**
 * SDL display flush callback
 */
static void sdl_display_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    int32_t x, y;
    
    /* Update texture with LVGL buffer */
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            SDL_Rect rect = {x, y, 1, 1};
            lv_color_t c = *color_p;
            SDL_SetRenderDrawColor(renderer, c.ch.red << 3, c.ch.green << 2, c.ch.blue << 3, 0xFF);
            SDL_RenderFillRect(renderer, &rect);
            color_p++;
        }
    }
    
    SDL_RenderPresent(renderer);
    lv_disp_flush_ready(disp_drv);
}

/**
 * SDL mouse read callback
 */
static bool sdl_mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    (void)indev_drv;
    
    int mouse_x, mouse_y;
    uint32_t mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    
    data->point.x = mouse_x;
    data->point.y = mouse_y;
    data->state = (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    
    return false;
}

/**
 * SDL tick interface
 */
static uint32_t sdl_tick_get(void)
{
    return SDL_GetTicks();
}

/**
 * Initialize SDL2 HAL for LVGL
 */
void sdl_hal_init(int32_t width, int32_t height)
{
    display_width = width;
    display_height = height;
    
    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return;
    }
    
    /* Create window */
    window = SDL_CreateWindow("LilyPi Simulator",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN);
    if(!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    
    /* Create renderer */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    
    /* Clear screen */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    /* Initialize LVGL display buffer */
    lv_disp_buf_init(&disp_buf, buf1, NULL, 320 * 10);
    
    /* Initialize display driver */
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = sdl_display_flush;
    lv_disp_drv_register(&disp_drv);
    
    /* Initialize input device driver (mouse) */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = sdl_mouse_read;
    lv_indev_drv_register(&indev_drv);
    
    /* Set up tick interface */
    lv_tick_set_cb(sdl_tick_get);
    
    printf("SDL HAL initialized: %dx%d\n", width, height);
}
