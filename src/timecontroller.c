#include <pebble.h>
#include "timecontroller.h"
#include "appmessagecontroller.h"
#include "eventloop.h"

static TextLayer *hourLayer = NULL;
static TextLayer *minuteLayer = NULL;
static TextLayer *flashingDotLayer = NULL;
static TextLayer *dayLayer = NULL;
static TextLayer *dateLayer = NULL;
static GFont timeFont;
static bool initiated = false;
static bool blinkingColons = true;

static void updateTime() 
{
  // Create a long-lived buffer
  static char hourBuffer[] = "00";
  static char minuteBuffer[] = "00";
  static char dayBuffer[10];
  static char dateBuffer[20];
  static bool dotToggle = true;

  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  if( !blinkingColons )     
    dotToggle = true;
  else
    dotToggle = !dotToggle;
  
  dotToggle?text_layer_set_text(flashingDotLayer, ":"):text_layer_set_text(flashingDotLayer, "");
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
      strftime(hourBuffer, sizeof("00"), "%H", tick_time);
  } 
  else {
      strftime(hourBuffer, sizeof("00"), "%I", tick_time);
  }

  text_layer_set_text(hourLayer, hourBuffer);
  
  strftime(minuteBuffer, sizeof("00"), "%M", tick_time);
  text_layer_set_text( minuteLayer, minuteBuffer );
  
  strftime( dayBuffer, sizeof(dayBuffer), "%A", tick_time );
  text_layer_set_text(dayLayer, dayBuffer);
  
  strftime( dateBuffer, sizeof(dateBuffer), "%h %d,%Y", tick_time );
  text_layer_set_text(dateLayer, dateBuffer);
}

static void createHourLayer( Window *window )
{
  hourLayer = text_layer_create( GRect(15, 0, 60, 45) );
  text_layer_set_background_color( hourLayer, GColorClear );
  text_layer_set_text_color( hourLayer, GColorBlack );
  text_layer_set_text( hourLayer, "00" );
  text_layer_set_font( hourLayer, timeFont );
  text_layer_set_text_alignment( hourLayer, GTextAlignmentCenter );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( hourLayer) );
}

static void createFlashingDotLayer( Window *window )
{
  flashingDotLayer = text_layer_create( GRect(65, 0, 22, 45) );
  text_layer_set_background_color( flashingDotLayer, GColorClear );
  text_layer_set_text_color( flashingDotLayer, GColorBlack );
  text_layer_set_text( flashingDotLayer, ":" );
  text_layer_set_font( flashingDotLayer, timeFont );
  text_layer_set_text_alignment( flashingDotLayer, GTextAlignmentCenter );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( flashingDotLayer) );
}

static void createMinuteLayer( Window *window )
{
  minuteLayer = text_layer_create( GRect( 77, 0, 60, 45) );
  text_layer_set_background_color( minuteLayer, GColorClear );
  text_layer_set_text_color( minuteLayer, GColorBlack );
  text_layer_set_text( minuteLayer, "00" );
  text_layer_set_font( minuteLayer, timeFont );
  text_layer_set_text_alignment( minuteLayer, GTextAlignmentCenter );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( minuteLayer) ); 
}

static void createDayLayer( Window *window )
{
  dayLayer = text_layer_create( GRect(0, 48, 144, 20) );
  text_layer_set_background_color( dayLayer, GColorWhite );
  text_layer_set_text_color( dayLayer, GColorBlack );
  text_layer_set_text( dayLayer, "Saturday" );
  text_layer_set_font( dayLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD) );
  text_layer_set_text_alignment( dayLayer, GTextAlignmentCenter );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( dayLayer ) );
}

static void createDateLayer( Window *window )
{
  dateLayer = text_layer_create( GRect(0, 65, 144, 20) );
  text_layer_set_background_color( dateLayer, GColorWhite );
  text_layer_set_text_color( dateLayer, GColorBlack );
  text_layer_set_text( dateLayer, "Saturday" );
  text_layer_set_font( dateLayer, fonts_get_system_font(FONT_KEY_GOTHIC_18) );
  text_layer_set_text_alignment( dateLayer, GTextAlignmentCenter );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( dateLayer ) );
}

void timecontroller_setBlinkingColons( bool value )
{
  persist_write_bool( KEY_BLINKING_COLONS, value );
  blinkingColons = value;  
}

bool timecontroller_getBlinkingColons()
{
  return blinkingColons;  
}

void timecontroller_load( Window *window )
{
  if( initiated == 0 ) {
    initiated = 1;
    timeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SANSATION_REGULAR_42));
    createHourLayer( window );
    createFlashingDotLayer( window );
    createMinuteLayer( window );
    createDayLayer( window );
    createDateLayer( window );
    
    if( persist_exists( KEY_BLINKING_COLONS ) ) {
      timecontroller_setBlinkingColons( persist_read_bool( KEY_BLINKING_COLONS) );
    }
    updateTime();
    
    eventloop_subscribe( &updateTime );
  }
}

void timecontroller_unload()
{
  eventloop_unsubscribe( &updateTime );
  text_layer_destroy( dayLayer );
  text_layer_destroy( dateLayer );
  text_layer_destroy( hourLayer );
  text_layer_destroy( minuteLayer );
  text_layer_destroy( flashingDotLayer );
}