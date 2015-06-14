/*
 * SetTimeWindow.h
 *
 *  Created on: June 8th, 2015
 *      Author: Devan Huapaya
 */

#ifndef SECONDWINDOW_H_
#define SECONDWINDOW_H_

#define MINUTES true
#define SECONDS false
#define DARK 0
#define LIGHT 1

void set_time_window_init(int time_to_be_set);
void updateSetTextLayer(void);
static void setTextLayerTheme(TextLayer *layer, uint8_t theme);

#endif /* FIRSTWINDOW_H_ */

