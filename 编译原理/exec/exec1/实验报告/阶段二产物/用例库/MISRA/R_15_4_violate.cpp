// MISRA R15.4 VIOLATE - Loop with multiple break statements                  
 extern void consume(int);                                                     
 void test_violate(void) {                                                     
     int arr[10] = {0};                                                        
     int found = 0;                                                            
     for (int i = 0; i < 10; i++) {                                            
         if (arr[i] == 1) {                                                    
             found = 1;                                                        
             break;  // first break                                            
         }                                                                     
         if (i == 8) {                                                         
             break;  // second break - VIOLATION                               
         }                                                                     
     }                                                                         
     consume(found);                                                           
 }                                                                             
 void test_violate_goto(void) {                                                
     int sum = 0;                                                              
     for (int i = 0; i < 10; i++) {                                            
         if (i == 3) {                                                         
             break;   // first termination                                     
         }                                                                     
         if (i == 5) {                                                         
             goto end;  // second termination - VIOLATION                      
         }                                                                     
         sum += i;                                                             
     }                                                                         
 end:                                                                          
     consume(sum);                                                             
 }