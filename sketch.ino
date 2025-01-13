#define PIN_HEATER A0 
#define PIN_SENSOR A4 
#define PIN_OUTPUT A3 
#define buzzerPIN 8 
int melo = 300;


#define len_of_array 10
int data[10]; //匂いセンサで計測した直近10個のデータを格納する配列
int index = 0;
int target_value = 400; // data配列に格納した値の平均がこの値を超えればブザーを鳴らす。

void setup() {
 pinMode(PIN_HEATER,OUTPUT);
 pinMode(PIN_SENSOR,OUTPUT);
 pinMode(PIN_OUTPUT, INPUT);


 digitalWrite(PIN_HEATER,HIGH); // Heater Off
 digitalWrite(PIN_SENSOR,LOW); // Sensor Pullup Off

 // 配列のデータを1000で初期化。
for (int i = 0; i < len_of_array; i++) {
        data[i] = 1000;
    }

  Serial.begin(9600);
  delay(3000);
}


void loop() {
    delay(237);
    digitalWrite(PIN_SENSOR,HIGH); // Sensor Pullup On
    delay(3);
      
    int val = analogRead(PIN_OUTPUT); // 匂いセンサで計測した値
    delay(2);

    digitalWrite(PIN_SENSOR,LOW); // Sensor Pullup Off
    digitalWrite(PIN_HEATER,LOW); // Heater On
    delay(8);
    digitalWrite(PIN_HEATER,HIGH); // Heater Off

    Serial.println(val);

    index = (index +1) % len_of_array;
    data[index] = val;

    // 配列に格納された値の平均を計算
    int sum = 0;
    for (int i=0;i<len_of_array;i++){
      sum +=data[i];
    }
    int average = sum/len_of_array;

    Serial.print("average: ");
    Serial.println(average);

    // 配列の平均値がtarget_valueを超えていればブザーならす。
    if (average < target_value){
      tone(buzzerPIN,262, melo);
    }
}