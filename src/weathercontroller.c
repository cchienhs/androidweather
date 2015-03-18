#include <pebble.h>
#include "appmessagecontroller.h"
#include "weathercontroller.h"
#include "eventloop.h"
  
static TextLayer *temperatureLayer = NULL;
static TextLayer *maxTemperatureLayer = NULL;
static TextLayer *minTemperatureLayer = NULL;
static BitmapLayer *iconLayer = NULL; 
static GBitmap *iconBitmap = NULL;

static int temperatureUnit = 0; //Celcius=0; Fahrenheit=1;
static int weatherUpdateInterval = 3600; //default to one hour

/**
  @fn static int celsius( int value )
  @brief local helper function which converts kelvin to celsius
  @param value : degree in kelvin
  @return integer value in celsius 
*/
static int celsius( int value )
{
  return(value - 273.15);
}

/**
  @fn static int fahrenheitC( int value )
  @brief local helper function which converts kelvin to fahrenheit
  @param value: degree in kevin
  @return integer value in fahrenheit
*/
static int fahrenheit( int value )
{
  return (value - 273.15) * 9/5 + 32;
}

/**  
  @fn static void updateWeather()
  @brief Local function which sends request to pebble js for obtaining of weather information.  
  The function will trigger an update via appmessagecontroller depending on the current weatherUpdateInterval
*/

static void updateWeather()
{ 
  static int tick = 0;
  if( ((++tick)%weatherUpdateInterval) == 0 ) {
    appmessagecontroller_sendMessage( KEY_TEMPERATURE, 1 );
  }
}

/**  
  @fn static void updateWeatherNow()
  @brief Local function which sends request to pebble js for obtaining of weather information.  
  Similar to the above updateWeather function, just that this carries out the query instantly instead of waiting for thw required interval
*/
static void updateWeatherNow()
{
  appmessagecontroller_sendMessage( KEY_TEMPERATURE, 1 );
}

static void createMaxTemperatureLayer( Window *window )
{
  maxTemperatureLayer = text_layer_create( GRect(110, 140, 35, 20) );
  text_layer_set_background_color( maxTemperatureLayer, GColorBlack );
  text_layer_set_text_color( maxTemperatureLayer, GColorWhite );
  text_layer_set_font( maxTemperatureLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD) );
  text_layer_set_text_alignment( maxTemperatureLayer, GTextAlignmentCenter );
  text_layer_set_text( maxTemperatureLayer, "NA" );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( maxTemperatureLayer) );
}

static void createMinTemperatureLayer( Window *window )
{
  minTemperatureLayer = text_layer_create( GRect(65, 140, 35, 20) );
  text_layer_set_background_color( minTemperatureLayer, GColorBlack );
  text_layer_set_text_color( minTemperatureLayer, GColorWhite );
  text_layer_set_font( minTemperatureLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD) );
  text_layer_set_text_alignment( minTemperatureLayer, GTextAlignmentCenter );
  text_layer_set_text( minTemperatureLayer, "NA");
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( minTemperatureLayer) );
}

static void createTemperatureLayer( Window *window )
{
  temperatureLayer = text_layer_create( GRect(55, 105, 55, 40) );
  text_layer_set_background_color( temperatureLayer, GColorBlack );
  text_layer_set_text_color( temperatureLayer, GColorWhite );
  text_layer_set_font( temperatureLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD) );
  text_layer_set_text_alignment( temperatureLayer, GTextAlignmentCenter );
  text_layer_set_text( temperatureLayer, "NA");
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( temperatureLayer) );
}

static void createIconLayer( Window *window )
{
  iconLayer = bitmap_layer_create( GRect(0, 110, 50, 50) );
  layer_add_child( window_get_root_layer(window), bitmap_layer_get_layer( iconLayer ));
}

char* weathercontroller_getTemperatureUnit()
{
  if( temperatureUnit==0 )
    return "Celcius";
  else
    return "Fahrenheit";
}

