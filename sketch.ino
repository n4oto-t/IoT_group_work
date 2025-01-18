#include <Servo.h>

#define PIN_HEATER A0 
#define PIN_SENSOR A4 
#define PIN_OUTPUT A3 

//サーボモータ
Servo servo;
#define servoPIN 7
bool AngleIsZero = true;

#define len_of_array 10
int data[len_of_array]; // 匂いセンサで計測した直近10個のデータを格納する配列
int index = 0;
int target_value = 400; // data配列に格納した値の平均がこの値を超えればブザーを鳴らす。

// 光センサー
const int cdsPin = A5; // CDSセルを接続するアナログピン
int cdsValue = 0; // CDSセルの値を格納する変数
const int lightThreshold = 1000; // CDSセルの明るさの閾値(環境に合わせて変更)

void setup() {
  servo.attach(servoPIN , 500 , 2400);

  pinMode(PIN_HEATER, OUTPUT);
  pinMode(PIN_SENSOR, OUTPUT);
  pinMode(PIN_OUTPUT, INPUT);
  pinMode(cdsPin, INPUT);

  digitalWrite(PIN_HEATER, HIGH); // Heater Off
  digitalWrite(PIN_SENSOR, LOW); // Sensor Pullup Off

  Serial.begin(9600);
  delay(3000);

  // 配列のデータを実際のセンサー値で初期化
  for (int i = 0; i < len_of_array; i++) {
    data[i] = analogRead(PIN_OUTPUT);
  }
}

void loop() {
  delay(237);
  digitalWrite(PIN_SENSOR, HIGH); // Sensor Pullup On
  delay(3);

  int val = analogRead(PIN_OUTPUT); // 匂いセンサで計測した値
  delay(2);

  digitalWrite(PIN_SENSOR, LOW); // Sensor Pullup Off
  digitalWrite(PIN_HEATER, LOW); // Heater On
  delay(8);
  digitalWrite(PIN_HEATER, HIGH); // Heater Off

  Serial.println(val);

  index = (index + 1) % len_of_array;
  data[index] = val;

  // 配列に格納された値の平均を計算
  long sum = 0;
  for (int i = 0; i < len_of_array; i++) {
    sum += data[i];
  }
  int average = sum / len_of_array;

  Serial.print("average: ");
  Serial.println(average);

  // CDSセルの値を読み取る
  cdsValue = analogRead(cdsPin);
  Serial.print("CDS value: ");
  Serial.println(cdsValue);

  // サーボモータの制御
  if (average < target_value && cdsValue < lightThreshold) {
    // 配列の平均値がtarget_value以下で、CDSセルの値が閾値未満ならサーボモータの角度変更
    changeAngleOfServo();
    Serial.println("Buzzing...");
  } else {
    Serial.println("No Buzz.");
  }

  // 閾値の送信
  Serial.print("threshold: ");
  Serial.println(target_value);
}

void changeAngleOfServo() {
    // 0度と180度を切り替える
    int servoAngle = AngleIsZero ? 180 : 0;
    AngleIsZero = !AngleIsZero;

    servo.write(servoAngle);
    Serial.print("Servo angle: ");
    Serial.println(servoAngle);
    delay(1000);
}