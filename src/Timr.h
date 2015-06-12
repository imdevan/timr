/*
 * timr.h
 *
 *  Created on: June 7th, 2015
 *      Author: Devan Huapaya
 */

#ifndef TIMR_H_
#define TIMR_H_
	
#define ANIMATED true
	
#define TIMER_WINDOW 0
#define MENU_WINDOW 1
#define SET_TIME_WINDOW 2
	
#define SET_VIB_INTERVAL 0
#define SET_FINAL_WARNING 1
	
	
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