void weathercontroller_setTemperatureUnit( char *value )
{
  if( strcmp( value, "Celsius") == 0 )
    temperatureUnit = 0;
  else
    temperatureUnit = 1;
  updateWeatherNow();
}

int weathercontroller_getWeatherUpdateInterval()
{
  return weatherUpdateInterval;
}

void weathercontroller_setWeatherUpdateInterval( int value ) {
  weatherUpdateInterval = value;
  updateWeatherNow();
}

void weathercontroller_updateCondition( char *value )
{
  gbitmap_destroy( iconBitmap );
  iconBitmap = NULL;
  if( strcmp( value, "01n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_01n );
  else if( strcmp( value, "01d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_01d );
  else if( strcmp( value, "02n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_02n );
  else if( strcmp( value, "02d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_02d );
  else if( strcmp( value, "03n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_03d );
  else if( strcmp( value, "03d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_03d );
  else if( strcmp( value, "04n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_04d );
  else if( strcmp( value, "04d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_04d );
  else if( strcmp( value, "09n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_09d );
  else if( strcmp( value, "09d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_09d );
  else if( strcmp( value, "10d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_10d );
  else if( strcmp( value, "10n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_10d );
  else if( strcmp( value, "11d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_11d );
  else if( strcmp( value, "11n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_11d );
  else if( strcmp( value, "13d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_13d );
  else if( strcmp( value, "13n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_13d );
  else if( strcmp( value, "50d") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_50d );
  else if( strcmp( value, "50n") == 0 )
    iconBitmap = gbitmap_create_with_resource( RESOURCE_ID_50d );
  
  if( iconBitmap != NULL )
    bitmap_layer_set_bitmap( iconLayer, iconBitmap );
}
 
void weathercontroller_updateMinTemperature( int value )
{
  static char minTemperatureBuffer[8];
  if( temperatureUnit == 0 )
    snprintf(minTemperatureBuffer, sizeof(minTemperatureBuffer), "%dC", celsius(value) );
  else
    snprintf(minTemperatureBuffer, sizeof(minTemperatureBuffer), "%dF", fahrenheit(value) );

  text_layer_set_text( minTemperatureLayer, minTemperatureBuffer);
} 

void weathercontroller_updateMaxTemperature( int value )
{
  static char maxTemperatureBuffer[8];
  if( temperatureUnit == 0 )    
    snprintf(maxTemperatureBuffer, sizeof(maxTemperatureBuffer), "%dC", celsius(value) );
  else
    snprintf(maxTemperatureBuffer, sizeof(maxTemperatureBuffer), "%dF", fahrenheit(value) );

  text_layer_set_text( maxTemperatureLayer, maxTemperatureBuffer);
}
  
     
void weathercontroller_updateTemperature( int value )
{
  static char temperatureBuffer[8];  
  if( temperatureUnit == 0 )
    snprintf(temperatureBuffer, sizeof(temperatureBuffer), "%dC", celsius(value) );
  else
    snprintf(temperatureBuffer, sizeof(temperatureBuffer), "%dF", fahrenheit(value) );
  
  text_layer_set_text( temperatureLayer, temperatureBuffer);
}

void weathercontroller_load( Window *window )
{
  static bool initiated = false;
  
  if( initiated == false ) {
    initiated = true;
 
    createTemperatureLayer( window );
    createMaxTemperatureLayer( window );
    createMinTemperatureLayer( window );
    createIconLayer( window );
    
    eventloop_subscribe( &updateWeather );
    
  }
}

void weathercontroller_unload()
{
  eventloop_unsubscribe( &updateWeather );
  text_layer_destroy( temperatureLayer );
  text_layer_destroy( maxTemperatureLayer );
  text_layer_destroy( minTemperatureLayer );
  gbitmap_destroy( iconBitmap );
  bitmap_layer_destroy( iconLayer );
}