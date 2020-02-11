#include <SPI.h>
#include <TFT.h>

// lcd 핀 정의하기 
#define cs   10  //CS or Chip Select
#define dc   9   //DC or A0, Data/Command
#define rst  8   //Reset

//조이스틱 핀 정의
#define joyX 0  //x축
#define joyY 1  //y축
#define joyZ 7  //z축

//맵 크기 정의
#define myWidth 120  //y축
#define myHeight 150  //z축

// 맵 좌표
#define maxX 12
#define maxY 16

// 빈곳, 현재벽돌, 굳은 벽돌
#define noBlock 0
#define PosBlock 1
#define onBlock 2

// 플레이어 좌표
int posX;
int posY;

//무작위 수
int randBlock; 
int updown;

//tft구조체 선언
TFT TFTscreen = TFT(cs, dc, rst);

// 점수
int preScore, posScore;
char scoreArray[3]; 
String sVal;

// 맵 위지 정의(배열)
char changeMap1[16][12]={0};
char orgMap[16][12];

// 블록 생성 상태
bool blockState;

// 블록 선언
bool block[7][4][4][4] = 
{
  // 첫 번째 블록■■■■
  {{
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 0, 0 }
  } },

  // ■■
  // ■■ 두 번째 블록
  {{
    { 1, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  }},

  //   ■
  // ■■■ 세 번째 블록
  {{
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 1, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 0, 0 }
  }},

  //    ■
  // ■■■ 네 번째 블록
  {{
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 1, 1, 0 },
    { 1, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 }
  }},

  // ■
  // ■■■ 다섯 번째 블록
  { {
    { 0, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 1, 1, 0, 0 },
    { 1, 0, 0, 0 },
    { 1, 0, 0, 0 },
    { 0, 0, 0, 0 }
  } },

  //   ■■
  // ■■    여섯 번째 블록
  { {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
  } },

  // ■■
  //   ■■ 일곱 번째 블록
  { {
    { 0, 0, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 0, 0 },
    { 1, 1, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 0, 0 }
  },
  {
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 0, 0 }
  } }
};

void setup() {
  Serial.begin(9600);
  //digital 핀모드 초기화
  pinMode(joyZ, INPUT_PULLUP);
  
  //플레이어 좌표 초기화 2+1
  posX = 3;
  posY = -3;

  //블록 상태 초기화
  blockState = false;
  
  //TFT 클래스 시작
  TFTscreen.begin();

  //각도 회전
  TFTscreen.setRotation(2);
}

void loop() {
  GameStart();
  Lanking();
}

void testDraw(){
  for(int i =0; i<maxY-1; i++){
    for(int j=1; j<maxX-1; j++){
      TFTscreen.stroke(255, 0, 0);
      TFTscreen.fill(0, 0, 0);
      TFTscreen.rect(j*10-10, i*10, 10, 10);
    }
  }
}

void GameStart() {
  blockState = false;
  Mapping();
  newBlock();
  
  while(true){
    for(int i =0; i<5; i++) {
      drawScrean();
      moveBlock();
      delay(50);
      if(!WallBlock(posX, posY+1)) {
        delay(50);
      }
      if(analogRead(joyY) < 20){
        downBlock();
        delay(50);
        break;
      }
    }
    
    dropBlock();
    lineClear();
    if(gameOver()){
      break;
    }
    newBlock();
  }
}

void Lanking(){
  // 검정색으로 초기화 
  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(0, 0, 255);
  TFTscreen.setTextSize(3);
  TFTscreen.text("End",30, myHeight/2);
  
  while(true){
    if(digitalRead(joyZ) == 0)
      break;
  }
}

void Mapping() {
  for(int i=0; i<maxY; i++){
    for(int j=0; j<maxX; j++){
      orgMap[i][j] = 0;
      changeMap1[i][j] = 0;
    }
  }
  // 검정색으로 초기화 
  TFTscreen.background(0, 255, 0);
  // 폰트 칼라 설정 (흰색)
  TFTscreen.stroke(0, 255, 255);
  // 폰트 크기를 1로 설정 
  TFTscreen.setTextSize(1);
  // 파란색으로 변경 
  TFTscreen.stroke(255, 0, 255);
  // 텍스트 쓰기 
  TFTscreen.text("Tetris World",0, myHeight);
  // 맵 그리기
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.fill(0, 0, 0);
  TFTscreen.rect(0, 0, myWidth-20, myHeight);

  //좌우 벽을 만듦
  for(int i=0; i<maxY; i++){   
    orgMap[i][0]=2;
    orgMap[i][maxX-1]=2;
  }
  //바닥벽을 만듦 
  for(int j=0; j<maxX; j++){ 
    orgMap[maxY-1][j]=2;
  }
  //changeMap 초기화
  for(int i=0; i<maxY; i++){
    for(int j=0; j<maxX; j++){
      changeMap1[i][j] = orgMap[i][j];    
    }
  }
  posScore = 0;
  preScore = 0;
}

void newBlock() {
  if(blockState == false) {
    posY=-3;
    posX=3;
  
    randBlock = rand()%7;  
    updown = 0;//rand() % 4;
    
    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
        if(block[randBlock][updown][i][j] == 1 && posY>=0) 
          orgMap[i+posY][j+posX] = 1;
      }
    }
  }
  blockState = true;
}

