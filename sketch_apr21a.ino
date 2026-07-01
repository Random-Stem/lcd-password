#include <LiquidCrystal_I2C.h>
#include <Key.h>
#include <Keypad.h>
const byte ROWS = 4;
const byte COLOMS = 4;
char map1[ROWS][COLOMS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte PIN_ROW[ROWS] = {2,3,4,5};
byte PIN_COLOMS[COLOMS] =  {6,7,8,9};
Keypad keypad=Keypad(makeKeymap(map1),PIN_ROW ,PIN_COLOMS, ROWS ,COLOMS);
LiquidCrystal_I2C lcd(0x3F,16,2);
String password = "1111";
String input ="";
String sos = "0000";
 bool menuSOS=false;
 int now=0;
 int up=1;
 int center=0;
 int down=-1;
 static int lastpos=-99;
 bool exit1 = false;
 bool reset1=false;
 bool newpass=false;
 String pass1="";
 bool stay=false;
 bool games1=false;
 bool reset2=false;
 bool check=false;
 bool returnloop=false;
 bool rfid=false;
 int cactuspos=16;
int points=0;
bool gameover=false;
bool velocity=true;
bool updelay=false;
byte cactus[8] = {
  B00100,
  B00101,
  B10101,
  B10111,
  B11100,
  B00100,
  B00100,
  B00000
};
byte player[8] = {
  B00111,
  B00101,
  B00111,
  B01111,
  B11110,
  B01110,
  B01010,
  B01010
};
unsigned long startjump=0;
bool jump=false;
int b=300;
void setup() {
lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("password:");
pinMode(10,OUTPUT);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT_PULLUP);
Serial.begin(9600);
lcd.createChar(1, cactus);
lcd.createChar(2, player);
pinMode(11,INPUT_PULLUP);
}
void loop() {
char key = keypad.getKey();
if(returnloop==true){
  returnloop=false;
  check=false;
}
if(key){
  if(key=='#'){
    lcd.clear();
    if(input==password){
      input="                    ";
      lcd.setCursor(0,0);
      lcd.print("CORRECT");
      tone(10,500);
      delay(200);
      noTone(10);
      tone(10,700);
      delay(200);
      noTone(10);
      delay(100);
      lcd.clear();
      games1=true;
      games();
    }else if(input==sos) {
      lastpos=-99;
      input="";
      menuSOS=true;
      tone(10,200);
      delay(200);
      noTone(10);
      tone(10,400);
      delay(200);
      noTone(10);
      tone(10,200);
      delay(200);
      noTone(10);
    }else {
      input="                  ";
      lcd.setCursor(0,0);
      lcd.print("WRONG");
      tone(10,200);
      delay(200);
      noTone(10);
      tone(10,200);
      delay(200);
      noTone(10);
    }
    }
     else if(key=='*'){
      if(newpass==false){
      input="";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("password:");
     }
    }else {
      input +=key;
      lcd.setCursor(input.length()-1,1);
      lcd.print('*');
    }
  }
  if(returnloop==false){
   if (menuSOS==true){
    int a = analogRead(A0);
    if(a>700){
      now=up;
    }else if (a>700){
      now=center;
    }
    if(a<300){
      now=down;
    }
    if (now !=lastpos){
    if (now==center){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("menuSOS:");
     lcd.setCursor(0,1);
     lcd.print(">reset password");   
    }
      if(now==down){
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("reset password");
       lcd.setCursor(0,1);
       lcd.print(">exit"); 
      }
        if(now==up){
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("menuSOS:");
         lcd.setCursor(0,1);
         lcd.print(">reset password"); 
        }
      } if(now==up||now==center){
        if(analogRead(A2)<100){
        reset1=true;
        if(reset1==true){
         lcd.clear();
         input="";
         lcd.setCursor(0,0);
         lcd.print("RFID request");
         newpass=true;
        }
       }             
      } 
        if(now==down){
        if(analogRead(A2)<100){
        exit1=true;
        if(exit1==true){
         input="";
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("password:");
         lastpos=-99;
         menuSOS=false;
        }
       }
      }  
      if(newpass==true){
        rfid=true;
        if(rfid==true){
        input="                     ";
        lcd.setCursor(0,0);
        lcd.print("RFID request");
        }
        if(rfid==true){
        if(digitalRead(12)>0){
        rfid=false;
        reset2=true;
        input="";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("UID recognised");
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("new pass:");
        lcd.setCursor(0,1);
        lcd.print(input);
        changepass();
        return;
        }
       }
      }
  }else if(returnloop==true){
    return;
  }
      reset1=false;
      exit1=false;
      lastpos=now;
    }
  }
