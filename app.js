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
