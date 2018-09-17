# MATSU-bed-to-EPOPS4-withSDO
MATSU-bedでCANopenのSDO Communicationを用いてEPOS4の速度制御を実行する

MATSU-bed<br>
https://os.mbed.com/teams/MATSU-bed/wiki/MATSU-bed-config

### このプログラムの指令の流れ<br>
1. 's'でプログラムスタート<br>
2. Profile Velocity Mode(PVM)に設定される<br>
3. 送信コマンド選択ループ開始<br>
    't' : プログラム中に指定された回転速度で回転開始<br>
    'h' : Halt(一時停止)コマンド<br>
    'v' : Actual Velocityを尋ねるコマンド<br>
    'q' : Quick Stop,Shutdownコマンド｜プログラム終了<br>
    もし動かないとき：PVMの設定がコケてることがあります。<br>
    'm' : PVMを再設定
