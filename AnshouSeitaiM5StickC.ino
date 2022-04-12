#include <M5StickC.h>
#include "finger.h"
uint8_t userNum;           //User number
FingerPrint FP_M;


char password[4] = { 0 , 0 , 0 , 0 };
enum STATE
{
  SELECT1 = 0,
  SELECT2 = 1,
  SELECT3 = 2,
  SELECT4 = 3,
  COMP_FINGER = 4,
  COMPLETE = 5
};

STATE state = SELECT1;
bool result = false;

void Disp()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(20, 10);
  M5.Lcd.setTextFont(4);
  M5.Lcd.printf("%d    %d    %d    %d",password[0],password[1],password[2],password[3]);

  if((state >= SELECT1) && (state <= SELECT4)){
    M5.Lcd.setCursor(5 + 34 * state, 10);
    M5.Lcd.printf(">");
  }
  else if(state == COMP_FINGER)
  {
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(18, 34);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.printf("matching...");
  }
  else if(state == COMPLETE)
  {
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(56, 38);
    if(result){
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.printf("OK");
    }else{
      M5.Lcd.setTextColor(RED);
      M5.Lcd.printf("NG");
    }
  }
}

void setup() {
	M5.begin();
	Serial2.begin(19200, SERIAL_8N1, 26, 0);
	M5.Lcd.setRotation(3);
	//M5.Lcd.clear(BLACK);
	//M5.Lcd.setTextColor(YELLOW);
  Disp();
}

//ButtonA: Add user
//ButtonB: Matching
void StateNext()
{
  if(state ==  SELECT1) state = SELECT2;
  else if(state == SELECT2) state = SELECT3;
  else if(state == SELECT3) state = SELECT4;
  else if(state == SELECT4) state = COMP_FINGER;
  else if(state == COMP_FINGER) state = COMPLETE;
  else if(state == COMPLETE) state = SELECT1;

  return;
}

void loop(){
  if(M5.BtnA.wasPressed()){
    StateNext();
    Disp();
    if(state == COMP_FINGER)
    {
      uint8_t res1;
      result = false;
      res1 = FP_M.fpm_compareFinger();
      if(res1 == ACK_SUCCESS){
        if(password[0] == 1 && 
           password[1] == 2 && 
           password[2] == 3 && 
           password[3] == 4){ 
          result = true;
        }
      }
      if(res1 == ACK_NOUSER){
        result = false;
      }
      if(res1 == ACK_TIMEOUT){
        result = false;
      }
      StateNext();
      Disp();
    }
  }else if(M5.BtnB.wasPressed()){
    if((state >= SELECT1) && (state <= SELECT4)){
      password[state] = (password[state] + 1) % 10;
    }
    Disp();
  }
  
  /*
	uint8_t res1;
	  
	if(M5.BtnB.wasPressed()){
	  CleanScreen();
	  M5.Lcd.println("Matching");
	  
	  res1 = FP_M.fpm_compareFinger();
	  if(res1 == ACK_SUCCESS){
		  M5.Lcd.println("Success");
	  }
	  if(res1 == ACK_NOUSER){
		  M5.Lcd.println("No Such User");
	  }
	  if(res1 == ACK_TIMEOUT){
		  M5.Lcd.println("Timeout");
	  }
	}
 */

	M5.update();
}
