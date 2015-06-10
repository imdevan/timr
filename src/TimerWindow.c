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

// To keep track of timer_running state
static bool timer_running;

// To keep track of time
static int s_time = 0;
static uint16_t timer_start_time;
static uint16_t interval_time;
static uint16_t final_warning_time;


// Bool to test if menu was opened (i.e. update start time)
static bool menu_was_opened;


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
	if(timer_running)
		stopTimer();
	
	menu_was_opened = true;
	
	switchWindow(MENU_WINDOW);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	
	if(timer_running == false){
  	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_pause);
		timer_running = (bool) true;
		// start the clock
	}else{
  	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_play);
		timer_running = (bool) false;
		// pause the clock
	}
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	// restart the clock
	s_time = timer_start_time;

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
// Stops timer and resets ui
void stopTimer(){
	
	s_time = timer_start_time;
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_play);
	timer_running = (bool) false;
	
	updateTextLayer();
}

// Set UI elements
void updateTextLayer(){
	
  // Use a long-lived buffer
  static char s_uptime_buffer[32];

  // Get time since launch
  int seconds = s_time % 60;
  int minutes = (s_time % 3600) / 60;
  int hours = s_time / 3600;

  // Update the TextLayer
  snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), "%dh %dm %ds", hours, minutes, seconds);
  text_layer_set_text(text_layer, s_uptime_buffer);
}


// Manage what happens while the timer is running
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	// Set UI elements
	updateTextLayer();
	
	if(menu_was_opened){
		menu_was_opened = false;
		timer_start_time = persist_exists(TIMER_START_TIME) ? persist_read_int(TIMER_START_TIME) : DEFAULT_TIMER_START_TIME;
		s_time = timer_start_time;
	}
  // Increment s_uptime
	if(timer_running)
		s_time--;
}

/*
	Load and Unload Definitions
	========================================================================================
*/

static void window_load(Window *window) {
	
	// Initiate window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	
	// Get persistant time variables
	timer_start_time = persist_exists(TIMER_START_TIME) ? persist_read_int(TIMER_START_TIME) : DEFAULT_TIMER_START_TIME;
	interval_time = persist_exists(INTERVAL_TIME) ? persist_read_int(INTERVAL_TIME) : DEFAULT_INTERVAL_TIME;
	final_warning_time = persist_exists(FINAL_WARNING_TIME) ? persist_read_int(FINAL_WARNING_TIME) : DEFAULT_FINAL_WARNING_TIME;
	
	// Set s_time to start time
	s_time = timer_start_time;
	
	// Create text layer and add it to the window layer
  text_layer = text_layer_create((GRect) { .origin = { -10, 72 }, .size = { window_bounds.size.w, 20 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
	
	updateTextLayer();
	
	
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
	
	// Set the state to not be running
	timer_running = (bool) false;
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
		
	
	menu_was_opened = false;
	 
  // Subscribe to TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

	