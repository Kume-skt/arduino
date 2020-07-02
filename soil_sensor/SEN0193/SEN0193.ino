void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val;
  val = analogRead(A6);
    Serial.println(convertMoistureToPercent(val));
//  Serial.println(val);
  delay(1000);
}
int convertMoistureToPercent(int value){
  int MoisturePercentValue = 0;
  MoisturePercentValue = map(value, 2980, 1400, 0, 100);
  if(MoisturePercentValue > 100){
    MoisturePercentValue = 100;
  }

  return MoisturePercentValue;
}
