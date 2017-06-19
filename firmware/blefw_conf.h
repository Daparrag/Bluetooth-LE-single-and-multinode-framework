#ifndef _BLEFW_CONFIG_H_
#define _BLEFW_CONFIG_H_
 

#define GENERAL_DISCOVERY		(0x0)
#define SELECTIVE_DISCOVERY		(0x1)
#define SPECIFIC_DISCOVERY      (0x2)


#define DISCOVERY_MODE          SELECTIVE_DISCOVERY


#define LED_TOGGLE_CONFIG
#define LED_TOGGLE_UNESTABLISHED      80000
#define LED_TOGGLE_DISCOVERY          80000
#define LED_TOGGLE_CONNECTED          10000
#endif

#ifdef DEBUG
#undef DEBUG


#endif /* _BLEFW_CONFIG_H_ blefw_conf.h*/