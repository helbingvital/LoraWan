function decodeUplink(input) {
  // Message identifiers
    const STATUS_MSG = 0x24;
    const DATA_MSG = 0xC0;

    var decode = {};
    
    decode.identifier = input.bytes[0] ; //first byte is identifier

    if (decode.identifier == STATUS_MSG) { 
      decode.message = input.bytes.slice(1).map(byte => String.fromCharCode(byte)).join(''); // Decode from Bytes to ASCII-Text
      decode.type = "Status Message";

    }else if (decode.identifier== DATA_MSG){
      decode.type = "Data Message";
      if (input.bytes.length == 3) {// Temperature values consist of 3 bytes
            let sensorValue = 0;
            for (let i = 1; i < input.bytes.length; i++) {
                sensorValue = (sensorValue << 8) | input.bytes[i]; //  Join bytes
                }
            const input_masked = sensorValue & 0x000FFF;
            decode.message = "Temperature Sensor";
            decode.sensorValue = input_masked / 16;
        } else {
            decode.error = "Invalid data message format.";
        }
    }else{
      decode.type = "Unknown";
      decode.message = "Unknown.";
    }
  return {
        data: decode
    };
}
