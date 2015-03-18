#pragma once
  
typedef void (*EventCallbackType)();

extern void eventloop_load( Window *window );
extern void eventloop_unload();
extern void eventloop_subscribe( EventCallbackType callback );
extern void eventloop_unsubscribe( EventCallbackType callback );
