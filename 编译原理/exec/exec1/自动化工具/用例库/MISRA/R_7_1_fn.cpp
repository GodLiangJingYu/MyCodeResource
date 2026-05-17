/* 通过宏定义隐藏八进制字面量，静态分析工具容易漏报 */                        
 #define FILE_PERM_READ    0444                                                
 #define FILE_PERM_WRITE   0222                                                
 #define FILE_PERM_EXEC    0111                                                
 #define FILE_PERM_RW      0660                                                
 #define COMBINED_PERM(r, w, x) ((r) | (w) | (x))                              
 #define STACK_SIZE    0100    /* 八进制 64，表面看像十进制 100 */             
 #define BUFFER_LEN    0200    /* 八进制 128，表面看像十进制 200 */            
 static int get_permission(int mode) {                                         
     if (mode == 0) {                                                          
         return FILE_PERM_READ;                                                
     } else if (mode == 1) {                                                   
         return FILE_PERM_WRITE;                                               
     } else {                                                                  
         return FILE_PERM_RW;                                                  
     }                                                                         
 }                                                                             
 void test_r71_fn(void) {                                                      
     int perm = get_permission(1);                                             
     int combined = COMBINED_PERM(FILE_PERM_READ, FILE_PERM_WRITE,             
 FILE_PERM_EXEC);                                                              
     int stack[STACK_SIZE];                                                    
     int buf[BUFFER_LEN];                                                      
     int hidden = 012 + 034;  /* 八进制 10 + 28 = 38 */                        
     stack[0] = perm;                                                          
     buf[0] = combined;                                                        
     stack[1] = hidden;                                                        
     (void)stack;                                                              
     (void)buf;                                                                
 }