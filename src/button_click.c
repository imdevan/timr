#include <pebble.h>

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
static bool playing;

/* 
		Set click handlers for different buttons
*/
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
	
	if(playing == false){
  	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_pause);
		playing = (bool) true;
	}else{
  	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_play);
		playing = (bool) false;
	}
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


/* 
		Manage what happens when the window is loaded
*/
static void window_load(Window *window) {
	
	// Initiate window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	// Set default text in text layer
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
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


/* 
		Manage what happens when the window is left
*/
static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}


/* 
		Set up app variables
*/
static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
		
	
}

/* 
		Dealocate windows
*/
static void deinit(void) {
  window_destroy(window);
}


/* 
		Main function called when app is initiated
*/
int main(void) {
  init();
  app_event_loop();
  deinit();
}


