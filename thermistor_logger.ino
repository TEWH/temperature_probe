// Set logging frequency
//   LOG_INTERVAL is milliseconds between each reading
//   There is not much latent delay for the thermistor (maybe just ~2 ms), therefore
//   its probably a good idea to include a little bit of delay using LOG_INTERVAL
//   SYNC_INTERVAL is the number of milliseconds to wait before writing data to SD card
//   If SYNC_INTERVAL = LOG_INTERVAL, then every data point is immediately written.
//   Increase SYNC_INTERVAL will use less power and is faster. 
#include <SOftwareSerial.h>
SoftwareSerial EEBlue(10,11);

const uint32_t LOG_INTERVAL = 1000;
const uint32_t SYNC_INTERVAL = 2500;



// Set pins to use on the Arduino
//   Thermistor is wired to analog pin 0 on the datalogging shield
#define THERMISTOR_PIN A0


// Set up temperature calculation constants for thermistor
//    Thermistor series resistor is 10kOhm.  Measured resistance for resistor
//    soldered to datalogging shield
#define THERMISTOR_SERIES_RESISTOR 10000// Protoype datalogging shield
#define NOMINAL_TEMP 25
#define B_COEF 3950
#define C_TO_K 273.15


///////////////////
// setup() runs once when the program starts
void setup( void ) {

//     Start the serial monitor to export data across serial connection to screen  
    Serial.begin(9600);
    EEBlue.begin(9600);
}
//
// End of setup function
///////////////////

///////////////////
// loop() is run continuously as long as the Arduino is powered on
void loop(void) {
  if (EEBlue.available())
    Serial.write(EEBlue.read());


//     Delay for the amount of time we want between readings
    delay( LOG_INTERVAL );

    
//     Set up variables to use to measure resistance and calculate temperature
    float thermistor_reading;
    float thermistor_resistance;
    float inverse_temp;
    float thermistor_temp;

//     Read voltage across thermistor on analog pin
    thermistor_reading = analogRead( THERMISTOR_PIN );

//     Convert voltage reading to resistance of thermistor using known series resistor value
    thermistor_resistance = THERMISTOR_SERIES_RESISTOR/((1023.0/thermistor_reading)-1.0);
    
//     Use one parameter B equaton to convert from thermistor resistance to temp
//       1/T = 1/T0 + 1/B*ln(R/R0)
    inverse_temp = (1.0/(NOMINAL_TEMP+C_TO_K)) + (1.0/B_COEF)*log(thermistor_resistance/THERMISTOR_SERIES_RESISTOR);

//    Convert to Celcius from Kelvin
    thermistor_temp = (1.0/inverse_temp) - C_TO_K;

    thermistor_temp = thermistor_temp*9/5+32;

    String data = "~TP#" + (String)thermistor_temp + ",";
    
//     Output temperature measurement to the serial connection
    //Serial.print(thermistor_reading);
    Serial.println( "Thermistor temp: " );
    Serial.println( thermistor_temp );
    EEBlue.print(data);
    
}
//
// End of loop function
///////////////////
