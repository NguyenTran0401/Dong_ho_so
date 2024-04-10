#define menu digitalRead(button_menu)
#define menu_bt digitalRead(button_menu_bt)
#define UP   digitalRead(tang)
#define DOWN digitalRead(giam)
#define on_coi  digitalWrite(coi,1)
#define off_coi digitalWrite(coi,0)

int D_in = 9;
int D_Sck = 10;
int D_RCk = 8;
int button_menu_bt = 2;
int button_menu = 3;
int tang = 4;
int giam = 5;
int coi = 13;
int mod_menu = 0 , mod_menu_bt = 0 ;
int i,n,k,tt,tt_coi = 0;
byte led = 0x00;
int thoigian,timecho,dem, tam = 0;
int hientai = 0, hientai1 = 0;
int diachi_gio_bt = 1, diachi_phut_bt = 2;

char led1[]= {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}    ;
//             0   1   2     3     4   5     6   7     8   9
char vitri[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
boolean khoa[] = { 0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int gio,phut,giay,ngay,thang,nam, s_gio,s_phut ;
#include <EEPROM.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
tmElements_t tm;

//---------------------------------------------------------------
void setup() {

 Serial.begin(9600);
 pinMode( D_in, OUTPUT);
 pinMode( D_Sck, OUTPUT);
 pinMode( D_RCk, OUTPUT);
 pinMode( coi, OUTPUT);

 pinMode( button_menu_bt, INPUT);
 pinMode( button_menu, INPUT);
 pinMode( tang, INPUT);
 pinMode( giam, INPUT);
 timecho=1000;
 dem = 0;
 thoi_gian_();
 tam = 0;
 s_gio = EEPROM.read(diachi_gio_bt);
 s_phut = EEPROM.read(diachi_phut_bt);
}
//------------------------------------------------------------------

void loop() 
{ 

  thoigian = millis();
   if(thoigian - hientai > timecho)
    {
        thoi_gian_();
        hientai = millis();
    }
      thoigian = millis();
   if(thoigian - hientai1 > 200)
    {
        khoa[mod_menu] = !khoa[mod_menu];
        if (mod_menu == 6) {khoa[mod_menu + 1] = !khoa[mod_menu + 1];}
        hientai1 = millis();
    }
   if (mod_menu !=0){ khoa[mod_menu -1] = 0;}
   if (mod_menu ==0){ khoa[7] = 0;khoa[6] = 0;}
   if ((menu ==0) & (mod_menu_bt == 0))
      {
        on_coi;
        while (menu ==0);
        off_coi;
        mod_menu++;    
      }
  //thoi_gian_();
  if (mod_menu_bt == 0)
  {
  hien_thi(gio/10,0 );    hien_thi(gio%10,1 ); 
  hien_thi(phut/10,2 );   hien_thi(phut%10,3 );
  hien_thi(giay/10,4 );   hien_thi(giay%10,5 );
  hien_thi(ngay/10,6 );   hien_thi(ngay%10,7 );
  hien_thi(thang/10,8 );  hien_thi(thang%10,9 );
  nam = tmYearToCalendar(tm.Year);  
  hien_thi(nam/1000,10 );               hien_thi((nam%1000)/ 100,11 );
  hien_thi(((nam%1000)%100)/10,12 );    hien_thi(((nam%1000)%100)%10,13 );
  }

  if (mod_menu == 1) {set_gio();    } 
  if (mod_menu == 2) {set_phut();   }
  if (mod_menu == 3) {set_giay();   }
  if (mod_menu == 4) {set_ngay();   }
  if (mod_menu == 5) {set_thang();  }
  if (mod_menu == 6) {set_nam();    }
  if (mod_menu >= 7) {mod_menu = 0; }

    
  if ((menu_bt == 0) & (mod_menu == 0))
      {
        on_coi;
        while (menu_bt ==0);
        off_coi;
        mod_menu_bt++;
        if (mod_menu_bt > 2) {  khoa[mod_menu_bt - 1 ] = 0; mod_menu_bt =0;}
       }
  if (mod_menu_bt >= 1 )
  { 
    dem++;
    if (dem > 50) {  khoa[mod_menu_bt] = !khoa[mod_menu_bt]; dem = 0;}
    khoa[mod_menu_bt - 1 ] = 0; 
    if (mod_menu_bt == 1) {set_s_gio();    } 
    if (mod_menu_bt == 2) {set_s_phut();   }
    hien_thi(s_gio/10,0 );    hien_thi(s_gio%10,1 );
    hien_thi(s_phut/10,2 );   hien_thi(s_phut%10,3 );     
  } 
   
  if ((gio == s_gio) && (phut == s_phut)  )
  { 
     on_coi;

  } 
  
  
  if ((gio != s_gio) && (phut != s_phut)){ off_coi; tam = 0;}
}
//-----------------------------------------------------------------
void hien_thi(char giatri ,char vt)
{  
 char tam = ((vt/2)+1);
       if (khoa[tam] == 0) {
        digitalWrite(D_RCk,LOW);
          if (vt <= 7 ){
            shiftOut(D_in, D_Sck, LSBFIRST , 0x00);
            shiftOut(D_in, D_Sck, LSBFIRST , vitri[vt]);}
          if (vt >  7 ){
            shiftOut(D_in, D_Sck, LSBFIRST , vitri[(vt-8)]);
            shiftOut(D_in, D_Sck, LSBFIRST , 0x00);
          }      
          shiftOut(D_in, D_Sck, MSBFIRST , led1[giatri]); 
          digitalWrite(D_RCk,HIGH);
          }
        delay(1);       
}
//-----------------------------------------------------------------
void xoa()
{     shiftOut(D_in, D_Sck, MSBFIRST , 0xff);
      shiftOut(D_in, D_Sck, MSBFIRST , 0xff);
      shiftOut(D_in, D_Sck, MSBFIRST , 0xff);
      digitalWrite(D_RCk,HIGH); digitalWrite(D_RCk,LOW);   
}
//-----------------------------------------------------------------
void set_s_gio()
{
  if (UP ==0)
  {
    on_coi;
    while (UP ==0);s_gio ++;
    off_coi; 
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);s_gio --;  
    off_coi;
  } 
  if (s_gio < 0) { s_gio = 23;}
  if (s_gio == 24) { s_gio = 0;}
    EEPROM.write(diachi_gio_bt, s_gio);
}
//-------------------------------------
void set_s_phut()
{
  if (UP ==0)
  { on_coi;
    while (UP ==0);s_phut ++;
    off_coi; 
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);s_phut --;  
    off_coi;
  } 
    if (s_phut < 0) { s_phut = 59;}
  if (s_phut == 60) { s_phut = 0;} 
  EEPROM.write(diachi_phut_bt, s_phut);
}
//-----------------------------------------------------------------
void set_gio()
{
  if (UP ==0)
  {
    on_coi;
    while (UP ==0);gio ++;
    off_coi; 
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);gio --;  
    off_coi;
  } 
  if (gio < 0) { gio = 23;}
  if (gio == 24) { gio = 0;}
  tm.Hour = gio;
  RTC.write(tm);  
}
//-----------------------------------------------------------------

