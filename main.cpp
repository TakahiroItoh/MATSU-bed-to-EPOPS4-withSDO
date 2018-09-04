//-------------------------------------------------------
//for MATSU-bed to EPOS4 with "SDO" communication
//operating mode : "Profile Velocity Mode"
//CANで速度指令モードを選択して指定の回転数で回すプログラム
//Created by Takahiro Itoh
//-------------------------------------------------------
#include "mbed.h"
#include "USBSerial.h"

#define LED1 P0_29
#define LED2 P0_28
#define LED3 P0_27
#define LED4 P0_26

USBSerial pc;
CANMessage canmsgTx;
CAN canPort(P0_13, P0_18);//CAN name(PinName rd, PinName td)

BusOut myled(LED1, LED2, LED3, LED4);

//send関数の例
//Operating Mode
//0x2F-6060-00-03-//-//-//
void sendOPMode(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 5;       //Data Length
    canmsgTx.data[0] = 0x2F;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x60;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByate
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x03;//data:0x03 = "Profile Velocity Mode" 
    /*
    canmsgTx.data[5] = 0x00;//data:(user value)
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);
    pc.printf("ok\r\n");
}

//プロトタイプ宣言
//Control Word
void sendCtrlSD(int);       //Shutdown
void sendCtrlEN(int);       //Switch on & Enable
void sendCtrlQS(int);       //Quick Stop
//Velocity Setting
void sendTgtVel(int,int);   //Target Velocity

int main(){
    int node1 = 1;
    int rpm = 1000;
    myled = 0b0001;
    while(pc.readable() == 0);
    pc.printf("KEY DETECTED!!\r\nPROGRAM START\r\n");
    //オペレーティングモードを送信
    pc.printf("Send Operating Mode\r\n");
    sendOPMode(node1);
    myled = 0b0011;
    wait(0.5);
    //Shutdown,Enableコマンド送信｜リセット
    pc.printf("Send Shutdown Command\r\n");
    sendCtrlSD(node1);
    wait(0.5);
    pc.printf("Send SW on & Enable Command\r\n");
    sendCtrlEN(node1);
    myled = 0b0111;
    wait(0.5);
    //目標速度を送信後、Enableコマンド送信
    pc.printf("Send Target Velocity\r\n");
    sendTgtVel(node1,rpm);
    wait(0.5);
    pc.printf("Send SW on & Enable Command\r\n");
    sendCtrlEN(node1);
    myled = 0b1111;
    wait(0.5);
    //10秒後Quick Stop
    wait(10);
    pc.printf("send Quick stop commmand\r\n");
    sendCtrlQS(node1);
    myled = 0b0000;
}

//0x2F-6040-00-0006-//-//
void sendCtrlSD(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByate
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x06;//data:0x00"06" = "Controlword(Shutdown)" 
    canmsgTx.data[5] = 0x00;//data:0x"00"06
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);
    pc.printf("ok\r\n");
}

//0x2F-6040-00-000F-//-//
void sendCtrlEN(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByate
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x0F;//data:0x00"0F" = "Controlword(Enable)" 
    canmsgTx.data[5] = 0x00;//data:0x"00"0F
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);
    pc.printf("ok\r\n");
}

//0x2F-6040-00-000B-//-//
void sendCtrlQS(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByate
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x0B;//data:0x00"0B" = "Quick Stop" 
    canmsgTx.data[5] = 0x00;//data:0x"00"0B
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);
    pc.printf("ok\r\n");
}

//0x2F-60FF-00-03E8-//-//
void sendTgtVel(int nodeID,int rpm){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0xFF;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByate
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0xE8;//data:0x03E8 = "Target Velocity Value" 
    canmsgTx.data[5] = 0x03;//data:
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);
    pc.printf("ok\r\n");
}