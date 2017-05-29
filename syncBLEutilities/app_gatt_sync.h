/*API_Config*/

#ifndef _APP_H_
#define _APP_H_

typedef enum 
{
  _OPT_SUCCESS = 0x00,
  _OPT_ERROR = 0x10,
  _APP_NOT_CONFIG= 0x02
} _OPT_Status;

typedef struct scan_adv_conf_{
	uint16_t LE_scan_interval;
	uint16_t LE_scan_windows;
}SC_adv_conf;/*scan adverticement configuration*/

typedef struct connection_setup_conf_{
	uint16_t LE_scan_interval;
	uint16_t LE_scan_windows;
	uint16_t LE_CI_max;
	uint16_t LE_CI_min;
	uint16_t Conn_Latency;
	uint16_t LE_Timeout_;
	uint16_t Conn_Len_Min;
	uint16_t Conn_Len_Max;
}CN_setup_conf; /*conf Connection Setup Master to slave*/



typedef struct app_conf_{
	uint16_t num_slaves;
	SC_adv_conf scan_adv_c;
	CN_setup_conf CN_c;
}app_Master_Broadcast_conf;



typedef struct app_conf_{
	uint16_t num_masters;
	uint16_t LE_adv_int_min;
	uint16_t LE_adv_int_max;
	
}app_Slave_Observer_conf;


typedef struct app_{
	uint32_t role;
	app_Master_broadcast_conf M_conf;
	app_Slave_observer_conf  S_conf;
}App;


#endif /* _APP_H_ */