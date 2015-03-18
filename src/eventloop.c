#include <pebble.h>
#include "eventloop.h"

struct Node
{
  EventCallbackType callback;
  struct Node *next;
};

static struct Node *head = NULL;
  
static void tickHandler(struct tm *tick_time, TimeUnits units_changed) 
{
  if( head != NULL ) {
    struct Node *temp = head;
    while( temp ) {
      temp->callback();
      temp = temp->next;
    }
  }
}
  
void eventloop_subscribe( EventCallbackType callback )
{
  if( head == NULL ) {
    head = (struct Node*)malloc(sizeof(struct Node));
    head->callback = callback;
    head->next = NULL;
  }
  else {
    struct Node *temp = head;
    
    while( temp->next != NULL ) {
      temp = temp->next;
    }
    
    temp->next = (struct Node*)malloc( sizeof(struct Node) );
    temp = temp->next;
    temp->callback = callback;
    temp->next = NULL;
  }
}

void eventloop_unsubscribe( EventCallbackType callback )
{
  if( head != NULL ) {
    if( head->callback == callback ) {
      struct Node *temp = head;
      head = head->next;
      free( temp );
    }
    else{
      struct Node *temp = head;
      while( temp->next != NULL ) {
        if( temp->next->callback == callback ) {
          struct Node *childNode = temp->next->next;
          free( temp->next );
          temp->next = childNode;
          break;
        }
      }
    }
  }  
}

void eventloop_load( Window *window )
{
  tick_timer_service_subscribe(SECOND_UNIT, tickHandler);
}

void eventloop_unload()
{
  tick_timer_service_unsubscribe();
}

