#include <pebble.h>
#include "bluetoothcontroller.h"
#include "appmessagecontroller.h"

static bool vibrateOnBluetoothEvents = true;

static void bluetoothHandler(bool connected) 
{
  vibes_short_pulse();
}

void bluetoothcontroller_load( Window *window )
{
   if( persist_exists( KEY_VIBRATE_ON_BLUETOOTH_EVENTS ) ) {
      bluetoothcontroller_setVibrateOnBluetoothEvents( persist_read_bool( KEY_VIBRATE_ON_BLUETOOTH_EVENTS) );
    }
}

void bluetoothcontroller_unload()
{
  if( vibrateOnBluetoothEvents )
    bluetooth_connection_service_unsubscribe();
}

void bluetoothcontroller_setVibrateOnBluetoothEvents( bool value )
{
    persist_write_bool( KEY_VIBRATE_ON_BLUETOOTH_EVENTS , value );
    vibrateOnBluetoothEvents = value;  
    if( vibrateOnBluetoothEvents )
      bluetooth_connection_service_subscribe(bluetoothHandler);
    else
      bluetooth_connection_service_unsubscribe();
}

bool bluetoothcontroller_getVibrateOnBluetoothEvents()
{
    return vibrateOnBluetoothEvents;
}