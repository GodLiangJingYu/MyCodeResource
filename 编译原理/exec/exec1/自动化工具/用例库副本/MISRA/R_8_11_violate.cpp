#include <stdint.h>                                                           
 /* Violate: extern array with external linkage, size not explicitly specified 
 */                                                                            
 extern int32_t sensor_data[10];                                                 
 int32_t get_first(void)                                                       
 {                                                                             
     return sensor_data[0];                                                    
 }