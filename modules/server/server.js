const Blynk = require('blynk-library');
const AUTH = '-3XdDoUfyPxZG23tqn23DTm1EoUW0l7q';
const NFC_AUTH = 'bE-vYAT-y3C3C7g8MrzPiaYTgeEsZ653';

const blynk = new Blynk.Blynk(AUTH, options = {
    connector : new Blynk.TcpClient()
});

var nfc = new blynk.WidgetBridge(31); //31
var v0 = new blynk.VirtualPin(0);

v0.on('write', function(param) {
  console.log("request on V0");
  //nfc.virtualWrite(0,[48,32,FA,BC,24]);
});

blynk.on('connect', function() {
    nfc.setAuthToken(NFC_AUTH);
    console.log("Blynk ready.");
    nfc.virtualWrite(0,'4832FABC24');
});

blynk.on('disconnect', function() { console.log("DISCONNECT"); });

