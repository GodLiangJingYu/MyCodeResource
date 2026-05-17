#define FEATURE_A 1                                                           
 #define FEATURE_B 1                                                           
 #if FEATURE_A                                                                 
 #define CONFIG_SIZE 128                                                       
 #endif                                                                        
 #if FEATURE_B                                                                 
 #define CONFIG_SIZE 256                                                       
 #endif                                                                        
 int main(void) {                                                              
     int buf[CONFIG_SIZE];                                                     
     (void)buf;                                                                
     return 0;                                                                 
 }