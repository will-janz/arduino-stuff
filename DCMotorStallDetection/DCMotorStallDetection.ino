/* ***************************************************************
 *  DC motor single transistor w/ current and power measurement 
 *  MEGN200 - Arduino activities Week 3
 * ***************************************************************/

/* Global Constants */ 
const int R1in = A0; // analog pin connected to + side of 10 Ohm resistor
const int R1out = A1; // analog pin connected to - side of 10 Ohm resistor
const int speed_pin = 3; // digital pin connected to the base of the transistor
const int R1Val = 10; // shunt resistor value in Ohms
const float ADCconvert = 5/1023.0; // ratio converting the 10bit ADC value from analogRead to a real voltage value

#define LEDPIN 4

/* Global Variables */ 
int motor_speed = 255; // use full voltage for power calculation 
int Vin = 0; // voltage at + side of 10 Ohm resistor
int Vout = 0; // voltage at - side of 10 Ohm resistor
float current = 0; // mA
float Power = 0; // mW
float avgpowercalculation = 0; // mW
int power_calc_iteration = 0;
float avgPower = 0; // mW
  
void setup() {
  Serial.begin(9600);
  pinMode(speed_pin, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  analogWrite(speed_pin,motor_speed); // run motor at full speed
}

  // no need to modify any code above here - make sure to check defined variables

void loop() {

  delay(500); // don't need to read fast (you can change this if you like)
  Vin = analogRead(A0); // read the voltage at the + side of the 10 Ohm resistor
  Vout = analogRead(A1); // read the voltage at the - side of the 10 Ohm resistor
  delay(10);

  // calculate current and power.
  // equation for current:  I = V/R
  // equation for power:  P = IV
  
  // complete the variable assignment calculations below
  // use the ADCconvert variable (defined above) to convert from digital values to voltages
  current =  ((Vin - Vout) * ADCconvert / R1Val) * 1000;  // x1000 to convert to milliamps. Convert analog read value to voltage using ADCconvert.
  Power =  current * (float)Vout * ADCconvert; // motor power = mA * motor volts. Which analog read corresponds to the voltage at the + motor terminal?

  // no need to modify the averaging code here, scroll down to Stall Detection section to complete the assignment
  // the code below is used to average the last 5 power values to find the average power avgPower (printed below)
  if (power_calc_iteration < 5){
    avgpowercalculation = avgpowercalculation + Power;
    power_calc_iteration++;
  }
  else {
    avgPower = avgpowercalculation/5;
    power_calc_iteration = 0;
    avgpowercalculation = 0;
  }

  // Alternate logging for excel
  Serial.print(Vout * ADCconvert);
  Serial.print(",");
  Serial.print(current);
  Serial.print(",");
  Serial.println(Power);

  //Serial print the voltage across the motor, current, motor Power, and avgPower
  // Serial.print("VOut: " + (String)Vout);
  // Serial.print(", ");
  // Serial.print("current: " + (String)current);
  // Serial.print(", ");
  // Serial.print("Power: " + (String)Power);
  // Serial.print(", ");
  // Serial.println("avgPower: " + (String)avgPower);

// Stall detection 
// Find a threshold value for avgPower that is between the power consumed by the motor 1. when the fan is attached and spinning at full speed
// and 2. When the motor is stalled

// Write a conditional statement to check if avgPower is greater than this threshold value
// If the power draw is too high (motor stalled), then turn off the motor using analogWrite
// If the power draw is below the threshold (motor not stalled), run the motor at motor_speed using analogWrite. The motor will attempt to restart every time the avgPower variable drops below the threshold

  // 400mW is above max power for a free spinning fan
  if (Power > 400) {
    analogWrite(speed_pin, 0);
    Serial.println("STALL"); 
    digitalWrite(LEDPIN, HIGH);
    delay(2000);
    analogWrite(speed_pin,motor_speed);
    digitalWrite(LEDPIN, LOW);
  }
}
