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
	
void setCurWindow(uint8_t newWindow);
void switchWindow(uint8_t newWindow);
uint8_t getCurWindow(void);
const char* config_getFontResource(int id);
void closeApp(void);
bool canVibrate(void);

#endif /* TIMR_H_ */
