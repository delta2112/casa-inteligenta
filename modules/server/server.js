const Blynk = require('blynk-library');
const AUTH = '-3XdDoUfyPxZG23tqn23DTm1EoUW0l7q';
const ARDU_AUTH = 'bE-vYAT-y3C3C7g8MrzPiaYTgeEsZ653';

const blynk = new Blynk.Blynk(AUTH, options = {
    connector : new Blynk.TcpClient()
});

var bridge1 = new blynk.WidgetBridge(31); //31
var v0 = new blynk.VirtualPin(0);

v0.on('write', function(param) {
  console.log("request on V0");
  bridge1.virtualWrite(0,[1,2,3]);
});

blynk.on('connect', function() {
    bridge1.setAuthToken(ARDU_AUTH);
    console.log("Blynk ready.");
});

blynk.on('disconnect', function() { console.log("DISCONNECT"); });

