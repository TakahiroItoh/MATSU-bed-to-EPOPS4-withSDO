//-------------------------------------------------------
//for MATSU-bed to EPOS4 with "SDO" communication
//operating mode : "Profile Velocity Mode"
//CANで速度指令モードを選択して指定の回転数で回すプログラム
//Created by Takahiro Itoh
//-------------------------------------------------------
/*
//CANで送っているsend関数の例
//Operating Modeの設定
void sendOPMode(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 5;       //Data Length
    canmsgTx.data[0] = 0x2F;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x60;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x03;//data:0x03 = "Profile Velocity Mode"
//-------------------------------------------------------
    canmsgTx.data[5] = 0x00;//data:(user value)
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
//-------------------------------------------------------
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}
*/
#include "mbed.h"
#include "USBSerial.h"

#define LED1 P0_29
#define LED2 P0_28
#define LED3 P0_27
#define LED4 P0_26

USBSerial pc;
char Serialdata;
BusOut myled(LED1, LED2, LED3, LED4);

CANMessage canmsgTx;
CAN canPort(P0_13, P0_18);  //CAN name(PinName rd, PinName td)

//プロトタイプ宣言
//------------------send関数-------------------
//mode Setting
void sendOPMode(int);       //Operating Mode
//Control Word
void sendCtrlRS(int);       //Reset
void sendCtrlSD(int);       //Shutdown
void sendCtrlEN(int);       //Switch on & Enable
void sendCtrlQS(int);       //Quick Stop
void sendCtrlHL(int);       //Halt
//Velocity Setting
void sendTgtVel(int,int);   //Target Velocity
//------------------read関数-------------------
void readActVel(int);       //Actual Velocity
//-------------------その他--------------------
void printCANmsg(void);     //CAN送信データをPCに表示
void SerialRX(void);         //Serial受信処理

int main(){
    //Serial
    pc.baud(9600);  //Baud Rate:9600
    pc.attach(SerialRX,Serial::RxIrq);
    //CAN
    canPort.frequency(1000000); //Bit Rate:1MHz
    int node1 = 1;  //CAN node Setting
    //User Setting
    int rpm = 1000; //Velocity Setting[rpm]
    myled = 0b0001;
    while(pc.readable() == 0);
    Serialdata = 0;
    pc.printf("KEY DETECTED!!\r\nPROGRAM START\r\n");
    wait(0.5);
    //-------------起動時に必ず送信---------------
    //オペレーティングモードを送信
    pc.printf("Send Operating Mode\r\n");
    sendOPMode(node1);
    myled = 0b0011;
    wait(0.1);
    //コントロールワードのリセット
    pc.printf("Send Reset Command\r\n");
    sendCtrlRS(node1);
    wait(0.1);
    //Shutdown,Enableコマンド送信｜リセット
    pc.printf("Send Shutdown Command\r\n");
    sendCtrlSD(node1);
    wait(0.1);
    pc.printf("Send SW on & Enable Command\r\n");
    sendCtrlEN(node1);
    myled = 0b0111;
    wait(0.1);
    //-------------------------------------------
    while(1){
        //-------------送信コマンドを選択--------------
        if(Serialdata == ){
            //目標速度を送信後、Enableコマンド送信
            pc.printf("Send Target Velocity\r\n");
            sendTgtVel(node1,rpm);
            myled = 0b1111;
        }
        else if(Serialdata == ){
            //Haltコマンド送信
            pc.printf("Send Halt Command\r\n");
            sendCtrlHL(node1);
            myled = 0b0111;
        }
        else if(Serialdata ==){
            //quick stopコマンド送信
            pc.printf("Send Quick Stop\r\nPROGRAM END\r\n");
            sendCtrlQS(node1);
            break;
        }
        else if(Serialdata ==){
            //Actual Velocityを尋ねる
            pc.printf("Read Actual Velocity\r\n", );
            readActVel(node1);
        }
        //-------------------------------------------
        Serialdata = 0;
    }
    myled = 0b0000;
}

//0x2F-6060-00-03-//-//-//
void sendOPMode(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 5;       //Data Length
    canmsgTx.data[0] = 0x2F;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x60;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x03;//data:0x03 = "Profile Velocity Mode"
    /*
    canmsgTx.data[5] = 0x00;//data:(user value)
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}

//0x2B-6040-00-0000-//-//
void sendCtrlRS(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x80;//data:0x00"80" = "Controlword(Shutdown)"
    canmsgTx.data[5] = 0x00;//data:0x"00"80
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}

//0x2B-6040-00-0006-//-//
void sendCtrlSD(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x06;//data:0x00"06" = "Controlword(Shutdown)"
    canmsgTx.data[5] = 0x00;//data:0x"00"06
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}

//0x2B-6040-00-000F-//-//
void sendCtrlEN(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x0F;//data:0x00"0F" = "Controlword(Enable)"
    canmsgTx.data[5] = 0x00;//data:0x"00"0F
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}

//0x2B-6040-00-000B-//-//
void sendCtrlQS(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x0B;//data:0x00"0B" = "Quick Stop"
    canmsgTx.data[5] = 0x00;//data:0x"00"0B
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}

//0x2B-6040-00-010F-//-//
void sendCtrlHL(int nodeID){
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 6;       //Data Length
    canmsgTx.data[0] = 0x2B;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0x40;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    canmsgTx.data[4] = 0x0F;//data:0x01"0F" = "Halt"
    canmsgTx.data[5] = 0x01;//data:0x"01"0F
    /*
    canmsgTx.data[6] = 0x00;//data:(user value)
    canmsgTx.data[7] = 0x00;//data:(user value)
    */
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
}

//0x2B-60FF-00-[user data(4Byte)]
void sendTgtVel(int nodeID,int rpm){
    pc.printf("%drpm|0x%08x\r\n",rpm,rpm);  //回転数送信データの表示
    canmsgTx.id = 0x600+nodeID;
    canmsgTx.len = 8;       //Data Length
    canmsgTx.data[0] = 0x23;//|0Byte:40|1Byte:2F|2Byte:2B|4Byte:23|other:22|
    canmsgTx.data[1] = 0xFF;//Index LowByte
    canmsgTx.data[2] = 0x60;//Index HighByte
    canmsgTx.data[3] = 0x00;//sub-Index
    //下位から1Byteずつdataに格納
    for(char cnt=4;cnt<8;cnt++){
        canmsgTx.data[cnt] = rpm % 256;
        rpm = rpm / 256;
    }
    canPort.write(canmsgTx);//CANでデータ送信
    printCANmsg();          //CAN送信データをPCに表示
    wait(0.1);
    //send Enable
    pc.printf("Send Enable Command\r\n");
    sendCtrlEN(nodeID);
    wait(0.1);
}

void readActVel(int nodeID){

}

//送信データの表示
void printCANmsg(void){
  //0x canID|Byte0|Byte1|Byte2|Byte3|Byte4|Byte5|Byte6|Byte7|
    pc.printf("0x%3x|",canmsgTx.id);
    for(char i=0;i < canmsgTx.len;i++){
        pc.printf("%02x|",canmsgTx.data[i]);
    }
    pc.printf("\r\n");
}

void SerialRX(void){
    Serialdata = pc.getc();
}
