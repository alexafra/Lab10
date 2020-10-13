#include "eyebot.h"
#include <stdio.h>
// #define HORIZONTAL_MAX 320
// #define VERTICAL_MAX 240

int v_des;  /* desired ticks/s */
int enc_new1, v_act1;
int count = 0;
int prevCount = 0;
void onoff_controller()
{ 
    count++;

    int r_mot1;
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
    CAMInit(QQVGA);
    LCDImageSize(QQVGA);
    LCDPrintf("STARTING");
    FILE* ptr = fopen("/home/pi/usr/output.csv", "w");
    v_des = 10000;
    TIMER t1;
    t1 = OSAttachTimer(10, onoff_controller); //100ms
    LCDMenu("","","","Quit");

    
    //x = 320
    //y = 240


    while (KEYRead() != KEY4) { /* check key */
        // LCDPrintf("Encoder: %d \n", enc_new1);
        // LCDPrintf("Encoder: %d \n", v_act1);
        // LCDPrintf("Encoder: %d \n", count);
        // fprintf(ptr, "Encoder: %d \n", v_act1);

        if (count != prevCount) {
            prevCount = count;
            fprintf(ptr, "%d,%d\n", enc_new1,v_act1);
        }
        int mod = count%320;
        if(v_act1 > 0) {
            LCDPixel(mod,240-(v_act1/100),WHITE);
        }
        if(mod == 0){
            LCDClear();
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

void CamToDisplay(){
    LCDImage(CAMGet(colimage));
}

//Sure this can be found from a constant somewhere
int height = 240;
int width = 320;
int[2] CENTER = [width/2,height/2];

void Circle(int[2] position){
    LCDCircle(position[0], position[1], 10, WHITE, NULL);
}

/* EXAMPLE OF USE */
void CenterCircle(){
    Circle(CENTER);
}

void PrintHueToScreen(int[2] position){
    LCDSetPrintf(position[0], position[1], ("%d,%d",HueFromPosition(position)[0],HueFromPosition(position)[1]);
}

int[3] RGBFromPosition(int[] position){
    CAMGet(colimage);
    /* TO BE IMPLEMENTED */
}

int[2] RGBToHSI(int[3] RGB){
    /* TO BE IMPLEMENTED */
}

int[2] HueFromPosition(int[] position){
    return RGBToHSI(RGBFromPosition(position));
}
