# Intro
This is a repo with (almost) everything necessary to build an "OK to Wake" lamp - the gist is, when the lamp is a certain color, the kiddo knows to stay in bed; when it changes, it's okay to get out of bed.

# Components
There are three components to this project: the physical lamp, physical components that light up the lamp, and the software running on the lamp.

## The Physical Lamp
In some ways this was the easiest part of the project. There's an STL file included that in two pieces - the shade and the base. The base has a hole along the bottom to thread in a USB cable and a spot to rest the electronics; the shade fits into a cutout groove on the base.

Additionally, a PDF of the circuit is also included; it 

## The Electronic Components
The circuit uses an ESP8266, two RGB LEDs plus 220ohm resistors, and a switch to cut power to the LEDs regardless of what the software says to do. The base of the lamp fits two mini breadboards.

### BOM
* 1x NodeMCU ESP8266 dev board
* 2x RGB LEDs (common cathode)
* 6x 220ohm resistors
* 1x switch (this layout uses a SPDT switch but any mechanical non-momentary switch should work)

## The Software

The firmware on the ESP8266 comes in two parts: the actual system control, and a web interface to provide visibility and allow for changes. The firmware is reasonably straightforward Arduino-compatible C++; the web interface is a Vue 3 single-page app (SPA) built in the `/webserver` directory, which then compiles to the `/data` directory (which can be built and then uploaded to the ESP8266 flash storage) where it's served by the MCU.