void set_phut()
{
  if (UP ==0)
  { on_coi;
    while (UP ==0);phut ++;
    off_coi; 
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);phut --;  
    off_coi;
  } 
    if (phut < 0) { phut = 59;}
  if (phut == 60) { phut = 0;}
  tm.Minute = phut;
  RTC.write(tm);  
}
//-----------------------------------------------------------------
void set_giay()
{
  if (UP ==0)
  {
    on_coi;
    while (UP ==0);giay ++; 
    off_coi;
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);giay --;  
    off_coi;
  } 
  if (giay < 0) { giay = 59;}
  if (giay == 60) { giay = 0;}
  tm.Second = giay;
  RTC.write(tm);  
}
//-----------------------------------------------------------------
void set_ngay()
{
  if (UP ==0)
  {
    on_coi;
    while (UP ==0);ngay ++; 
    off_coi;
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);ngay --; 
    off_coi; 
  }
    
  if ((( thang <= 7 ) & ( thang % 2 == 1)& ( thang != 2))| ((thang >=8) & (thang % 2 == 0)))
      {  
        if (ngay == 32) { ngay = 1;}
        if (ngay < 1) { ngay = 30;}} 
       else 
       if ( thang == 2)
       {
        {if (ngay == 29) { ngay = 1;}
        if (ngay < 1) { ngay = 28;}}}
          else 
          {
          {if (ngay == 31) { ngay = 1;}
          if (ngay < 1) { ngay = 30;}}}
        
  tm.Day = ngay;
  RTC.write(tm);  
}
//-----------------------------------------------------------------
void set_thang()
{
  if (UP ==0)
  {
    on_coi;
    while (UP ==0);thang ++; 
    off_coi;
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);thang --; 
    off_coi; 
  } 
  if (thang > 12) {thang = 1;}
  if (thang <1 )   {thang = 12;}
  tm.Month = thang;
  RTC.write(tm);  
}
//-----------------------------------------------------------------
void set_nam()
{
  if (UP ==0)
  {
    on_coi;
    while (UP ==0);tm.Year ++;
    off_coi; 
  }
    if (DOWN ==0)
  {
    on_coi;
    while (DOWN ==0);tm.Year --;  
    off_coi;
  } 
  //tm.Year = nam;
  
  RTC.write(tm);  
}
//-----------------------------------------------------------------
void thoi_gian_()
{

  if (RTC.read(tm)) { 
    print2digits(tm.Hour);  
    print2digits(tm.Minute);   
    print2digits(tm.Second);
    Serial.print(tm.Day); 
    Serial.print(tm.Month);  
    Serial.print(tmYearToCalendar(tm.Year));
  }
  ngay = tm.Day ;
  thang = tm.Month;
  nam = tmYearToCalendar(tm.Year);
  gio = tm.Hour;
  phut = tm.Minute;
  giay = tm.Second;
}


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
//-----------------------------------------
