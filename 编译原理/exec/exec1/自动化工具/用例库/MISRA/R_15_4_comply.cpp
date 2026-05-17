// MISRA R15.4 COMPLY - Loop with at most one break/goto per loop             
 extern void consume(int);                                                     
 void test_comply_single_break(void) {                                         
     int arr[10] = {0};                                                        
     int found = 0;                                                            
     for (int i = 0; i < 10; i++) {                                            
         if (arr[i] == 1) {                                                    
             found = 1;                                                        
             break;  // only one break in this loop                            
         }                                                                     
     }                                                                         
     consume(found);                                                           
 }                                                                             
 void test_comply_no_break(void) {                                             
     int sum = 0;                                                              
     for (int i = 0; i < 10; i++) {                                            
         sum += i;  // no break or goto at all                                 
     }                                                                         
     consume(sum);                                                             
 }                                                                             
 void test_comply_separate_loops(void) {                                       
     int a = 0;                                                                
     int b = 0;                                                                
     for (int i = 0; i < 10; i++) {                                            
         if (i == 5) {                                                         
             break;  // only break in this loop                                
         }                                                                     
         a += i;                                                               
     }                                                                         
     for (int j = 0; j < 10; j++) {                                            
         if (j == 3) {                                                         
             break;  // only break in a different loop - OK                    
         }                                                                     
         b += j;                                                               
     }                                                                         
     consume(a + b);                                                           
 }