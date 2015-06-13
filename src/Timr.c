#include <pebble.h>
	
#include "Timr.h"
	
#include "TimerWindow.h"
#include "MenuWindow.h"
#include "SetTimeWindow.h"
	
int8_t curWindow = 0;

void switchWindow(uint8_t newWindow)
{
	switch(newWindow)
	{
	case TIMER_WINDOW:
		curWindow = TIMER_WINDOW;
		timer_window_init();
		break;
	case MENU_WINDOW:
		curWindow = MENU_WINDOW;
		menu_window_init();
		break;
	case SET_TIMER_START_WINDOW:
		curWindow = SET_TIME_WINDOW;
		set_time_window_init(SET_TIMER_START_WINDOW);
		break;
	case SET_TIMER_INTERVAL_WINDOW:
		curWindow = SET_TIME_WINDOW;
		set_time_window_init(SET_TIMER_INTERVAL_WINDOW);
		break;
	case SET_FINAL_WARNING_WINDOW:
		curWindow = SET_TIME_WINDOW;
		set_time_window_init(SET_FINAL_WARNING_WINDOW);
		break;
	}
}

int main(void) {
	
	switchWindow(0);

	app_event_loop();

	window_stack_pop_all(false);
	return 0;
}
