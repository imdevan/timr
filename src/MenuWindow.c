#include <pebble.h>
	
#include "Timr.h"
#include "MenuWindow.h"
	
/*
	Definitions
	========================================================================================
*/
#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 3
	

/*
	Variables
	========================================================================================
*/
// Window
static Window *s_main_window;
static Layer *window_layer;

// Menu Layyer
static MenuLayer *s_menu_layer;

/*
	Button Callbacks
	========================================================================================
*/


/*
	UI Setup
	========================================================================================
*/
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return NUM_MENU_ITEMS;
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
		
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Set Time", "Timer length", NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, "Set Interval", "Vibrate interval", NULL);
          break;
        case 2: 
          menu_cell_basic_draw(ctx, cell_layer, "Final Warning", "Vibrate twice at time", NULL);
          break;
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	
  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 0:
			switchWindow(SET_TIMER_START_WINDOW);
      break;
    case 1:
			switchWindow(SET_TIMER_INTERVAL_WINDOW);
      break;
    case 3:
			switchWindow(SET_FINAL_WARNING_WINDOW);
      break;
  }

}


/*
	Load and Unload Definitions
	========================================================================================
*/
static void main_window_load(Window *window) {
	
  // Now we prepare to initialize the menu layer
  window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
	
  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
}


/*
	Window Initiation
	========================================================================================
*/
void menu_window_init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

void deinit() {
  window_destroy(s_main_window);
}

