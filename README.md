# Project: Home Appliance Monitoring System
 This project has the main focus of providing a base for systems which can provide increasingly advanced features and metrics in helping monitor the consumption of any and any number of electrical devices connected to the system. The main goal is the efficient usage of the arduino microcontroller, while using a flexible number of relay stations and amplified electrical connections.

# 1. Planning / Components
For the system, we will be using the following:

 A. Arduino board with ATMega328P microcontroller
  - [Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42743-ATmega324P_Datasheet.pdf)

 B. I2C LCD Display
  - Standard 16x2 Liquid Crystal Display.
  - Display mode: STN
  - Connected through an I2C serial adapter board module ( [I2C information](https://en.wikipedia.org/wiki/I²C) )
  - [Buy link](https://www.bitmi.ro/electronica/modul-interfata-i2c-pentru-lcd1602-10456.html)
  <img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/adapteri2c.jpg" width=200>

 C. Relay
  - The Relay is a digital normally open switch that controls a relay capable of switching much higher voltages and currents than your normal Arduino boards. When the relay is switched on, the Bulb will light up and the relay will allow current to flow.
  - As the relay will have contact with a 220-240V power source on the signal end, we will be using a diode from the microcontroller to the VCC of the relay.
  - Relay Maximum output: DC 30V/10A, AC 250V/10A
  - 4 Channel Relay Module with Optocoupler LOW Level Trigger expansion board, which is compatible with arduino
  - [Buy link](https://www.bitmi.ro/modul-releu-4-canale-cu-optocuplor-5v-low-level-10494.html)
  <img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/relay.jpg" width=300>

 D. Rocker switch
  - 2 pins, one for ground, one for GPIO
  <img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/butonswitch.jpg" width=200>

 E. Pushbutton
  - 4 pin button found in standard circuitry kits

 F. 4.5W Lightbulb
  - Standard E27 lightbulb, wired both into the relay, and into the power socket
  - Stands as an example for any outlet powered appliance in the house

 G. 1N4007 Diode
  - Used as a failsafe for the current that will flow on the heavy end of the relay, since we are working with high voltage, compared to the 5 volts we will be using to power up the relay itself and send commands to it.
  - Makes it so that no backlash from coursing high currents through the relay could flow in towards the VCC of the microcontroller

# 2. Hardware scheme
<img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/schematicproiect.png" width=600>

This shows shortly how the basic structure of how the circuit will be connected, with the rough idea that the buttons will communicate through their scopes of action via code. In essence, any piece of indoor electricity could be connected simultaneously to the relay and programmed to be monitored by the microcontroller. 

<img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/setupcomplet.jpeg" width=300> <img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/setupoutlet.jpeg" width=300> <img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/setupbody.jpeg" width=300>

Additionally, as seen, the socketting should be done after ensuring the wires are fixed in place and the connections secured. The entire setup will be placed in a DIY case with certain holes for the power socket and the rocker switch. 

# 3. Functionality

<img src="https://github.com/pricop-alexandru/Robotics/blob/proiect/images/simulareproiect.png" width=600>

This is the detailed schematic of the circuit, containing the following connections of the system:

1. Arduino UNO (ATMega328P, 16MHz)
- SCL (A5) to SCL pin on the 16x2 I2C LCD
- SDA (A4) to SDA pin on the 16x2 I2C LCD
- 5V to VCC pin on the 16x2 I2C LCD
- 5V to Anode of the Diode
- GND to GND pin on the Pushbutton
- GND to Pin 2 on the Rocker Switch
- GND to GND pin on the 16x2 I2C LCD
- GND to GND pin on the Relay Module
- GND to negative terminal of the Bulb
- D3 to Pin 2 (in) on the Pushbutton
- D4 to Pin 1 on the Rocker Switch
- D5 to IN4 pin on the Relay Module

2. 16x2 I2C LCD
- SCL to SCL (A5) on the Arduino UNO
- SDA to SDA (A4) on the Arduino UNO
- VCC to 5V on the Arduino UNO
- GND to GND on the Arduino UNO

3. Relay 4 Channel 5v Module
- COM4 to positive terminal of the Bulb
- NC4 to the Live Wire of the power socket
- VCC to Cathode of the Diode
- GND to GND on the Arduino UNO
- IN4 to D5 on the Arduino UNO

4. Pushbutton
- Pin 3 (out) to GND on the Arduino UNO
- Pin 2 (in) to D3 on the Arduino UNO

5. Rocker Switch
- Pin 2 to GND on the Arduino UNO
- Pin 1 to D4 on the Arduino UNO

6. Bulb
- Positive terminal to COM4 on the Relay Module
- Neutral terminal to Neutral Wire in the power socket

7. Diode
- Anode to 5V on the Arduino UNO
- Cathode to VCC on the Relay Module

Flow: 

The Arduino checks every second the states of the Switch and the Pushbutton. It displays every second the on/off state of the electrical device(s) connected to the relay, and the runtime in hh.dd.ss, on the LCD, via I2C connection. The LCD's lifetime can be preserved by turning it off or on through the pushbutton. All the readings are measured in real time through the microcontroller's built-in timer. The lightbulb and essentially any other piece of electronic household object is neutrally grounded to their power socket, and the Relay Module is set on NC for their respective station (we are using station 4), which stands for normally closed as we do not want the flow to go through by default. The relay thus acts as a gate of electricity between the lightbulb and the 220-240V power socket, as the signal is connected to the bulb, which is then triggered by the flick of the rocker switched, sent as information further by the microcontroller.

# 4. Coding

# 5. Final

