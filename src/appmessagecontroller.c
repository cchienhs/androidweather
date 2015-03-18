#include <pebble.h>
#include "appmessagecontroller.h"
#include "weathercontroller.h"
#include "timecontroller.h"
   
static void inboxReceivedCallback(DictionaryIterator *iterator, void *context) 
{
  Tuple *t = dict_read_first(iterator);
  while(t != NULL) {
    switch(t->key) {
      case KEY_TEMPERATURE:
        weathercontroller_updateTemperature( (int)t->value->int32 );
        break;
      case KEY_TEMPERATURE_MAX:
        weathercontroller_updateMaxTemperature( (int)t->value->int32 );
        break;
      case KEY_TEMPERATURE_MIN:
        weathercontroller_updateMinTemperature( (int)t->value->int32 );
        break;
      case KEY_CONDITIONS:
        weathercontroller_updateCondition( t->value->cstring );
        break;
      case KEY_TEMPERATURE_UNIT:
        weathercontroller_setTemperatureUnit( t->value->cstring );
        break;
      case KEY_BLINKING_COLONS:
        timecontroller_setBlinkingColons( t->value->int32 );
        break;
      case KEY_WEATHER_UPDATE_INTERVAL:
        weathercontroller_setWeatherUpdateInterval( t->value->int32 );
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    t = dict_read_next(iterator);
  }  
}

static void inboxDroppedCallback(AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outboxFailedCallback(DictionaryIterator *iterator, AppMessageResult reason, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outboxSentCallback(DictionaryIterator *iterator, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void appmessagecontroller_sendMessage( int key, int value )
{
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, key, value);
  app_message_outbox_send();
}

void appmessagecontroller_load( Window *window )
{
    app_message_register_inbox_received(inboxReceivedCallback);
    app_message_register_inbox_dropped(inboxDroppedCallback);
    app_message_register_outbox_failed(outboxFailedCallback);
    app_message_register_outbox_sent(outboxSentCallback);
    
    app_message_open(200, 200);
}

void appmessagecontroller_unload()
{
  
}