#pragma once

extern void weathercontroller_load( Window *window );
extern void weathercontroller_unload();

extern void weathercontroller_setTemperatureUnit( char *value );
extern char* weathercontroller_getTemperatureUnit();

extern void weathercontroller_setWeatherUpdateInterval( int value );
extern int weathercontroller_getWeatherUpdateInterval();

extern void weathercontroller_updateTemperature( int value );
extern void weathercontroller_updateMinTemperature( int value );
extern void weathercontroller_updateMaxTemperature( int value );
extern void weathercontroller_updateCondition( char* value );