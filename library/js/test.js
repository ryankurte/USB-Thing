var UsbThing = require('./build/Release/usbthing');

console.log(UsbThing);

var obj = new UsbThing.UsbThing(10);
console.log( obj.plusOne() ); // 11
console.log( obj.plusOne() ); // 12
console.log( obj.plusOne() ); // 13
