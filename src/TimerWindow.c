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
static TextLayer *minute_text_layer;
static TextLayer *second_text_layer;

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
static uint16_t check_time;

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
	if(timer_running){
		stopTimer();
	}
	
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
  static char s_minute_buffer[32];
  static char s_second_buffer[32];

  // Get time since launch
  int seconds = s_time % 60;
  int minutes = (s_time % 3600) / 60;

  // Update the TextLayer
  snprintf(s_minute_buffer, sizeof(s_minute_buffer), "%d", minutes);
  text_layer_set_text(minute_text_layer, s_minute_buffer);
	
  snprintf(s_second_buffer, sizeof(s_second_buffer), "%d", seconds);
  text_layer_set_text(second_text_layer, s_second_buffer);
}


// Manage what happens while the timer is running
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	
	// Check if start time has changed
	check_time = persist_exists(TIMER_START_TIME) ? persist_read_int(TIMER_START_TIME) : DEFAULT_TIMER_START_TIME;
	if(check_time != timer_start_time){
		timer_start_time = check_time;
		s_time = timer_start_time;
	}
	
	// Check if interval time has changed
	check_time = persist_exists(INTERVAL_TIME) ? persist_read_int(INTERVAL_TIME) : DEFAULT_INTERVAL_TIME;
	if(check_time != interval_time){
		interval_time = check_time;
		s_time = timer_start_time;
	}
	
	// Check if final warning time has changed
	check_time = persist_exists(FINAL_WARNING_TIME) ? persist_read_int(FINAL_WARNING_TIME) : DEFAULT_FINAL_WARNING_TIME;
	if(check_time != final_warning_time){
		final_warning_time = check_time;
		s_time = timer_start_time;
	}
	
	if(timer_running){

  	// Decrease the counter 
		s_time--;
		
		// Vibrate if on interval time
		if(s_time % interval_time == 0)
			vibes_short_pulse();
		
		if(s_time <= 0)
			stopTimer();
	}
	
	// Set UI elements
	updateTextLayer();
	
	// If the menu was opened
	if(menu_was_opened){
		
		// Reset the start time and counter
		menu_was_opened = false;
		timer_start_time = persist_exists(TIMER_START_TIME) ? persist_read_int(TIMER_START_TIME) : DEFAULT_TIMER_START_TIME;
		s_time = timer_start_time;
	}
}

/*
	Load and Unload Definitions
	========================================================================================
*/
static void initTextLayer(){
	
	
	// Initiate window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	
	// Create minute text layer and add it to the window layer
  minute_text_layer = text_layer_create((GRect) { .origin = { -10, 10 }, .size = { window_bounds.size.w - 30, window_bounds.size.h/2 } });
  text_layer_set_text_alignment(minute_text_layer, GTextAlignmentRight);
	text_layer_set_font(minute_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(minute_text_layer));
	
	// Create second text layer and add it to the window layer
  second_text_layer = text_layer_create((GRect) { .origin = { -10, window_bounds.size.h/2 }, .size = { window_bounds.size.w - 30, window_bounds.size.h/2 } });
  text_layer_set_text_alignment(second_text_layer, GTextAlignmentRight);
	text_layer_set_font(second_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  layer_add_child(window_layer, text_layer_get_layer(second_text_layer));
	
	
}
static void window_load(Window *window) {
	
	// Get persistant time variables
	timer_start_time = persist_exists(TIMER_START_TIME) ? persist_read_int(TIMER_START_TIME) : DEFAULT_TIMER_START_TIME;
	interval_time = persist_exists(INTERVAL_TIME) ? persist_read_int(INTERVAL_TIME) : DEFAULT_INTERVAL_TIME;
	final_warning_time = persist_exists(FINAL_WARNING_TIME) ? persist_read_int(FINAL_WARNING_TIME) : DEFAULT_FINAL_WARNING_TIME;
	
	// Set s_time to start time
	s_time = timer_start_time;
	
	// Initialize text layer
	initTextLayer();
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

	