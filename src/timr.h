/*
 * timr.h
 *
 *  Created on: June 7, 2015
 *      Author: Devan Huapaya
 */

#ifndef TIMR_H_
#define TIMR_H_
	
void setCurWindow(uint8_t newWindow);
void switchWindow(uint8_t newWindow);
uint8_t getCurWindow(void);
const char* config_getFontResource(int id);
void closeApp(void);
bool canVibrate(void);

#endif /* TIMR_H_ */
