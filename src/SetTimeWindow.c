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
static TextLayer *minute_text_wrapper;
static TextLayer *second_text_wrapper;
static TextLayer *minute_text_layer;
static TextLayer *second_text_layer;



// The action bar
ActionBarLayer *action_bar;
static GBitmap *my_icon_plus;
static GBitmap *my_icon_minus;
static GBitmap *my_icon_settings;

// To timer set time
static uint16_t timer_set_time = 0;

// Time to be set
static uint8_t s_time_to_be_set;

// To track which variable is being edited
static bool field_to_edit;

/*
	Button Callbacks
	========================================================================================
*/
static void button_back_single(ClickRecognizerRef recognizer, void* context)
{
	
  persist_write_int(TIMER_START_TIME, timer_set_time);
	window_stack_pop(ANIMATED);
}

static void setTextLayerTheme(TextLayer* layer, uint8_t theme){
	if(theme == DARK){
		text_layer_set_background_color(layer, GColorBlack);
		text_layer_set_text_color(layer, GColorWhite);
	}
	else if(theme == LIGHT){
		text_layer_set_background_color(layer, GColorWhite);
		text_layer_set_text_color(layer, GColorBlack);
	}
}

static void center_click_handler(ClickRecognizerRef recognizer, void* context)
{
	
	
	// Change field to edit to seconds
	if(field_to_edit == MINUTES){
		field_to_edit = SECONDS;
		
		// Set minutes layer light
		setTextLayerTheme(minute_text_wrapper, LIGHT);
		setTextLayerTheme(minute_text_layer, LIGHT);
		
		// Set seconds layer dark
		setTextLayerTheme(second_text_wrapper, DARK);
		setTextLayerTheme(second_text_layer, DARK);
		
	}else{
		field_to_edit = MINUTES;
		
		// Set minutes layer dark
		setTextLayerTheme(minute_text_wrapper, DARK);
		setTextLayerTheme(minute_text_layer, DARK);
		
		// Set seconds layer light
		setTextLayerTheme(second_text_wrapper, LIGHT);
		setTextLayerTheme(second_text_layer, LIGHT);
		
		
	}
	
	
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	
	timer_set_time += (field_to_edit == SECONDS) ? 1 : 60;
	updateSetTextLayer();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	
	timer_set_time -= (field_to_edit == SECONDS) ? 1 : 60;
	updateSetTextLayer();

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

// Set UI elements
void updateSetTextLayer(){
	
  // Use a long-lived buffer
  static char s_minute_buffer[32];
  static char s_second_buffer[32];

  // Get time since launch
  int seconds = timer_set_time % 60;
  int minutes = (timer_set_time % 3600) / 60;

  // Update the TextLayer
  snprintf(s_minute_buffer, sizeof(s_minute_buffer), "%d", minutes);
  text_layer_set_text(minute_text_layer, s_minute_buffer);
	
  snprintf(s_second_buffer, sizeof(s_second_buffer), "%d", seconds);
  text_layer_set_text(second_text_layer, s_second_buffer);
}




/*
	Load and Unload Definitions
	========================================================================================
*/
	
static void initSetTextLayer(){
	
	
	// Initiate window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	field_to_edit = MINUTES;
	
	// Create minute text layer and add it to the window layer
	// ====================================
	
	// wrapper for minutes
  minute_text_wrapper = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { window_bounds.size.w, window_bounds.size.h/2 } });
  text_layer_set_text_alignment(minute_text_wrapper, GTextAlignmentRight);
	setTextLayerTheme(minute_text_wrapper, DARK);
	//text_layer_set_background_color(minute_text_wrapper, GColorBlack);
	//text_layer_set_text_color(minute_text_wrapper, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(minute_text_wrapper));
	
	// Actuall minutes text
  minute_text_layer = text_layer_create((GRect) { .origin = { -10, 10 }, .size = { window_bounds.size.w - 30, window_bounds.size.h/2 } });
  text_layer_set_text_alignment(minute_text_layer, GTextAlignmentRight);
	setTextLayerTheme(minute_text_layer, DARK);
	//text_layer_set_background_color(minute_text_layer, GColorBlack);
	//text_layer_set_text_color(minute_text_layer, GColorWhite);
	text_layer_set_font(minute_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(minute_text_layer));
	
	
	// wrapper for seconds
  second_text_wrapper = text_layer_create((GRect) { .origin = { 0, window_bounds.size.h/2  }, .size = { window_bounds.size.w, window_bounds.size.h/2 } });
  text_layer_set_text_alignment(second_text_wrapper, GTextAlignmentRight);
	setTextLayerTheme(second_text_wrapper, LIGHT);
  //text_layer_set_background_color(second_text_wrapper, GColorWhite);
	//text_layer_set_text_color(second_text_wrapper, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(second_text_wrapper));
	
	// Create second text layer and add it to the window layer
  second_text_layer = text_layer_create((GRect) { .origin = { -10, window_bounds.size.h/2 }, .size = { window_bounds.size.w - 30, window_bounds.size.h/2 } });
  text_layer_set_text_alignment(second_text_layer, GTextAlignmentRight);
	setTextLayerTheme(second_text_layer, LIGHT);
	//text_layer_set_background_color(second_text_layer, GColorWhite);
	//text_layer_set_text_color(second_text_layer, GColorBlack);
	text_layer_set_font(second_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  layer_add_child(window_layer, text_layer_get_layer(second_text_layer));
	
	
}
	
static void window_load(Window *window)
{

	
	// Set value of time
	timer_set_time = persist_exists(TIMER_START_TIME) ? persist_read_int(TIMER_START_TIME) : DEFAULT_TIMER_START_TIME;
	
	// Set text in text layer
	initSetTextLayer();
	updateSetTextLayer();
	
	// Set the action bar
	// ====================================
	
	// Initialize the action bar:
  action_bar = action_bar_layer_create();
  // Associate the action bar with the window:
  action_bar_layer_add_to_window(action_bar, window);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(action_bar,
                                             click_config_provider);

	// Load the icons
	my_icon_plus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLUS2);
	my_icon_minus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MINUS2);
	my_icon_settings = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SETTINGS2);
	
  // Set the icons:
  // The loading of the icons is omitted for brevity... See gbitmap_create_with_resource()
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_plus);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, my_icon_settings);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, my_icon_minus);
	
}

static void window_unload(Window *window)
{
	/*
	Do this for all layers:
	text_layer_destroy(text_layer);
	*/
	text_layer_destroy(minute_text_wrapper);
	text_layer_destroy(minute_text_layer);
	text_layer_destroy(second_text_wrapper);
	text_layer_destroy(second_text_layer);
	window_destroy(window);
}


/*
	Window Initiation
	========================================================================================
*/
void set_time_window_init(int time_to_be_set)
{
	s_time_to_be_set = time_to_be_set;
	
	window = window_create();

	window_set_window_handlers(window, (WindowHandlers) {
		.load = (WindowHandler) window_load,
		.unload = (WindowHandler) window_unload
	});


	// window_set_click_config_provider(window, (ClickConfigProvider) registerButtons);
	window_stack_push(window, true);
}

