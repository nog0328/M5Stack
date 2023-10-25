#include <M5Stack.h>

#define HR_pin 35 // 出力信号線（真ん中の線）をGPIO 35につなぐ（36も可）
                  // 半固定抵抗の両端の線をG（グランド）と3.3V（電源）につなぐ（配線に向きは無いのでどちら向きでも良い）

float vol=0;                   // 実数(float)型で変数volを定義（変数は型を定義しないとsetup()やloop()でエラー） 
const float MAX_VOLTAGE = 3.3; // 3.3Vを電源とした場合
const float ANALOG_MAX = 4095; // M5STACK(ESP32)の場合: 2^12(12bit)

void setup() { // 開始時1度だけ実行（セットアップ）
// M5.begin(false, true, true, true); // LCD以外の全ての引数を「true」：ノイズ低減
  M5.begin();
  Serial.begin(115200); // シリアル通信のデータ転送レート bps：ビット/秒
                        // ボーレートとも言う
                        // 飛び飛びの値 (9600/19200/38400/57600/115200)などを選ぶ
  dacWrite(25, 0);      // Speaker OFF（ブーと低音が鳴るのを防ぐ：M5Stackのテクニック）
  pinMode(HR_pin, INPUT);
}

void loop() { // 繰り返し実行（ループ）
   M5.update();
   vol=analogRead(HR_pin)*MAX_VOLTAGE/ANALOG_MAX;    // アナログ信号の取得（bitから電圧に変換）
   Serial.println(vol);                              // シリアル(USBでPC)に出力
   M5.Lcd.setCursor(30, 30, 4);                      // 液晶での表示位置
   M5.Lcd.printf("%.3f [V]",vol);                    // 液晶に表示（小数点3桁）
   delay(50);                                        // 次のループを少し(50msec)遅らせる
}
