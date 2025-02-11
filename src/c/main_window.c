#include <pebble.h>
#include "main_window.h"

static Window *s_window;
// static Layer *canvas;
// static TextLayer *hour_layer;
// static TextLayer *minute_layer;

static GBitmap *fox_bitmap = NULL;
static BitmapLayer *bitmap_layer;
static GBitmapSequence *fox_sequence = NULL;

static void load_sequence();

// static GBitmap * idle_bitmap;
// static BitmapLayer *idle_bitmap_layer;

// static const uint32_t RESOURCES [4] = {RESOURCE_ID_FOX_IDLE, RESOURCE_ID_SHEEP_IDLE, RESOURCE_ID_WOLF_IDLE, RESOURCE_ID_BUNNY_IDLE};

// // currently experimenting
// static uint32_t current_animal = 0;
// static uint32_t starttime = 0;
// static uint32_t currentsec = 0;
// static uint32_t startdelay = 10; //sec
// static bool finishedstart = false;

// // Draw the background elements onto the canvas layer.
// static void canvas_update_proc(Layer *layer, GContext *ctx) {
// }

// static void update_time() {
//     // Get a tm structure.
//     time_t temp = time(NULL);
//     struct tm *tick_time = localtime(&temp);
//     starttime = tick_time->tm_sec;

//     // format the time from the tm structure and stick it into a buffer
//     static char hour_buffer[4];
//     strftime(hour_buffer, sizeof(hour_buffer), clock_is_24h_style() ? "%H" : "%I", tick_time);

//     static char minute_buffer[4];
//     strftime(minute_buffer, sizeof(minute_buffer), clock_is_24h_style() ? "%M" : "%M", tick_time);

//     // Change the text layer to reflect the current time.
//     text_layer_set_text(hour_layer, hour_buffer);
//     text_layer_set_text(minute_layer, minute_buffer);
// }

// static void update_animal() {
//     if (idle_bitmap && (finishedstart || (currentsec - starttime > 30))) {
//         gbitmap_destroy(idle_bitmap);
//         idle_bitmap = NULL;
//         finishedstart = true;
//         //current_animal = current_animal % 4; 
//     idle_bitmap = gbitmap_create_with_resource(RESOURCES[RESOURCE_ID_BUNNY_IDLE]);
//     bitmap_layer_set_bitmap(idle_bitmap_layer, idle_bitmap);
//     }
    
    
// }

// static void tick_handler(struct tm *tick_time, TimeUnits unit_changed) {
//     update_time();
//     // Do other things.
//     // run animation.
//     //update_animal();
// }


static void timer_handler(void *context) {
  uint32_t next_delay;

  // Advance to the next APNG frame
  if(gbitmap_sequence_update_bitmap_next_frame(fox_sequence, fox_bitmap, &next_delay)) {
    bitmap_layer_set_bitmap(bitmap_layer, fox_bitmap);
    layer_mark_dirty(bitmap_layer_get_layer(bitmap_layer));

    // Timer for that delay
    app_timer_register(next_delay, timer_handler, NULL);
  } else {
    // Start again
    load_sequence();
  }
}

static void load_sequence() {
  // Free old data
  if(fox_sequence) {
    gbitmap_sequence_destroy(fox_sequence);
    fox_sequence = NULL;
  }
  if(fox_bitmap) {
    gbitmap_destroy(fox_bitmap);
    fox_bitmap = NULL;
  }

  // Create sequence
  // fox_sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_WOLF);
  fox_sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_WOLF_SMALL);

  // Create GBitmap
  fox_bitmap = gbitmap_create_blank(gbitmap_sequence_get_bitmap_size(fox_sequence), GBitmapFormat8Bit);

  // Begin animation
  app_timer_register(1, timer_handler, NULL);
}

static void window_load(Window *window) {
    // window info.
    Layer *window_layer = window_get_root_layer(window);
    GRect window_bounds = layer_get_bounds(window_layer); // This canvas will cover the whole window

    // Drawing layer init.
    // canvas = layer_create(window_bounds);
    // layer_set_update_proc(canvas, canvas_update_proc);
    // layer_add_child(window_layer, canvas);

    bitmap_layer = bitmap_layer_create(window_bounds);
    layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));

    load_sequence();
    // idle_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FOX_IDLE);
    // GRect idle_bounds = layer_get_bounds(window_layer);
    // idle_bitmap_layer = bitmap_layer_create(idle_bounds);
    // bitmap_layer_set_compositing_mode(idle_bitmap_layer, GCompOpSet);
    // bitmap_layer_set_bitmap(idle_bitmap_layer, idle_bitmap);
    // layer_add_child(window_layer, bitmap_layer_get_layer(idle_bitmap_layer));

    // hour_layer = text_layer_create(GRect(0, 20, window_bounds.size.w/2, 50));
    // text_layer_set_background_color(hour_layer, GColorClear);
    // text_layer_set_text_color(hour_layer, GColorSunsetOrange);
    // text_layer_set_text(hour_layer, "HH");
    // text_layer_set_font(hour_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    // text_layer_set_text_alignment(hour_layer, GTextAlignmentCenter);
    // layer_add_child(window_layer, text_layer_get_layer(hour_layer));

    // minute_layer = text_layer_create(GRect(35, 95, window_bounds.size.w, 50));
    // text_layer_set_background_color(minute_layer, GColorClear);
    // text_layer_set_text_color(minute_layer, GColorVividCerulean);
    // text_layer_set_text(minute_layer, "MM");
    // text_layer_set_font(minute_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    // text_layer_set_text_alignment(minute_layer, GTextAlignmentCenter);
    // layer_add_child(window_layer, text_layer_get_layer(minute_layer));
}

static void window_unload(Window *window) {
    //text_layer_destroy(whateverLayer);
    // text_layer_destroy(hour_layer);
    // text_layer_destroy(minute_layer);
}

// Ran in the app's init function.
void main_window_create() {
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers){
        .load = window_load,
        .unload = window_unload,
    });

    window_set_background_color(s_window, GColorBlack);
    const bool animated = true;
    window_stack_push(s_window, animated);

    // Get the time every minute.
    // Can be placed before the window is created too. I think this first update_time() call could be placed in window_load() if you do.
    // tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    // update_time();
}

// Ran in deinit.
void main_window_destroy() {
    //destroy stuff. Not sure if destroying the window must be done last.
    // gbitmap_sequence_destroy(fox_sequence);
    // gbitmap_destroy(fox_bitmap);
    // bitmap_layer_destroy(bitmap_layer);
    // Free old data
    if(fox_sequence) {
        gbitmap_sequence_destroy(fox_sequence);
        fox_sequence = NULL;
    }
    if(fox_bitmap) {
        gbitmap_destroy(fox_bitmap);
        fox_bitmap = NULL;
    }

        // gbitmap_destroy(idle_bitmap);
        // bitmap_layer_destroy(idle_bitmap_layer);

    window_destroy(s_window);
}