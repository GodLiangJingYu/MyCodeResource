// MISRA R15.4 FP - Two break statements but in different loops (nested)      
 extern void consume(int);                                                     
 void test_fp_nested_loops(void) {                                             
     int matrix[4][4] = {{0}};                                                 
     int found = 0;                                                            
     for (int i = 0; i < 4; i++) {                                             
         for (int j = 0; j < 4; j++) {                                         
             if (matrix[i][j] == 1) {                                          
                 found = 1;                                                    
                 break;  // breaks inner loop only                             
             }                                                                 
         }                                                                     
        if (found) {
            break;  // breaks outer loop only - each loop has exactly one break
        }
     }                                                                         
     consume(found);                                                           
 }                                                                             
 void test_fp_separate_same_scope(void) {                                      
     int val = 0;                                                              
     for (int i = 0; i < 10; i++) {                                            
         if (i == 3) {                                                         
             break;  // single break in this loop                              
         }                                                                     
         val += i;                                                             
     }                                                                         
     // Next loop in same function but distinct                                
     for (int i = 0; i < 10; i++) {                                            
         if (i == 5) {                                                         
             break;  // single break in this separate loop                     
         }                                                                     
         val -= i;                                                             
     }                                                                         
     consume(val);                                                             
 }
