# USBThing Firmware

## Dependencies

## Testing

Working on PyRSP based automated integration tests. Need to add TCP socket functionality to PyRSP.
The idea is that Python will call the usbthing library function then intercept the peripheral call in the firmware to ensure correct data is passed.
Requires `pip install pyelftools pyserial`
