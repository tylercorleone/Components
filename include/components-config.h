#ifndef COMPONENTS_CONFIG_H
#define COMPONENTS_CONFIG_H

#ifndef COMPONENTS_DEFAULT_LOG_LEVEL
#define COMPONENTS_DEFAULT_LOG_LEVEL LogLevel::TRACE
#endif

#ifndef DEBUG_ENTERING_IDLE_STATE
/* Set DEBUG_ENTERING_IDLE_STATE to true if you want to be alerted
 * when entering an idle loop state (onIdle)
 */
#define DEBUG_ENTERING_IDLE_STATE false
#endif

#endif
