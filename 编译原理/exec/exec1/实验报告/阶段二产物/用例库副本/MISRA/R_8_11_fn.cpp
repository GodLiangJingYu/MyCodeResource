#include <stdint.h>                                                           
 /* FN: Violation hidden behind macro expansion.                               
    After preprocessing this becomes:                                          
        extern int32_t buffer[];                                               
    but the tool may fail to resolve the macro and miss the violation. */      
 #define DECLARE_EXTERN_ARRAY(elem_type, name, size) extern elem_type name[size]
 DECLARE_EXTERN_ARRAY(int32_t, buffer, 10);                                        
 int32_t get_first(void)                                                       
 {                                                                             
     return buffer[0];                                                         
 }