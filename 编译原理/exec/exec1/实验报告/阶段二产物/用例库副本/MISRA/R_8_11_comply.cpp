#include <stdint.h>                                                           
 /* Comply: extern array with external linkage, size explicitly specified */   
 extern int32_t sensor_data[128];                                              
 int32_t get_first(void)                                                       
 {                                                                             
     return sensor_data[0];                                                    
 }