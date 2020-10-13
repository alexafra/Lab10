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
    BYTE myimage[QQVGA_SIZE];

    /* FILE* ptr = fopen("/home/pi/usr/output.csv", "w");
    v_des = 10000;
    TIMER t1;
    t1 = OSAttachTimer(10, onoff_controller); //100ms
     */
    LCDMenu("","","","Quit");
    
    //x = 320
    //y = 240


    while (KEYRead() != KEY4) { /* check key */
        // LCDPrintf("Encoder: %d \n", enc_new1);
        // LCDPrintf("Encoder: %d \n", v_act1);
        // LCDPrintf("Encoder: %d \n", count);
        // fprintf(ptr, "Encoder: %d \n", v_act1);

        /* if (count != prevCount) {
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
        } */
        /* do other tasks, e.g. set speeds */ 

        
        CAMGet(myimage);
        LCDImage(myimage);
    } 
    
    return 0; 
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

BYTE[3] RGBFromPosition(int[] position){
    CAMGet(colimage);
    int pixel = PositionToPixelIndex(position);
    BYTE p_red = colimage[pixel*3];
    BYTE p_green = colimage[pixel*3+1];
    BYTE p_blue = colimage[pixel*3+2];
    return [p_red,p_green,p_blue];
}

BYTE[3] RGBToHSI(BYTE[3] RGB){
    /* return hue value for RGB color */
    #define NO_HUE 255
    int hue, delta, max, min;
    max = MAX(RGB[0], MAX(RGB[1],RGB[2]));
    min = MIN(RGB[0], MIN(RGB[1],RGB[2]));
    delta = max - min;
    hue =0; /* init hue*/
    if (2*delta <= max) hue = NO_HUE; /* gray, no color */
    else {
        if (RGB[0]==max) hue = 42 + 42*(RGB[1]-RGB[2])/delta; /* 1*42 */
        else if (RGB[1]==max) hue = 126 +42*(RGB[2]-RGB[0])/delta; /* 3*42 */
        else if (RGB[2]==max) hue = 210 +42*(RGB[0]-RGB[1])/delta; /* 5*42 */
    }
    sat = 1-(min/(RGB[0]+RGB[1]+RGB[2]));
    intensity = (1/3)*(RGB[0]+RGB[1]+RGB[2]);
    return [hue,sat,intensity];
}

int[2] HueFromPosition(int[] position){
    return RGBToHSI(RGBFromPosition(position));
}

int PositionToPixelIndex(int[] position){
    return position[0]*width+position[1];
}
