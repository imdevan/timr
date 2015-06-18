/*
 * timr.h
 *
 *  Created on: June 7th, 2015
 *      Author: Devan Huapaya
 */

#ifndef TIMR_H_
#define TIMR_H_
	
#define ANIMATED true
	
// Used to navigate to different windows 
#define TIMER_WINDOW 0
#define MENU_WINDOW 1
#define SET_TIME_WINDOW 2

// To indicate which set time window the user is on#
#define SET_TIMER_START_WINDOW 3
#define SET_TIMER_INTERVAL_WINDOW 4
#define SET_FINAL_WARNING_WINDOW 5
	
// Used in setting time windows
#define MINUTES true
#define SECONDS false
#define DARK 0
#define LIGHT 1
	
	
// Timer settings
// ================================
	
// Timer
#define TIMER_START_TIME 600
#define DEFAULT_TIMER_START_TIME 600

// Interval time
#define INTERVAL_TIME 5
#define DEFAULT_INTERVAL_TIME 5
	
// Final warning
#define FINAL_WARNING_TIME 60
#define DEFAULT_FINAL_WARNING_TIME 60
	
	
void setCurWindow(uint8_t newWindow);
void switchWindow(uint8_t newWindow);
uint8_t getCurWindow(void);
const char* config_getFontResource(int id);
void closeApp(void);
bool canVibrate(void);

#endif /* TIMR_H_ */
