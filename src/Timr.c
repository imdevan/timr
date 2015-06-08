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
	case 0:
		curWindow = TIMER_WINDOW;
		timer_window_init();
		break;
	case 1:
		curWindow = MENU_WINDOW;
		menu_window_init();
		break;
	case 2:
		curWindow = SET_TIME_WINDOW;
		set_time_window_init();
		break;
	}
}

int main(void) {
	
	switchWindow(0);

	app_event_loop();

	window_stack_pop_all(false);
	return 0;
}
