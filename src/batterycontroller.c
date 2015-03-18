#include <pebble.h>
#include "batterycontroller.h"

#define BATTERY_OUTLINE_WIDTH 20
#define BATTERY_OUTLINE_HEIGHT 8
static const GPathInfo BATTERY_OUTLINE = {
    .num_points = 4,
    .points = (GPoint []) {{0, 0}, {BATTERY_OUTLINE_WIDTH, 0}, {BATTERY_OUTLINE_WIDTH, BATTERY_OUTLINE_HEIGHT}, {0, BATTERY_OUTLINE_HEIGHT}}
  };

static bool initiated = 0;
static TextLayer *batteryLayer;
static Layer *batteryGraphicsLayer;
static GPath *batteryOutlinePath = NULL;
static int batteryLevel = 0;

static void batteryGraphicsLayerDraw( Layer *layer, GContext *ctx ) 
{
   // Stroke the path:
  graphics_context_set_stroke_color(ctx, GColorWhite);
  gpath_draw_outline(ctx, batteryOutlinePath);
  
  // Signify the percentage:
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect( ctx, GRect( 0, 0, (batteryLevel/100.) * BATTERY_OUTLINE_WIDTH, BATTERY_OUTLINE_HEIGHT ), 0, 0 );
}

static void batteryHandler( BatteryChargeState chargeState ) 
{
  static char batteryBuffer[16];

  if ( chargeState.is_charging ) {
    snprintf( batteryBuffer, sizeof( batteryBuffer ), "charging" );
  } 
  else {
    snprintf( batteryBuffer, sizeof( batteryBuffer ), "%d%%", chargeState.charge_percent );
  }
  
  batteryLevel = chargeState.charge_percent;
  text_layer_set_text( batteryLayer, batteryBuffer );
}

static void createBatteryLayer( Window *window )
{
  batteryLayer = text_layer_create( GRect(110, 110, 30, 20) );
  text_layer_set_background_color( batteryLayer, GColorBlack );
  text_layer_set_text_color( batteryLayer, GColorWhite );
  text_layer_set_text( batteryLayer, "100%" );
  text_layer_set_font( batteryLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14) );
  text_layer_set_text_alignment( batteryLayer, GTextAlignmentCenter );
  layer_add_child( window_get_root_layer(window), text_layer_get_layer( batteryLayer) );
}

static void createBatteryGraphicsLayer( Window *window )
{  
  batteryGraphicsLayer = layer_create(GRect(115, 130, 31, 10));
  layer_set_update_proc( batteryGraphicsLayer, batteryGraphicsLayerDraw );
  layer_add_child( window_get_root_layer(window), batteryGraphicsLayer );
  
  batteryOutlinePath = gpath_create(&BATTERY_OUTLINE);
}

void batterycontroller_load( Window *window )
{
  if( initiated == 0 ) {
    initiated = 1;
    createBatteryLayer( window );
    createBatteryGraphicsLayer( window );
    battery_state_service_subscribe( batteryHandler );
    batteryHandler( battery_state_service_peek() );
  }  
}

void batterycontroller_unload()
{
  text_layer_destroy( batteryLayer );
  layer_destroy( batteryGraphicsLayer );
  gpath_destroy( batteryOutlinePath );
}