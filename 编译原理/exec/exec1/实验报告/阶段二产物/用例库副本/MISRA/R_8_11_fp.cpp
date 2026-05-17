#include <stdint.h>                                                           
 /* FP: internal-linkage array (static), R8.11 only applies to external        
 linkage.                                                                      
    A naive tool may still flag this because the size is omitted. */           
static int32_t local_buf[] = { 0, 0, 0, 0 };                                   
 static int32_t read_local(int32_t index)                                      
 {                                                                             
     local_buf[0] = 42;                                                        
     return local_buf[index];                                                  
 }
