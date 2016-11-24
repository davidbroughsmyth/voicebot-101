/**
 *  11/19/2016 VoiceBot101.ino
 *  
 *  This is the begining sketch to implement BLE interface to control a mobile robot platform. 
 *  The voice control component will be implemented on the browser.
 *  
 *  This is an initial prototype sketch for the voice bot 101, and is likely to change
 *  significantly as the project progresses. For instance, you will see many calls to
 *  delay in this code. They can be deadly for robot applications and will disapear 
 *  as the project develops and is refined.
 */
#include <CurieBLE.h>
 
#define BLELED 2
#define DEBUGLED 4

class MainDrive
{
  /**
   * The MainDrive class is implemented here to drive 2 DC motors using the 
   * Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board, PRODUCT ID: 2448
   * https://www.adafruit.com/products/2448
   * 
   * Motor is controlled with,
   * Enable pins(PWM) - Arduino pins 3,5
   * Control pins 1,2   - Arduino pins 8,9
   * Control pins 2,2   - Arduino pins 12,13
   */
   
  #define M1PWM 3
  #define M2PWM 5

  #define M1CP1 8
  #define M1CP2 9
  #define M2CP1 12
  #define M2CP2 13

  #define STOP 0
  #define QUARTERSPEED 63
  #define HALFSPEED 127
  #define FULLSPEED 255
  
  public:

  MainDrive(int i){}

  void init()
  {
    pinMode(M1PWM, OUTPUT);
    pinMode(M2PWM, OUTPUT);
    
    pinMode(M1CP1, OUTPUT);
    pinMode(M1CP2, OUTPUT);
    pinMode(M2CP1, OUTPUT);
    pinMode(M2CP2, OUTPUT);
   
  }

  void halt()
  {  
    analogWrite(M1PWM, STOP);
    analogWrite(M2PWM, STOP);

    digitalWrite(M1CP1, LOW);
    digitalWrite(M1CP2, LOW);
    digitalWrite(M2CP1, LOW);
    digitalWrite(M2CP2, LOW);
  }
  
  void forward(int velocity)
  {
      velocity =  HALFSPEED;
      
      analogWrite(M1PWM, velocity);
      analogWrite(M2PWM, velocity);
      
      digitalWrite(M1CP1, LOW);
      digitalWrite(M1CP2, HIGH);
      digitalWrite(M2CP1, LOW);
      digitalWrite(M2CP2, HIGH);
  }
  
  void reverse(int velocity)
  {
      //comment out for go live:
      velocity = HALFSPEED;

      analogWrite(M1PWM, velocity);
      analogWrite(M2PWM, velocity);
      
      digitalWrite(M1CP1, HIGH);
      digitalWrite(M1CP2, LOW);
      digitalWrite(M2CP1, HIGH);
      digitalWrite(M2CP2, LOW);
     
  }
  
  void right()
  {  
      analogWrite(M1PWM, FULLSPEED);
      analogWrite(M2PWM, FULLSPEED);
      
      digitalWrite(M1CP1, LOW);
      digitalWrite(M1CP2, HIGH);
      digitalWrite(M2CP1, HIGH);
      digitalWrite(M2CP2, LOW);
  }  
  
  void left()
  { 
      analogWrite(M1PWM, FULLSPEED);
      analogWrite(M2PWM, FULLSPEED);
      
      digitalWrite(M1CP1, HIGH);
      digitalWrite(M1CP2, LOW);     
      digitalWrite(M2CP1, LOW);
      digitalWrite(M2CP2, HIGH);
  }
    
  void test()
  {
    forward(0);
    delay(500);
    halt();
    reverse(0);
    delay(500);
    halt();
    left();
    delay(350);
    halt();
    right();
    delay(350);
    halt();
    forward(0);
    delay(500);
    halt();
  }
  
};


/**
 * Implement BLE service
 * BLE Service Name vbot101
 * BLE Characteristic mainDriveCharacteristic [ CMD ] [Speed]
 *  CMD
 *   0    Halt/Stop
 *   1    Forward
 *   2    Reverse
 *   3    Right
 *   4    Left
 *   
 *   Speed 0..255
 */

BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService vbot101Service("917649A0-D98E-11E5-9EEC-0002A5D5C51B"); // Custom UUID
BLECharacteristic mainDriveCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLEWrite, 2);

MainDrive mainDrive(1);


void setup() 
{
  // Serial.begin(9600);
  // initialize the main drive object
  mainDrive.init();

  // The LED on pin 2 is used as an indicator light for BLE
  pinMode(BLELED, OUTPUT);
  digitalWrite(BLELED, LOW);
  
  // the LED on pin 4 is used for debugging purposes
  pinMode(DEBUGLED, OUTPUT);
  digitalWrite(DEBUGLED, LOW);

  // initilize the ble service, characterisitcs, advertising, event handlers, etc.
  blePeripheral.setLocalName("vbot101");
  blePeripheral.setAdvertisedServiceUuid(vbot101Service.uuid());
  blePeripheral.addAttribute(vbot101Service);
  blePeripheral.addAttribute(mainDriveCharacteristic);
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  mainDriveCharacteristic.setEventHandler(BLEWritten, mainDriveCharacteristicWritten);

  const unsigned char initMainDrive[2] = {0,0};
  mainDriveCharacteristic.setValue(initMainDrive,2);
  
  // advertise the service
  blePeripheral.begin();


  // give me a few seconds to get everythong ready!
  
  delay(3000);
  debugLED();
  mainDrive.test();
  debugLED();
 
}

void loop() 
{
  blePeripheral.poll();
}


void debugLED()
{
  // debug
  for (int i=0;i<25;i++)
  {
    digitalWrite(DEBUGLED,HIGH);
    delay(50);
    digitalWrite(DEBUGLED,LOW);
    delay(50);  
  }
}


/**
 * BLE Event Handlers are defined in the fucntions below
 */
 
void blePeripheralConnectHandler(BLECentral& central) 
{
  // central connected event handler
  digitalWrite(BLELED, HIGH);
}

void blePeripheralDisconnectHandler(BLECentral& central) 
{
  // central disconnected event handler
   digitalWrite(BLELED, LOW);
}

/**
 * This event handler responds to changes to the mainDriveCharacterisitc when they are made across 
 * the BLE connection. For now, the speed/velocity component of the mainDriveCharacteristic will not be
 * implemented,.
 */
void mainDriveCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{
  
  const unsigned char* mainDriveCommand = mainDriveCharacteristic.value();
  int cmd = mainDriveCommand[0];
  int velocity = mainDriveCommand[1];

  switch(cmd){
    case(0):
      mainDrive.halt();
      break;
    case(1):
      mainDrive.forward(0);
      break;
    case(2):
      mainDrive.reverse(0);
      break;
    case(3):
      mainDrive.right();
      break;
    case(4):
      mainDrive.left();
      break;
  }


  
}



