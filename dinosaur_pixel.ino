#include <LiquidCrystal.h> // Adiciona a biblioteca "LiquidCrystal" ao projeto

LiquidCrystal lcd(12, 13, 7, 6, 5, 4); // Pinagem do LCD

int PORTA_LED = 9;


const int SW_pin = 2; // digital pin connected to SW
const int X_pin = 0; // analog pin connected to VRx
const int Y_pin = 1; // analog pin connected to VRy

//player
int playerX = 2;
int playerY = 1;

//analog
bool analog_upped = false;

//obstacles 
int obstacleX[] = {18, 24};

//timer
long timer = 0;
long jumpTimer = 0;
int mainTimer = 300;

bool jump = false;

bool gameover = false;

//the count of game points
int score = 0;


void setup() {
  //led and bip
  pinMode(PORTA_LED, OUTPUT);

  //analogico
  pinMode(X_pin, INPUT);

  //lcd
  lcd.begin(16, 2); // Inicia o lcd de 16x2

  //serial send info
  Serial.begin(9600);
  Serial.write('S');
}

void loop() {
  //Serial.print(jump == true);
  //digitalWrite(PORTA_LED, HIGH);         
  
  //!analogRead(X_pin) - return HIGH when the joystick is up
  digitalWrite(PORTA_LED, !analogRead(X_pin));

  if(!analogRead(X_pin) == HIGH) {
    if(analog_upped == false) {
      analog_upped = true;
      if(gameover == true) { //reset game
        playerY = 1;
        obstacleX[0] = 18;
        obstacleX[1] = 24;
        gameover = false;
        Serial.write('S');
      } else {
        if(jump == false) {
          playerY = 0;
          jump = true;
          lcd.clear();
          updateDraw();
        }
      }
    }
  } else {
    analog_upped = false;
  }

  //collision
  if(jump == false && playerX == obstacleX[0] + 1 && playerY == 1 || jump == false && playerX == obstacleX[1] + 1 && playerY == 1) {
    digitalWrite(PORTA_LED, HIGH);
    lcd.clear();
    drawText(playerX, playerY, 'X');
    gameover = true;
    //Serial.write('s');
    
    score = 0;
    mainTimer = 300;
  }

  if(gameover == false) {
    //jumpping
    unsigned long mil = millis();
    if(mil - jumpTimer > 600 && jump == true) {
      jumpTimer = mil;
      
      playerY = 1;
      jump = false;
      lcd.clear();
      updateDraw();

      
      score += 1;
    }
  
    if(mil - timer > mainTimer){
      if(score % 10 == 0 && mainTimer > 100) { //most fast
        mainTimer -= 10;
      }
      
      timer = mil;
  
      //limpa lcd
      lcd.clear();
      
      updateDraw();
    
      //obstacle pos
      obstacleX[0]--;
      if(obstacleX[0] < 0) {
        obstacleX[0] = 17;
      }

      obstacleX[1]--;
      if(obstacleX[1] < 0) {
        obstacleX[1] = 23;
      }
    }
  }
}

void drawText(int x, int y, char text) {
  lcd.setCursor(x, y);
  lcd.print(text); 
}

void updateDraw() {
  //player
  drawText(playerX, playerY, 'o');
  //obstacles
  drawText(obstacleX[0], 1, '<');
  drawText(obstacleX[1], 1, '<');
  //delay(200);
  
}
