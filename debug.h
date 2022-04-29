#ifdef DEBUG
    #define MYDEBUG_PRINT(x)     Serial.print (x)
    #define MYDEBUG_PRINTDEC(x)  Serial.print (x, DEC)
    #define MYDEBUG_PRINTHEX(x)  Serial.print (x, HEX)
    #define MYDEBUG_PRINTLN(x)   Serial.println (x)
    #define MYDEBUG_PRINTF(a,b,c,d,e)    Serial.printf (a,b,c,d,e)
#else
    #define MYDEBUG_PRINT(x)
    #define MYDEBUG_PRINTDEC(x)
    #define MYDEBUG_PRINTHEX(x)
    #define MYDEBUG_PRINTLN(x)
    #define MYDEBUG_PRINTF(x)
#endif
  
void setupDebug(){
    #ifdef DEBUG 
        Serial.begin(115200);
        Serial.println("Ouverture du port série");
    #endif  
}
