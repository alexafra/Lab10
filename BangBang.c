int v_des;  /* desired ticks/s */
void onoff_controller()
{ 
    int enc_new, v_act, r_mot;
    static int enc_old;
    enc_new = ENCODERRead(1); 
    v_act = (enc_new-enc_old)*100; 
    if (v_act < v_des) {
        r_mot = 100;
    } else {
        r_mot = 0;  
    } 
    MOTORDrive(1, r_mot);
    enc_old = enc_new; 
}

int main () {
    
}