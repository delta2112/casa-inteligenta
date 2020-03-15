const Blynk = require('blynk-library');
const AUTH = '-3XdDoUfyPxZG23tqn23DTm1EoUW0l7q';

const blynk = new Blynk.Blynk(AUTH, options = {
    connector : new Blynk.TcpClient()
});


// local server:
// const Blynk = require('blynk-library');  // Links variable 'Blynk' to the Blynk Library
// const AUTH = 'xxxxxxxxxx';  // Your top secret auth code
// const blynk = new Blynk.Blynk(AUTH, options = {
//     connector : new Blynk.TcpClient( options = { addr: "xxx.xxx.xxx.xxx", port: 8080 } )  // This takes all the info and directs the connection to you Local Server.
//     });
