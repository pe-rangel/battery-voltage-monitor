const int pinoLeitura = A0;
const int ledVerde = 13;
const int ledAmarelo = 12;
const int ledVermelho = 11;

// Constantes dos resistores para divisão de tensão
const float R1 = 20000.0;  // 20kΩ
const float R2 = 10000.0;  // 10kΩ

void setup() {
  Serial.begin(9600);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  }

void loop() {
  int valorADC = analogRead(pinoLeitura);
  
  // Converte a leitura (0–1023) para tensão no pino (0–5V)
  float tensaoA0 = valorADC * (5.0 / 1023.0);
  // Corrige para tensão real da fonte (antes dos resistores)
  float tensaoFonte = tensaoA0 * ((R1 + R2) / R2);

  String estado = "";

  if (tensaoFonte >= 13.9) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    estado = "Verde";
  } 
  else if (tensaoFonte >= 12.2 && tensaoFonte < 13.9) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    estado = "Amarelo";
 } 
  else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    estado = "Vermelho";
  }

  // Envia dados no formato CSV: tempo(s), tensao(V), estado
  Serial.print(millis() / 1000.0);
  Serial.print(",");
  Serial.print(tensaoFonte, 2);
  Serial.print(",");
  Serial.println(estado);

  delay(500);
}
