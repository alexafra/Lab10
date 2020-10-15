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
int sp = 80;

void PController(){
    //setpoint sp is lastXCrosshair
    //process variable pv is encoder location
    //p - proportion coeffic/* ient
    /**float p = 0.1;
    static int enc_old;
    //int enc_new = ENCODERRead(1); //to be changed for correct motor
    int e = sp - lastXCrosshair;
    int output = -p*e;
    if (output > 100 ) {
        output = 100;
    } else if (output < -100) {
        output = -100;  
    }
     //to be changed for correct motor
    enc_old = enc_new;\
    
 */

    if(lastXCrosshair < 30){
        SERVOSet(1, 200);
    } else if(lastXCrosshair < 120) {
        SERVOSet(1, 600);
    } else {
        SERVOSet(1, 1000);
    }
}
void Crosshair(int x, int y){
    LCDLine(x,y+5,x,y-5,WHITE);
    LCDLine(x+5,y,x-5,y,WHITE);
}

void PrintHue(int x, int y, BYTE hsiimage[QQVGA_SIZE]){
    // print("Hue: %d",(int)hsiimage[y*160+x]);
    int hueTot = 0;
    for(int i = -1;i<2;i++){
        for(int j = -1;j<2;j++){
            hueTot = hueTot + hsiimage[((y+i)*160+(x+j))*3];
        }
    }
    int hueAv = hueTot/9;

    // int intTot = 0;
    // for(int i = -1;i<2;i++){
    //     for(int j = -1;j<2;j++){
    //         intTot = intTot + hsiimage[((y+i)*160+(x+j))*3+2];
    //     }
    // }
    // int intAv = intTot/9;

    // LCDPrintf("Hue: %d\n", hueAv);
    // LCDPrintf("Int: %d\n", intAv);
    
}

void ImageFilter(BYTE* hsiimage, BYTE binimage[QQVGA_PIXELS]){
    //Check for red hue
    // int binimage[QQVGA_PIXELS];
    int hueUpper = 55;
    int hueLower = 35;
    
    int intLower = 30;
    int intHigher = 127;

    int satLower = 0;
    int satHigher = 127;

    for(int i = 0;i<QQVGA_PIXELS;i++){
        if(hsiimage[i*3] > hueLower && hsiimage[i*3] < hueUpper) { //Hue check TO BE DETERMINED
            if(hsiimage[i*3+2] > intLower && hsiimage[i*3+2] < intHigher){ //Intensity check
                if(hsiimage[i*3+1] > satLower && hsiimage[i*3+1] < satHigher){ //Intensity check
                    binimage[i]=0;
                }
                //Set this pixel to be red on the binary image TO BE IMPLEMENTED
                else{
                    binimage[i] = 1;
                }
            } else {
                binimage[i] = 1;
            }
        } else {
            binimage[i] = 1;
        }
    }
}

int MaxColumnHistogram(BYTE* binimage){
    int histogram[160];
    int min = 0;
    for(int i = 0;i<160;i++){
        for(int j = 0; j<120; j++){
            histogram[i]+=binimage[j*160+i];
            if(histogram[i]<histogram[min]) min = i;
        }
    }
    return min;
}

int MaxRowHistogram(BYTE* binimage){
    int histogram[120];
    int min = 0;
    for(int i = 0;i<120;i++){
        for(int j = 0; j<160; j++){
            histogram[i]+=binimage[i*160+j];
            if(histogram[i]<histogram[min]) min = i;
        }
    }
    return min;
}

int MAX(int a, int b){
    if(a>b) return a;
    return b;
}

bool MIN(int a, int b){
    if(a<b) return a;
    return b;
}

void RgbToHsiPixel(BYTE p_red,BYTE p_green,BYTE p_blue, BYTE hsi[3]){
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
    int sat = 1-(min/(p_red+p_green+p_blue));
    int intensity = (int) ((1.0/3.0)*(p_red+p_green+p_blue));
    hsi[0] =hue;
    hsi[1] = sat;
    hsi[2] = intensity;
}

void  RgbToHsiImage(BYTE image[QQVGA_SIZE], BYTE hsiimage[QQVGA_SIZE]){
    for(int pixel = 0;pixel<QQVGA_SIZE;pixel=pixel+3){
        BYTE p_red = image[pixel];
        BYTE p_green = image[pixel+1];
        BYTE p_blue = image[pixel+2];
        BYTE HSI[3];
        RgbToHsiPixel(p_red,p_green,p_blue, HSI);
        hsiimage[pixel] = HSI[0];
        hsiimage[pixel+1] = HSI[1];
        hsiimage[pixel+2] = HSI[2];
    }
}

int main()
{ 
    CAMInit(QQVGA);
    LCDImageSize(QQVGA);
    BYTE colimage[QQVGA_SIZE];
    BYTE hsiimage[QQVGA_SIZE];
    BYTE binimage[QQVGA_PIXELS];

    //TIMER t1;
    //t1 = OSAttachTimer(10, onoff_controller); //100ms
    
    LCDMenu("","","Take HSI","Quit");
    
    //Timer setup
    TIMER t1;
    t1 = OSAttachTimer(10, PController); //100ms

    while (1) {
        if(KEYRead() == KEY4) break;
        
        CAMGet(colimage);
        LCDImageStart(0,0,160,120);
        LCDImage(colimage);

        RgbToHsiImage(colimage, hsiimage);
        ImageFilter(hsiimage, binimage);

        LCDImageStart(160,0,160,120);
        LCDImageBinary(binimage);

        
        PrintHue(lastXCrosshair,lastYCrosshair,hsiimage);
        
        
        lastXCrosshair = MaxColumnHistogram(binimage);
        lastYCrosshair = MaxRowHistogram(binimage);

        Crosshair(lastXCrosshair,lastYCrosshair);        
            
        

        
    } 
    
    return 0; 
}