void moveBlock() {
  int value = analogRead(joyX);
  
  if(value >700){
    if(WallBlock(posX-1, posY)){
      for(int i= 0; i<4; i++) {
        for(int j=0; j<4; j++) {
          if(block[randBlock][updown][i][j] == 1) 
            orgMap[i+posY][j+posX] = 0;
        }
      }

      posX -= 1;
      for(int i= 0; i<4; i++) {
        for(int j=0; j<4; j++) {
          if(block[randBlock][updown][i][j] == 1) 
            orgMap[i+posY][j+posX] = 1;
        }
      }
    } 
  }
  else if(value < 200) {
    if(WallBlock(posX+1, posY)){
      for(int i= 0; i<4; i++) {
        for(int j=0; j<4; j++) {
          if(block[randBlock][updown][i][j] == 1) 
            orgMap[i+posY][j+posX] = 0;
        }
      }

      posX += 1;
      for(int i= 0; i<4; i++) {
        for(int j=0; j<4; j++) {
          if(block[randBlock][updown][i][j] == 1) 
            orgMap[i+posY][j+posX] = 1;
        }
      }
    }
  }
  
  int value2 = digitalRead(joyZ);
  if(value2 == 0){
    for(int i= 0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 0;
      }
    }

    int temp = updown;
    updown++;
    updown = updown % 4;
    if(!WallBlock(posX, posY)){
      updown = temp;
    }
    
    for(int i= 0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 1;
      }
    }
  }
}

void drawScrean() {
  int key = 0;
  for(int i =0; i<maxY-1; i++){
    for(int j=1; j<maxX-1; j++){
      if(changeMap1[i][j] != orgMap[i][j]){
        key=orgMap[i][j];
        switch (key){
          case 0:
              TFTscreen.stroke(0, 0, 0);
              TFTscreen.fill(0, 0, 0);
              TFTscreen.rect(j*10-10, i*10, 10, 10);
              break;
          case 1:
              TFTscreen.stroke(255, 0, 0);
              TFTscreen.fill(0, 0, 0);
              TFTscreen.rect(j*10-10, i*10, 10, 10);
              break;
          case 2:
              TFTscreen.stroke(255, 255, 255);
              TFTscreen.fill(50, 50, 50);
              TFTscreen.rect(j*10-10, i*10, 10, 10);
              break;
        }
      }
    }
  }

  //점수
  if(posScore != preScore || posScore == 0){
    TFTscreen.stroke(0, 255, 0);
    TFTscreen.fill(0, 255, 0);
    TFTscreen.rect(105, 90, 10, 20);
    
    TFTscreen.stroke(255, 0, 255);
    TFTscreen.text("LP",105, 80);

    sVal = String(posScore);
    sVal.toCharArray(scoreArray, 4);
    TFTscreen.text(scoreArray, 105, 90);

    preScore = posScore;
  }
  
  for(int i =0; i<15; i++){
    for(int j=1; j<11; j++){
      changeMap1[i][j] = orgMap[i][j];
    }
  }
}

bool WallBlock(int m_posX, int m_posY) {
  for(int i= 0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(orgMap[i+m_posY][j+m_posX] == 2 && block[randBlock][updown][i][j] == 1)
        return false;
    }
  }
  return true;
}

void downBlock(){
  for(int i= 0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 0;
    }
  }

  while(WallBlock(posX, posY+1)){
    posY += 1;
  }
  
  for(int i= 0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 1;
      }
    }
}

void dropBlock(){
  for(int i= 0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 0;
    }
  }

  if(WallBlock(posX, posY+1)){
    posY += 1;
    for(int i= 0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 1;
      }
    }
  }
  else {
    for(int i= 0; i<4; i++) {
      for(int j=0; j<4; j++) {
        if(block[randBlock][updown][i][j] == 1) 
          orgMap[i+posY][j+posX] = 2;
      }
    }
    blockState = false;
  }
}

bool gameOver(){
  for(int i=1; i<11; i++){
    if(orgMap[2][i] == 2){
      drawScrean();
      TFTscreen.stroke(0, 0, 255);
      TFTscreen.setTextSize(2);
      TFTscreen.text("Game",30, myHeight/2);
      TFTscreen.text("Over",30, myHeight/2+20);
      delay(3000);
      
      return true;
    }
  }
  return false;
}

void lineClear(){
  for(int i=2; i<maxY-1; i++) {
    for(int j=1; j<maxX-1; j++) {
      if(orgMap[i][j] != 2) 
        break;
      
      if(j == maxX-2) {
        // 그줄의 값을 0으로 초기화
        for(int e=1; e<maxX-1; e++) {
          orgMap[i][e] == 0;
        }
        // 각 칸의 값들을 하나씩내림
        for(int k=i; k > 1; k--) {
          for(int s=1; s<maxX-1; s++) {
            orgMap[k][s] = orgMap[k-1][s];
            orgMap[k-1][s] = 0;
          }
        }
        posScore++;
      }
      
    }
  }
}
