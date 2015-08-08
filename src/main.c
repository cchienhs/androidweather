#include <pebble.h>
#include "timecontroller.h"
#include "batterycontroller.h"
#include "weathercontroller.h"
#include "bluetoothcontroller.h"
#include "appmessagecontroller.h"
#include "eventloop.h"
  
static Window *mainWindow;
static BitmapLayer *backgroundLayer;
static GBitmap *backgroundBitmap;

static void mainWindowLoad( Window *window ) 
{
  backgroundBitmap = gbitmap_create_with_resource( RESOURCE_ID_BACKGROUND_WEATHER );
  backgroundLayer = bitmap_layer_create( GRect(0, 0, 144, 168) );
  bitmap_layer_set_bitmap( backgroundLayer, backgroundBitmap );
  layer_add_child( window_get_root_layer(window), bitmap_layer_get_layer( backgroundLayer ));
  
  eventloop_load( window );
  appmessagecontroller_load( window );
  
  timecontroller_load( window );
  batterycontroller_load( window );
  weathercontroller_load( window );
  bluetoothcontroller_load( window );
  
}

static void mainWindowUnload( Window *window ) 
{
  gbitmap_destroy( backgroundBitmap );
  bitmap_layer_destroy( backgroundLayer );
  
  timecontroller_unload();
  batterycontroller_unload();
  weathercontroller_unload();
  bluetoothcontroller_unload();
  
  appmessagecontroller_unload();
  eventloop_unload();
}

static void init() 
{
  mainWindow = window_create();
  window_set_window_handlers( mainWindow, (WindowHandlers) {
    .load = mainWindowLoad,
    .unload = mainWindowUnload
  });
  
  window_stack_push( mainWindow, true );
}

static void deinit() 
{ 
  // Destroy Window
  window_destroy( mainWindow );
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}