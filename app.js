/**
  * Accepts commands from clicks on the UI, the velocity byte 
  * is not implemented yet. A zero is just passed in.
  */
function sendCommand(value)
{
console.log('sendCommand is clicked:' + value);
switch(value)
  {
    case 0:
      voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([0,0]));
      break;
    case 1:
      voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([1,0]));
      break;
    case 2:
      voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([2,0]));
      break;
    case 3:
      voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([3,0]));
      break;
    case 4:
      voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([4,0]));
      break;
  }
};

/**
* Add voice control with the annyang library
*/


if (annyang)
{
 
 var halt = function()
 {
   voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([0,0]));
 };

 var forward = function()
 {
   voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([1,0]));
 };
 
 var reverse = function()
 {
  voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([2,0]));
 };
 
 var left = function()
 {
   voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([3,0]));
 };
 
 var right = function()
{ 
  voicebot._writeCharacteristic(voicebot.mainDriveUUID, new Uint8Array([4,0]));
 };
 /**
   * These are the words and associated commands in key:value
   * pairs that are "understood" by the robot.
   */
 
 var commands =
 {
   'stop':halt,
   'halt':halt,
   'wait':halt,
   'go':forward,
   'forward':forward,
   'go ahead':forward,
   'onward':forward,
   'reverse':reverse,
   'back':reverse,
   'back up':reverse,
   'left':left,
   'turn left':left,
   'right':right,
   'turn right':right
  };
  
  annyang.debug();
  annyang.addCommands(commands);
  annyang.setLanguage('en');
  annyang.start();
 
};

/**
  * The Web Bluetooth API specifies that a user action like a click is needed to
  * initiate a connection.
  */
function bleConnect(buttonID)
{
  //var thisButton = document.getElementById(buttonID);
  // thisButton.setAttribute("fill-opacity",0.9);
  console.log('ble connect is clicked');
  voicebot.connect()
      .then(() => console.log('connected'))
      .catch(error => { console.log('connect error!'); });
};











