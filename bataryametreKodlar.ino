#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
/*
  Arduino'da 10 bitlik bir saklayıcı bulunmaktadır. Bu yüzden 5V giriş için 1024 değeri alınmaktadır
  1V'un anlık okumadaki değeri 204.8. İ
  204.8 --> 1V volt başına okunan değer
*/

/*
  1.5 Volt Alkain max ve min değerleri
  338 max - 1.65 V
  246 min - 1.2 V
  Aradaki fark 92
  Fark değerini max ve min % doluluk hesaplamada kullanıyoruz.
*/

/*
  3 Volt CR2032 max ve min değerleri. 
  614 max - 3 V
  410 min - 2 V
  Aradaki fark 204
  Fark değerini max ve min % doluluk hesaplamada kullanıyoruz.
*/

/*
  9 Volt Alkain max ve min değerleri
  1966 max -  9.6 V
  1106 min - 5.4 V
  Aradaki fark 860
  Fark değerini max ve min % doluluk hesaplamada kullanıyoruz.
*/

/* Voltmetre Değişkenler */
int analogInput = A0; // Analog pin tanımlaması
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // 100Kohm direnç
float R2 = 10000.0;  // 10Kohm direnç
int value = 0;

/* Pil türlerinin minimum ve maksimum gerilim değerleri */
float min_volt1 = 1.20;  // 1.5V pilin min voltajı
float max_volt1 = 1.65; // 1.5V pilin max voltajı

float min_volt3 = 2.0;  // 3V pilin min voltajı
float max_volt3 = 3.0;  // 3V pilin max voltajı

float min_volt9 = 5.4;  // 9V pilin min voltajı
float max_volt9 = 9.6;  // 9V pilin max voltajı

float yuzde = 0.0;

void setup() 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.clear();
}

void loop() 
{
  voltmetre();

  if (vin >= min_volt1 && vin <= max_volt1) 
  {
    hesapla1(); // 1.5 voltluk pillerin % doluluğunu hesaplayan fonksiyon
  }
  else if (vin >= min_volt3 && vin <= max_volt3) 
  {
    hesapla2(); // 3 voltluk pillerin % doluluğunu hesaplayan fonksiyon
  }
  else if (vin >= min_volt9 && vin <= max_volt9) 
  {
    hesapla3(); // 9 voltluk pillerin % doluluğunu hesaplayan fonksiyon
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Doluluk: %0");
  }

  delay(3000); // Ekranın sürekli güncellenmesini engellemek için delay
}

// Voltaj hesaplama fonksiyonu
void voltmetre()
{
  /*
    Bu sinyalin dijital sinyale çevrilmesi için Arduino'da 10 bitlik bir saklayıcı bulunmaktadır.
    Bu 5 volt giriş için ise 1024 değerini almaktadır.
  */

  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0; // ADC referans voltajı 5V olarak kabul edilmiştir
  vin = vout / (R2 / (R1 + R2));

  lcd.setCursor(0, 0);
  lcd.print("Voltaj: ");
  lcd.print(vin);

  Serial.print("Vin: ");
  Serial.println(vin);

  if (vin < 0.1) 
  {
    vin = 0.0;
    yuzde = 0.0;
    lcd.setCursor(0, 1);
    lcd.print("Doluluk: %");
    lcd.print(yuzde);
  }
}

// 1.5V'luk pillerin % doluluğunu hesaplayan fonksiyon
void hesapla1()
{
  yuzde = ((vin - min_volt1) / (max_volt1 - min_volt1)) * 100;
  yuzde = constrain(yuzde, 0, 100);

  lcd.setCursor(0, 1);
  lcd.print("Doluluk: %");
  lcd.print(yuzde);

  Serial.print(vin);
  Serial.println(yuzde);
  
}

// 3V'luk pillerin % doluluğunu hesaplayan fonksiyon
void hesapla2()
{
  yuzde = ((vin - min_volt3) / (max_volt3 - min_volt3)) * 100;
  yuzde = constrain(yuzde, 0, 100);

  lcd.setCursor(0, 1);
  lcd.print("Doluluk: %");
  lcd.print(yuzde);

  Serial.print("Doluluk 3V: %");
  Serial.println(yuzde);
}

// 9V'luk pillerin % doluluğunu hesaplayan fonksiyon
void hesapla3()
{ 
  yuzde = ((vin - min_volt9) / (max_volt9 - min_volt9)) * 100;
  yuzde = constrain(yuzde, 0, 100);

  lcd.setCursor(0, 1);
  lcd.print("Doluluk: %");
  lcd.print(yuzde);

  Serial.print("Doluluk 9V: %");
  Serial.println(yuzde);
}