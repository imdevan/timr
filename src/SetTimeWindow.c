#include <pebble.h>
#include <pebble_fonts.h>

#include "Timr.h"
#include "SetTimeWindow.h"

/*
	Variables
	========================================================================================
*/
// The window
static Window *window;

// Where text will show up
static TextLayer *text_layer;


/*
	Button Callbacks
	========================================================================================
*/
static void button_back_single(ClickRecognizerRef recognizer, void* context)
{
	window_stack_pop(true);
}

static void registerButtons(void* context) {
	window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler) button_back_single);
}


/*
	Logic and Operations
	========================================================================================
*/

/*
		This is where you would put any logical operations or functions for the window.
*/

/*
	Load and Unload Definitions
	========================================================================================
*/
static void window_load(Window *window)
{

	// Initiate window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	
	// Set default text in text layer
  text_layer = text_layer_create((GRect) { .origin = { -10, 72 }, .size = { window_bounds.size.w, 20 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Set text in text layer
  text_layer_set_text(text_layer, "Second Window");
	
	// Add text layer to window
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window)
{
	text_layer_destroy(text_layer);
	window_destroy(window);
}


/*
	Window Initiation
	========================================================================================
*/
void set_time_window_init(void)
{
	window = window_create();

	window_set_window_handlers(window, (WindowHandlers) {
		.load = (WindowHandler) window_load,
		.unload = (WindowHandler) window_unload
	});


	window_set_click_config_provider(window, (ClickConfigProvider) registerButtons);
	window_stack_push(window, true);
}
