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

#ifndef DEBUG_ENTERING_IDLE_STATE
/* Set DEBUG_ENTERING_IDLE_STATE to true if you want to be alerted
 * when entering an idle loop state (onIdle)
 */
#define DEBUG_ENTERING_IDLE_STATE false
#endif

#include "utils/fixed_trigonometry.h"
#include "utils/simplelogger/Logger.h"
#include "Enums.h"
#include "Identifiable.h"
#include "Describable.h"

#endif
