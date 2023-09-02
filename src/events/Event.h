#ifndef EVENT_H
#define EVENT_H

#include "../commons/Describable.h"

/**
 * This is the base Event's type definition
 * It just defines methods for type inference
 */
class Event : public Describable {

};

#include "events/ButtonEvent.h"
#include "events/MessageEvent.h"

#endif
