#ifndef COMPONENTS_COMMONS_H
#define COMPONENTS_COMMONS_H

#ifndef COMPONENTS_DEFAULT_LOG_LEVEL
#define COMPONENTS_DEFAULT_LOG_LEVEL LogLevel::TRACE
#endif

#define _constrain(value,low,high) ((value)<(low)?(low):((value)>(high)?(high):(value)))
#define _round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define _cube(x) ((x)*(x)*(x))
#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))

#ifndef ENTER_ATOMIC_BLOCK
#include <avr/interrupt.h>
#define ENTER_ATOMIC_BLOCK uint8_t sreg = SREG; cli();
#endif

#ifndef EXIT_ATOMIC_BLOCK
#include <avr/interrupt.h>
#define EXIT_ATOMIC_BLOCK SREG = sreg;
#endif

#ifndef MILLIS_PROVIDER
#include <Arduino.h>
#define MILLIS_PROVIDER() millis()
#endif

#include "utils/fixed_trigonometry.h"
#include "utils/simplelogger/simplelogger.h"
#include "Enums.h"

#endif
