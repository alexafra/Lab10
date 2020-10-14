#include "eyebot.h"
#include <stdio.h>
// #define HORIZONTAL_MAX 320
// #define VERTICAL_MAX 240

int v_des;  /* desired ticks/s */
int enc_new1, v_act1;
int count = 0;
int prevCount = 0;
float Kp = 0.5;
void onoff_controller()
{ 
    count++;

    int r_mot1;
    static int enc_old1;
    enc_new1 = ENCODERRead(1); 
    v_act1 = (enc_new1-enc_old1)*100; 
    int e_func = v_des - v_act1; 
    r_mot1 = (int)(Kp * 100.0) * e_func;
    MOTORDrive(1, r_mot1);
    enc_old1 = enc_new1; 
}

int main()
{ 
    LCDPrintf("SRARTING");
    FILE* ptr = fopen("/home/pi/usr/output2.csv", "w");
    v_des = 10000;
    TIMER t1;
    t1 = OSAttachTimer(10, onoff_controller); //100ms
    LCDMenu("","","","Quit");
    while (KEYRead() != KEY4) { /* check key */
        // LCDPrintf("Encoder: %d \n", enc_new1);
        // LCDPrintf("Encoder: %d \n", v_act1);
        // LCDPrintf("Encoder: %d \n", count);
        
        
        // fprintf(ptr, "Encoder: %d \n", v_act1);
        if (count != prevCount) {
            prevCount = count;
            // if (v_act1 >= 0)
            fprintf(ptr, "%d,%d\n", enc_new1,v_act1);
        }
        if (count == 1000) {
            fclose(ptr);
            OSDetachTimer(t1);
            break;
            return 0;
        }
        /* do other tasks, e.g. set speeds */ 
    } 
    
    return 0; 
}