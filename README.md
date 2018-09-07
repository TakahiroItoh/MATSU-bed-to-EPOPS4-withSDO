# MATSU-bed-to-EPOPS4-withSDO
MATSU-bedでCANopenのSDO Objectを用いてEPOS4の速度制御を実行する

MATSU-bed<br>
https://os.mbed.com/teams/MATSU-bed/wiki/MATSU-bed-config

#### このプログラムの指令の流れ<br>
1.sendOPMode関数でオペレーティングモードをProfile Velocity Modeに変更
2.sendCtrlRS関数でコントロールワードをリセット
3.sendCtrlSD関数、sendCtrlEN関数でEPOS4をリセット(Shutdown,Switch on)
4.sendTgtVel関数で目標回転数を指定
5.sendCtrlEN関数で回転開始(Enable)
6.sendCtrlQS関数で回転停止(Quick stop)
