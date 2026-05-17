/* 以下代码看起来可能触发八进制告警，但实际语义安全 */                        
 #include <stdint.h>                                                           
 /* 字符串中的"0"开头数字不是八进制字面量 */                                   
 static const char *version_str = "001.002.003";                               
 static const char *octal_str   = "0755 is not an octal integer literal";      
 void test_r71_fp(void) {                                                      
     int zero = 0;               /* 0 是零值，非八进制用法 */                  
     int hex_zero = 0x0;         /* 十六进制零 */                              
     int dec_ten = 10;           /* 十进制 */                                  
     char ch = '\0';             /* 空字符转义，非八进制字面量 */              
     int arr[10] = {0};          /* 初始化为零 */                              
     int ptr_val = (int)(uintptr_t)((void *)0);  /* NULL 展开为 0 */           
     unsigned int zero_u = 0u;   /* 带后缀的 0 不是八进制常量 */               
     static const char *looks_like_octal = "01234";                                                         
     (void)zero;                                                               
     (void)hex_zero;                                                           
     (void)dec_ten;                                                            
     (void)ch;                                                                 
     (void)arr;                                                                
     (void)ptr_val;                                                            
     (void)version_str;                                                        
     (void)octal_str;                                                          
     (void)zero_u;                                                             
     (void)looks_like_octal;                                                           
 }
