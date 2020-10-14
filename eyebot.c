#include "eyebot.h"
#include <stdio.h>
// #define HORIZONTAL_MAX 320
// #define VERTICAL_MAX 240

/* int v_des;  // desired ticks/s 
int enc_new1, v_act1;
int count = 0;
int prevCount = 0; */
/* void onoff_controller()
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
} */

int lastXCrosshair = 80;
int lastYCrosshair = 60;

int main()
{ 
    CAMInit(QQVGA);
    LCDImageSize(QQVGA);
    BYTE colimage[QQVGA_SIZE];
    BYTE hsiimage[QQVGA_SIZE];
    BYTE binimage[160*120];

    /* FILE* ptr = fopen("/home/pi/usr/output.csv", "w");
    v_des = 10000;
    TIMER t1;
    t1 = OSAttachTimer(10, onoff_controller); //100ms
     */
    LCDMenu("","","Take HSI","Quit");
    
    //x = 320
    //y = 240
    bool firstIter = True;

    while (1) {
        if(KEYRead() == KEY4) break;
        
        CAMGet(colimage);
        LCDImageStart(0,0,160,120);
        LCDImage(colimage);
        hsiimage = RgbToHsiImage(colimage);

        binimage = ImageFilter(hsiimage);
        LCDImageStart(160,0,320,120);
        LCDImageBinary(binimage);

        Crosshair(lastXCrosshair,lastYCrosshair);
        PrintHue(lastXCrosshair,lastYCrosshair,hsiimage)
        
        if(KEYRead() == KEY3) {
            lastXCrosshair = MaxColumnHistogram(binimage);
            lastYCrosshair = MaxRowHistogram(binimage);
            
        }

        //Timer setup
        TIMER t1;
        t1 = OSAttachTimer(10, PController); //100ms
    } 
    
    return 0; 
}

void Crosshair(int x, int y){
    LCDLine(x,y+5,x,y-5,WHITE);
    LCDLine(x+5,y,x-5,y,WHITE);
}

void PrintHue(int x, int y, BYTE[] hsiimage){
    LCDSetPrintf(1, 1, ("Hue: %d",hsiimage[y*160+x]));
}

BYTE[160*120] ImageFilter(BYTE[] hsiimage){
    //Check for red hue
    int binimage[QQVGA];
    for(int i = 0;i<240*320;i++){
        if(hsiimage[i*3] > something && hsiimage[i*3] < something) { //Hue check
            if(hsiimage[i*3+2]>50){ //Intensity check
                binimage[i]=WHITE;
                //Set this pixel to be red on the binary image TO BE IMPLEMENTED
            }
        }
    }
    return binimage;
}

int MaxColumnHistogram(BYTE[] binimage){
    int histogram[160];
    int max = 0;
    for(int i = 0;i<160;i++){
        for(int j = 0; j<120; j++){
            histogram[i]+=binimage[j*160+i];
            if(histogram[i]>histogram[max]) max = i;
        }
    }
    return max;
}

int MaxRowHistogram(BYTE[] binimage){
    int histogram[120];
    int max = 0;
    for(int i = 0;i<120;i++){
        for(int j = 0; j<160; j++){
            histogram[i]+=binimage[i*160+j];
            if(histogram[i]>histogram[max]) max = i;
        }
    }
    return max;
}

BYTE[QQVGA] RgbToHsiImage(BYTE[] image){
    BYTE hsiimage[QQVGA];
    for(int pixel = 0;i<QQVGA;i=i+3){
        BYTE p_red = colimage[pixel*3];
        BYTE p_green = colimage[pixel*3+1];
        BYTE p_blue = colimage[pixel*3+2];
        BYTE HSI[3] = RgbToHsiPixel(p_red,p_green,p_blue);
        hsiimage[pixel] = HSI[0];
        hsiimage[pixel+1] = HSI[1];
        hsiimage[pixel+2] = HSI[2];
    }
    return hsiimage;
}

BYTE[3] RgbToHsiPixel(BYTE p_red,BYTE p_green,BYTE p_blue)){
    /* return hue value for RGB color */
    #define NO_HUE 255
    int hue, delta, max, min;
    max = MAX(p_red, MAX(p_green,p_blue));
    min = MIN(p_red, MIN(p_green,p_blue));
    delta = max - min;
    hue = 0; /* init hue*/
    if (2*delta <= max) hue = NO_HUE; /* gray, no color */
    else {
        if (p_red==max) hue = 42 + 42*(p_green-p_blue)/delta; /* 1*42 */
        else if (p_green==max) hue = 126 +42*(p_blue-p_red)/delta; /* 3*42 */
        else if (p_blue==max) hue = 210 +42*(p_red-p_green)/delta; /* 5*42 */
    }
    sat = 1-(min/(p_red+p_green+p_blue));
    intensity = (1/3)*(p_red+p_green+p_blue);
    return BYTE[hue,sat,intensity];
}

void PController(int pv){
    //setpoint sp is lastXCrosshair
    //process variable pv is encoder location
    //p - proportion coefficient
    float p = 0.01;
    int r_mot;
    static int enc_old;
    enc_new = ENCODERRead(1); //to be changed for correct motor
    pv = (enc_new-enc_old);
    int e = sp - pv;
    int output = p*e;
    if (output > 100 ) {
        output = 100;
    } else if (output < -100) {
        output = -100;  
    }
    MOTORDrive(1, output); //to be changed for correct motor
    enc_old1 = enc_new1;
}