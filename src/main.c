#include <pebble.h>

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 3
	
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

// The menu
static MenuLayer *menu_layer;
static Layer *window_layer;

// To keep track of playing state
static bool playing;

// To keep track of time
static int s_time = 0;


/*
		Menu set callbbacks
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
          menu_cell_basic_draw(ctx, cell_layer, "Set Time", "Duration of timer", NULL);
          // menu_cell_title_draw(ctx, cell_layer, "Final Item");
          break;
        case 1:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, "Set Interval", "Of vibrations", NULL);
          break;
        case 2: 
          menu_cell_basic_draw(ctx, cell_layer, "Final Warning", "Start double vibrate when this close to the end", NULL);
          break;
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	
  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 1:
      // Cycle the icon
      // s_current_icon = (s_current_icon + 1) % NUM_MENU_ICONS;
      // After changing the icon, mark the layer to have it updated
      // layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
  }
}

/* 
		Set click handlers for different buttons
*/
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// go to menu
  // Create the menu layer
	
  window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
	
  menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));

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
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

/*
		Manage what happens while the timer is running
*/
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
		Manage what happens when the window is loaded
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
	
	// Create window and add loading callback
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
		
	
  // Subscribe to TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

/* 
		Dealocate window
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


