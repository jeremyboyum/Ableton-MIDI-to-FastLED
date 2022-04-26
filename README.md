# Ableton-MIDI-to-FastLED
Arduino code that reads MIDI data from Ableton (should work in any DAW) and outputs scenes on an LED strip using FastLED. (neopixel, WS2812B, etc.) This can be used for a lot of things but what I used it for is creating custom stage lights for my band to use at our concerts.

### an example of this working:
https://www.tiktok.com/@jeremyboyum/video/7088371054233652522?is_from_webapp=1&sender_device=pc&web_id=7057303959291872774

This Arduino code requires the use of the [Processing IDE](https://processing.org/download) and a VirtualMIDI application (native to MAC or *[loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html)* for windows)

The Processing code from [Yearnings](https://github.com/yearnings/MidiToNeopixel/tree/main/Processing/MidiToSerial) will also be required for this to work and I highly recommend watching their YouTube tutorial: https://www.youtube.com/watch?v=5VdeOb7PoQc

## Software used
### Processing + TheMidiBus
* [Download the Processing IDE here](https://processing.org/download)
* [The MIDIBUS library](http://www.smallbutdigital.com/projects/themidibus/) (available via the processing libraries window)

### Arduino + FastLED
* [Download the Arduino IDE here](https://www.arduino.cc/en/software)
* [FastLED](https://fastled.io/)
