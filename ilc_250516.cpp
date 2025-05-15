#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include "DHT.h" 
#include <math.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);
LiquidCrystal_I2C lcd2(0x26,  16, 2);
DHT dht(8, DHT22);

// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double getTD(double celsius, double humidity)
{
	double A0= 373.15/(273.15 + celsius);
	double SUM = -7.90298 * (A0-1);
	SUM += 5.02808 * log10(A0);
	SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
	SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
	SUM += log10(1013.246);
	double VP = pow(10, SUM-3) * humidity;
	double T = log(VP/0.61078);
	return (241.88 * T) / (17.558-T);
}

double getRH(double T, double TD){
  return 100*(exp((17.625*TD)/(243.04+TD))/exp((17.625*T)/(243.04+T)));
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd2.init();
  lcd2.backlight();
  dht.begin();
}

void loop() {
  delay(2000);
  lcd.clear();
  lcd2.clear();
  double h = dht.readHumidity();
  double t = dht.readTemperature();
  Serial.print(h);
  Serial.print(t);
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(t);
  lcd2.setCursor(0, 0);
  lcd2.print("TD: ");
  double dewPoint = getTD(t, h);
  lcd2.print(dewPoint);
  lcd2.setCursor(0, 1);
  lcd2.print("RH: ");
  lcd2.print(getRH(t, dewPoint));
}
