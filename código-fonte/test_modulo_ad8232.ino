void setup() {
   Serial.begin(9600);
   pinMode(3, INPUT); // Configuração para detecção de derivações LO +
   pinMode(2, INPUT); // Configuração para detecção de leads off LO -
}
 
void loop() {
 
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
     Serial.println('!');
  } else {
    // envia o valor da entrada analógica 0:
    Serial.println(analogRead(A0));
  }
  // Espere um pouco para evitar que os dados seriais saturem
  delay(1);
}
