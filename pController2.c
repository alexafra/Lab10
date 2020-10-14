#include "eyebot.h"
#include <stdio.h>
// #define HORIZONTAL_MAX 320
// #define VERTICAL_MAX 240

int v_des;  /* desired ticks/s */
int enc_new, v_act;
int count = 0;
int prevCount = 0;
float Kp = 0.018; 
float Ki = 0.0005;
void onoff_controller()
{ 
    count++;

    int r_mot;
    static int enc_old;
    static int r_old=0, e_old=0;
    enc_new = ENCODERRead(1); 
    v_act = (enc_new-enc_old)*100; 
    int e_func = v_des - v_act; 
    r_mot = r_old + Kp*(e_func-e_old) + Ki*(e_func+e_old)/2;
    if (r_mot > 100 ) {
        r_mot = 100;
    } else if (r_mot < -100) {
        r_mot = -100;  
    } 
    MOTORDrive(1, r_mot);
    r_old = r_mot;
    e_old = e_func;
    enc_old = enc_new; 
    
    // int enc_new2, v_act2, r_mot2;
    // static int enc_old2;
    // enc_new2 = ENCODERRead(2); 
    // v_act2 = (enc_new2-enc_old2)*100; 
    // if (v_act2 < v_des) {
    //     r_mot2 = 100;
    // } else {
    //     r_mot2 = 0;  
    // } 
    // MOTORDrive(2, r_mot2);
    // enc_old2 = enc_new2;
}

int main()
{ 
    LCDPrintf("SRARTING");
    FILE* ptr = fopen("/home/pi/usr/output4.csv", "w");
    v_des = 10000;
    TIMER t1;
    t1 = OSAttachTimer(10, onoff_controller); //100ms
    LCDMenu("","","","Quit");
    while (KEYRead() != KEY4) { /* check key */
        // LCDPrintf("Encoder: %d \n", enc_new1);
        // LCDPrintf("Encoder: %d \n", v_act1);
        // LCDPrintf("Encoder: %d \n", count);
        
        int mod = count%320;
        if (v_act > 0) {
            LCDPixel(mod,240-(v_act/100),WHITE);
        }
        if (mod == 0) {
            LCDClear();
        }
        
        // fprintf(ptr, "Encoder: %d \n", v_act1);
        if (count != prevCount) {
            prevCount = count;
            fprintf(ptr, "%d,%d\n", enc_new,v_act);
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