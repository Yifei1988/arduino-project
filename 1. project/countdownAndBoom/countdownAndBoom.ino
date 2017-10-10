//*******************************************************************************
//-------------------------Dortmund---09.10.2017(Montag)-------------------------
//-------------------------------------------------------------------------------
//--------------开发环境：Arduino IDE---------------------------------------------
//--------------开发板型号：Arduino UNO R3 + Multi function Shield---------------
//--------------实现目的：调用多功能扩展板上7-Segment-LED倒计时10秒，------------
//------------------------在计时终了时启动蜂鸣器并闪烁显示“byE!”---------------
//--------------作者：Yifei1988--------------------------------------------------
//*******************************************************************************/

int buzzer = 3; //设置控制蜂鸣器的数字IO脚
int latchPin = 4; //定义shiftOut调用的三个Pin的脚
int clockPin = 7;
int dataPin = 8; 
int LED1 = 13; //定义4个7-Segment-LED的脚
int LED2 = 12;
int LED3 = 11;
int LED4 = 10;

int timeNow = 10; //倒计时从10秒开始
int timeCount = 0; //已经流逝的时间从0秒开始计数

unsigned char Dis_table[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90}; //每个7-Segment-LED对应显示0～9的16进制编码
unsigned char Dis_buf[] = {0xF1,0xF2,0xF4,0xF8}; //16进制编码,对应4个7-Segment-LED的接口
unsigned char disbuff[] = {0, 0, 1, 0}; //7-Segment-LED初始显示值

void setup ()
{
  pinMode(buzzer,OUTPUT); //设置蜂鸣器IO脚为输出状态
  pinMode(latchPin,OUTPUT); //设置3个Pin脚都是输出状态
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(LED1,OUTPUT); //设置4个7-Segment-LED脚都是输出状态
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  
  LED1=0;
  LED2=0;
  LED3=0;
  LED4=0; 
}

void display()
{
  for(char i=0; i<=3; i++) //扫描4个7-Segment-LED管
  {
    digitalWrite(latchPin,LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_table[disbuff[i]]); //给一个7-Segment-LED发送对应的数字显示编码，串行数据输入引脚为dataPin，时钟引脚为clockPin，执行MSB有限发送，发送数据Dis_table[i]
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_buf[i] );  //发送到对应的 7-Segment-LED接口
    digitalWrite(latchPin,HIGH); //将ST_CP这个针脚恢复到高电平
    if(timeNow != 0){
      delay(2); //延时2ms  
    }
  }
}

void displayBye()
{
  //unsigned char Dis_tableHallo[] = {0x89,0x88,0xC9,0XC0}; //显示H,A,ll,O，弃用
  unsigned char Dis_tableHallo[] = {0x83,0x91,0x86,0X7D}; //显示b,y,E,!
  for(char i=0; i<=3; i++) //扫描4个7-Segment-LED管
  {
    digitalWrite(latchPin,LOW); //将ST_CP口上面加低电平让芯片准备好接收数据
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_tableHallo[i]);  //给一个7-Segment-LED发送对应的字符显示编码，串行数据输入引脚为dataPin，时钟引脚为clockPin，执行MSB有限发送，发送数据Dis_table[i]
    shiftOut(dataPin,clockPin,MSBFIRST,Dis_buf[i] );  //发送到对应的 7-Segment-LED接口
    digitalWrite(latchPin,HIGH); //将ST_CP这个针脚恢复到高电平
    if(timeNow != 0){
      delay(2); //延时2ms  
    }
  }
}

void countdown()
{
  delay(1);
  timeCount++;
  if(timeNow > 0 && timeCount%100 == 0)
  {
    timeNow--;
  }
  disbuff[2] = timeNow/10;
  disbuff[3] = timeNow%10;
}

void loop()
{
  if(timeNow == 0) //倒计时结束时
  { 
    for(char i=0;i<100;i++) //选定一个频率输出声音
    { 
      digitalWrite(buzzer,HIGH); //发声音
      delay(1); //延时1ms
      displayBye();//防止频闪
      digitalWrite(buzzer,LOW); //不发声音
      delay(1); //延时ms 
      }
      delay(100); //间歇发声
  }
  else{
    display();
    countdown();
  }
}
