#include <stdint.h>                                                           
 /* FN: Violation hidden behind macro expansion.                               
    After preprocessing this becomes:                                          
        extern int32_t buffer[];                                               
    but the tool may fail to resolve the macro and miss the violation. */      
 #define DECLARE_EXTERN_ARRAY(elem_type, name) extern elem_type name[]         
 DECLARE_EXTERN_ARRAY(int32_t, buffer);                                        
 int32_t get_first(void)                                                       
 {                                                                             
     return buffer[0];                                                         
 }