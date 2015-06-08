#include <pebble.h>
#include <pebble_fonts.h>

#include "Timr.h"
#include "TimerWindow.h"



/*
	Variables
	========================================================================================
*/
	
// The window
static Window *window;

// Where text will show up
static TextLayer *text_layer;

// The action bar
ActionBarLayer *action_bar;
static GBitmap *my_icon_play;
static GBitmap *my_icon_pause;
static GBitmap *my_icon_settings;
static GBitmap *my_icon_restart;

// To keep track of playing state
static bool playing;

// To keep track of time
static int s_time = 0;



/*
	Button Callbacks
	========================================================================================
*/
static void button_back_single(ClickRecognizerRef recognizer, void* context)
{
	window_stack_pop(ANIMATED);
}

static void center_click_handler(ClickRecognizerRef recognizer, void* context)
{
	switchWindow(MENU_WINDOW);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if(playing == false){
  	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_pause);
		playing = (bool) true;
		// start the clock
	}else{
  	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_play);
		playing = (bool) false;
		// pause the clock
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	// restart the clock
	s_time = 0;

}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, center_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, button_back_single);
}


/*
	Logic and Operations
	========================================================================================
*/

// Manage what happens while the timer is running
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Use a long-lived buffer
  static char s_uptime_buffer[32];

  // Get time since launch
  int seconds = s_time % 60;
  int minutes = (s_time % 3600) / 60;
  int hours = s_time / 3600;

  // Update the TextLayer
  snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), "Uptime: %dh %dm %ds", hours, minutes, seconds);
  text_layer_set_text(text_layer, s_uptime_buffer);

  // Increment s_uptime
	if(playing)
		s_time++;
}

/*
	Load and Unload Definitions
	========================================================================================
*/

static void window_load(Window *window) {
	
	// Initiate window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	
	// Set default text in text layer
  text_layer = text_layer_create((GRect) { .origin = { -10, 72 }, .size = { window_bounds.size.w, 20 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_text(text_layer, "Uptime: 0h 0m 0s");
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
	
	
	// Initialize the action bar:
  action_bar = action_bar_layer_create();
  // Associate the action bar with the window:
  action_bar_layer_add_to_window(action_bar, window);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(action_bar,
                                             click_config_provider);

	// Load the icons
	my_icon_play = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAY2);
	my_icon_pause = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PAUSE2);
	my_icon_settings = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SETTINGS2);
	my_icon_restart = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RESTART2);
	
  // Set the icons:
  // The loading of the icons is omitted for brevity... See gbitmap_create_with_resource()
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_play);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, my_icon_settings);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, my_icon_restart);
	
	// Set the state to not be playing
	playing = (bool) false;
}

static void window_unload(Window *window)
{
	text_layer_destroy(text_layer);
	window_destroy(window);
	window_stack_pop_all(false);
	// closeApp();
}


/*
	Window Initiation
	========================================================================================
*/
void timer_window_init(void)
{
	// Create window and add loading callback
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
    .unload = window_unload,
  });
		
  window_stack_push(window, ANIMATED);
		
	
  // Subscribe to TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

	