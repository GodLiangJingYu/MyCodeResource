#include <stdint.h>                                                           
 void test_r71_comply(void) {                                                  
     int a = 8;          /* 十进制 */                                          
     int b = 63;         /* 十进制 */                                          
     int c = 83;         /* 十进制 */                                          
     int d = 420;        /* 十进制 */                                          
     int e = 493;        /* 十进制 */                                          
     int f = 0;          /* 零值，无前缀 */                                    
     int g = 0xFF;       /* 十六进制 */                                        
     uint32_t h = 127U;  /* 十进制带后缀 */                                    
     long i = 255L;      /* 十进制带后缀 */                                    
     (void)a;                                                                  
     (void)b;                                                                  
     (void)c;                                                                  
     (void)d;                                                                  
     (void)e;                                                                  
     (void)f;                                                                  
     (void)g;                                                                  
     (void)h;                                                                  
     (void)i;                                                                  
 }