void changepass(){
if(reset2==true){
while(true){
char key = keypad.getKey();
if(returnloop==false){
if(key){
if(key=='*'){
  input="";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("new pass:");
  lcd.setCursor(0,1);
  lcd.print(input);
}else if(key=='#'){
  pass1=input;
  check=true;
  input="";
  stay=true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("password:");
  lcd.setCursor(0,1);
  lcd.print(input);
}else {
  input +=key;
  lcd.setCursor(input.length()-1,1);
  lcd.print('*');
}
while(stay==true){
  char key2=keypad.getKey();
  if(key2){
  if(key2=='*'){
  input="";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("password:");
  lcd.setCursor(0,1);
  lcd.print(input);
  }else if(key2=='#'){
  lcd.clear();
  if(input==pass1){
    input="                    ";
    lcd.setCursor(0,0);
    lcd.print("CORRECT");
    tone(10,500);
    delay(200);
    noTone(10);
    tone(10,700);
    delay(200);
    noTone(10);
    delay(100);
    lcd.clear();
    games1=true;
    games();
  }else{
    input="                  ";
    lcd.setCursor(0,0);
    lcd.print("WRONG");
    tone(10,200);
    delay(200);
    noTone(10);
    tone(10,200);
    delay(200);
    noTone(10); 
  }
}else{
  input +=key2;
  lcd.setCursor(input.length()-1,1);
  lcd.print('*');
  }
if(key2=='*'&&analogRead(A2)<100){
  input="";
  lcd.clear();
  menuSOS=false;
  now=0;
  up=1;
  center=0;
  down=-1;
  lastpos=-99;
  exit1 = false;
  reset1=false;
  newpass=false;
  pass1="";
  stay=false;
  games1=false;
  reset2=false;
  check=false;
  returnloop=false;
  cactuspos=16;
  points=0;
  gameover=false;
  pass1=password;
  lcd.setCursor(0,0);
  lcd.print("password: ");
  lcd.setCursor(0,1);
  lcd.print(input);
  returnloop=true;
  return;
}
}
}
}
}else if(returnloop==true){
  return;
}
}
}
}
void games(){
if(games1==true){
lcd.setCursor(0,0);
lcd.print("games:");
lcd.setCursor(0,1);
lcd.print(">Dino game");
while(true){
 char key=keypad.getKey();
 int a = analogRead(A0);
 int b=analogRead(A2);
  if(a>700){
    now=up;
  }else if (a>700){
   now=center;
  }
  if(a<300){
   now=down;
 }
 if(now !=lastpos){
  if(now==center){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("games:");
    lcd.setCursor(0,1);
    lcd.print(">Dino game");
  }
  if(now==down){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dino game");
    lcd.setCursor(0,1);
    lcd.print(">music K.525");
  }
  if(now==up){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("games:");
    lcd.setCursor(0,1);
    lcd.print(">Dino game"); 
  }
 }
 if(key=='#'){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("password:");
   lcd.setCursor(0,1);
   lcd.print(input);
   input="";
   if(check==true){
   stay=true;
   }
   lastpos=-99;
   return;
 }
 if(b<100){
  if(now==up||now==center){
    games1=false;
    dino();
  }
 }
  lastpos=now;
}
}
}
void dino(){
  lcd.clear();
while(true){
  char key=keypad.getKey();
  unsigned long mills=millis();
  int a=digitalRead(11);
  points++;
  cactuspos--;
  Serial.println(b);
  if(cactuspos<0){
  cactuspos=16;
}
if(velocity==true){
  b--;
}
if(b<50){
  velocity=false;
  updelay=true;
}
if(updelay==true){
  b++;
}
if(b>300&&velocity==false){
  velocity=true;
  updelay=false;
}
if(a==0){
  if(startjump==0||jump==false&&(mills-startjump>=600)){
  startjump=mills;
}
}else{
  jump=false;
}
if( mills - startjump>=400){
  jump=false;
}
if(mills-startjump<400){
  jump=true;
}
if(jump==false){
  lcd.setCursor(6,0);
  lcd.print(" ");
  lcd.setCursor(6,1);
  lcd.write(byte(2));
}
if(jump==true){
  lcd.setCursor(6,1);
  lcd.print(" ");
  lcd.setCursor(6,0);
  lcd.write(byte(2));
}
lcd.setCursor(0,0);
lcd.print(points);
lcd.setCursor(cactuspos,1);
lcd.write(byte(1));
delay(b);
lcd.setCursor(cactuspos,1);
lcd.print(" ");
if(jump==false&&cactuspos==6){
lcd.clear();
while(true){
  int a=digitalRead(11);
  if(a==0){
  gameover=true;
}
  lcd.setCursor(0,0);
  lcd.print("   Game Over");
  lcd.setCursor(0,1);
  lcd.print("   press jump"); 
  if(gameover==true){
    points=0;
    b=300;
    cactuspos=16;
    startjump=0;
    lcd.clear();
    gameover=false;
    break;
  }
}
}
  if(key=='#'){
   games1=false;
   stay=false;
   reset1=false;
   exit1=false;
   newpass=false;
   menuSOS=false;
   lcd.clear();
   input="";
   lcd.setCursor(0,0);
   lcd.print("games:");
   lcd.setCursor(0,1);
   lcd.print(">Dino game");
   return;
 }
}
} 