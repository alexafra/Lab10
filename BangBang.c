#include "eyebot.h"
// #define HORIZONTAL_MAX 320
// #define VERTICAL_MAX 240

int v_des;  /* desired ticks/s */
void onoff_controller()
{ 
    int enc_new1, v_act1, r_mot1;
    static int enc_old1;
    enc_new1 = ENCODERRead(1); 
    v_act1 = (enc_new1-enc_old1)*100; 
    if (v_act1 < v_des) {
        r_mot1 = 100;
    } else {
        r_mot1 = 0;  
    } 
    MOTORDrive(1, r_mot1);
    enc_old1 = enc_new1; 
    
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
    v_des = 10000;
    TimerHandle t1;
    t1 = OSAttachTimer(10, onoff_controller); 
    while (KEYRead() != KEY4) { /* check key */
        
        /* do other tasks, e.g. set speeds */ 
    } 
    OSDetachTimer(t1);
    return 0; 
}