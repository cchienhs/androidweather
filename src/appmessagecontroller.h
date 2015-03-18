#pragma once

enum KEY { KEY_TEMPERATURE = 0, 
           KEY_TEMPERATURE_MAX = 1, 
           KEY_TEMPERATURE_MIN = 2,
           KEY_CONDITIONS = 3,
           KEY_TEMPERATURE_UNIT = 4,
           KEY_BLINKING_COLONS = 5,
           KEY_WEATHER_UPDATE_INTERVAL = 6,
		   KEY_RANDOM_QUOTE = 7,
		   KEY_RANDOM_QUOTE_AUTHOR = 8
         };

extern void appmessagecontroller_load( Window *window );
extern void appmessagecontroller_unload();
extern void appmessagecontroller_sendMessage( int key, int value );