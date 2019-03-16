# LocoNet_IO

under construction for 18F4620 µC (last update: 13/Mrt/2019)


> !!! My native language is not English. I hope I made the explanation how it work clear enough. !!!
>
> 
> The explanation of the assembler code is in Flemish. In a while I will translate it into English. 
>
> The goal of this project is a simple LocoNet device for 30 Inputs/Outputs. You can choose for every port which function these have.  I shall explain how simple it is to upload the HEX code in the PIC µC (later). I shall explain how you can set up the function of the port with a JMRI Python script:
>
>https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20IO%20V1p55.py
>
><img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/JMRI%20Python%20LocoNet%20IO%20program%20tool%20v1p51_picture2.png>
>
><img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/JMRI%20Python%20LocoNet%20IO%20program%20tool%20v1p51_picture1.png>
>
> The settings menu provides sufficient information during use. This by giving information when you use the computer mouse over the choice of settings. This is only a basic version, later it will certainly be extended.
>


### Why 30 inputs/outputs? 
>A 40pin microcontroller PIC18F4620 (PIC18F4520) is quite cheap.  And the inputs/outputs are used faster than you think. For example, controlling LED's track signal (3-4 outputs); a control board with lay out tracks and so on. You do not have to use all the ports. I use them for modular construction.
>
### DIY? 
>The device  has a lot of hardware on-board.(byte-transmitter; byte-receiver; analog comparator and so on). I used a lot of this
hardware,  so the lay out of the PCB can be kept simple for DIY on stripboard or PCB.


## What can you do with this device:

### You can choose between different  inputs functions:

- TOGLE SWITCH:   CLOSED--> OPC_SW_REQ(ADRES+DIR=1)  /  OPEN--> OPC_SW_REQ(ADRES+DIR=0)  (AND OPTIONAL:  OPC_INPUT_REP(...) for block detector)

- PUSH BUTTON SWITCH:   PRESSED--> OPC_SW_REQ(ADRES+DIR=1)  /  PRESSED AGAIN --> OPC_SW_REQ(ADRES+DIR=0) and so on …

- PUSH BUTTON SWITCH ON:   PRESSED--> OPC_SW_REQ(ADRES+DIR=1)

- PUSH BUTTON SWITCH OFF:   PRESSED--> OPC_SW_REQ(ADRES+DIR=0)

- PUSH BUTTON SWITCH ON/OFF:  PRESSED--> OPC_SW_REQ(ADRES1+DIR(‘0’ OR ‘1’)) + OPC_SW_REQ(ADRES2+DIR(‘0’ OR ‘1’)) + ... + OPC_SW_REQ(ADRES12+DIR(‘0’ OR ‘1’)) (if  needed)


 
 
### Or you can choose between different output functions: 

- DIGITAL OUT:  OPC_SW_REQ(ADRES+DIR=1) then output is 'ON' / OPC_SW_REQ(ADRES+DIR=0) then output is 'OFF' (And with many timer functions)

- COIL1:   OPC_SW_REQ(ADRES+DIR=1) then output1 is 'ON' for 1-255ms (=parameter)
- COIL2:   OPC_SW_REQ(ADRES+DIR=0) then output2 is 'ON' for 1-255ms (=parameter)

- PWM/LED:   OPC_SW_REQ(ADRES+DIR=1) then Output is 'ON' with PWM / OPC_SW_REQ(ADRES+DIR=0) then Output is 'OFF' with PWM

- LED BLINKING:   OPC_SW_REQ(ADRES+DIR=1) then LED is toggle 'ON'/'OFF' with 1Hz frequency  / OPC_SW_REQ(ADRES+DIR=0) then LED is 'OFF'

- SERVO:   OPC_SW_REQ(ADRES+DIR=1) then servo is in state1 (parameter1) / OPC_SW_REQ(ADRES+DIR=0) then servo is in state2 (parameter2)

Each output function has various parameters, later more about it. Outputs with Relay or Coil function requires a extra 5V relay board!




# Hardware:

[Basic layout](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20IN%20UIT.png)
  
[Components needed to communicate with LocoNet](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20HARDWARE.png)
 
[Total schematic hardware](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20HARDWARE%20II.png)
 
[Hardware comparator](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20personal%20use%20edition%20conditions.png)


# PCB:
 
[Photo first test version](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet_IO_testopstelling.png)

[Hardware on stripboard](https://github.com/GeertGiebens/LocoNet_IO/blob/master/PCB%20LOCONET%20II.png)
 
[Back side stripboard](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20IO%20Stripboard%20backside.png)
  
[Photo stripboard](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20foto%20PCB.png)


>[Photo PCB V3 cupperside](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet_IO%20Print%20V3%20koperzijd.jpg)
>
>[Photo PCB V3 componentside](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet_IO%20Print%20V3%20bovenzijde.jpg)
>
>Eagle files:
>
>https://github.com/GeertGiebens/LocoNet_IO/blob/master/Loconet%20in-out%20v3.brd
>https://github.com/GeertGiebens/LocoNet_IO/blob/master/Loconet%20in-out%20v3.sch

I am now working on a multifunctional PCB for LocoNet applications. More on this later.

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20V6%20PCB%20koperbaan.png> 

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20V6%20PCB%20componenten.png> 

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/Testopstelling%20met%20componenten%20LocoNet%20IO%20V6p0.png>
  
  # Software:
  
## Communication with LocoNet:  LOCONET_18F4X20_V1p2.INC

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20HARDWARE-INTERRUPT-MAIN.png>

I explain here the relation between hardware and software interrupt program.

Main program must first initiate LocoNet communication:  ‘CALL  INIT_LOCONET’

For the communication with LocoNet bus I use hardware inside de microcontroller. The hardware reduce a lot of software code:

* Hardware comparator:  detect  when LocoNet signal is goes below 4V, the output comparator goes to hardware byte-receiver.

* Hardware Timer (T1) for keep time of Linebreak  and CD BackOff.

* Hardware Timer (T3) for Random generator. Used for calculate new CD BackOff time.

* Hardware Byte-transmitter for sending LocoNet data bytes.

* Hardware Byte-receiver for reception LocoNet data bytes .

* Hardware detect LocoNet is ‘IDLE’ --> register: BAUDCON,RCIDL=’0’. No receiving data.

* Hardware detect  ‘Linebreak’ --> register: RCSTA,FERR=’1’. If another device send a LineBreak when this device read bit’s, then Byte-receiver have a framing error.

* To avoid collisions to the minimum (access times lower than 2µs), the software resets the baud rate generator 60μs just before the transmitter starts transmitting 0x77 --> register: SPBRG (only if LocoNet is IDLE).

* I use the internal oscillator and no crystal. So far I have already connected several μC to LocoNet without communication problem. Should this ever occur, then provide a solution via the software.

 

## There are 2 interrupts in the communication part of LocoNet:

### Timer1 interrupt:

* Interrupt from LineBreak send by this device: then start timer with new CD BackOff time.

* Interrupt from CD BackOff time: if there is no new data to send --> restart timer with new CD BackOff time.

(I use for the CD BackOff time always a fixed time and a variable time consisting of CD+MD+PD  --> 1200µs+360µs+random(1µs-1024µs). The  timer restart always with new CD BackOff time: CD+MD+PD (PD with new random time).  Device continues to try to gain access to LocoNet!)

### Byte-receiver interrupt:

If the read Byte is a Opcode (bit7=‘1’), the interrupt know how much bytes there will follow. The read bytes are stored in a LocoNet receive buffer.  After read the last byte and the Error check is OK, a flag is SET so the MAIN program now that there is new LocoNet data.

If this device is sending data through the transmitter, this data is also read by the receiver. The receiver interrupt also regulate the next Byte to send. (The first Byte to send is started after Timer1 CD BackOff time). To check if there is ‘COLLISION’, the sending Byte must be the same with the received Byte. If not, then send a ‘Line Break’. The LocoNet data must then be resend.

## Schematic:
  
<img alt="open opps 1" src= https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20INTERRUPT%20PROGRAM.png>
  

 # LocoNet communication test in extreme conditions: 
 
To test the LocoNet communication in extreme conditions, it was necessary to add counters in the program code. These counters keep the most important data in connection with access to LocoNet. These counters can be requested via a (JMRI) Python script. This communication also takes place via LocoNet! First the intention was to remove these counters from the code after the tests. But I finally saved them. The Python script can be found here:

https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet_IO_Status_V1p1.py  

Save it, and you can start this script via JMRI software.

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20IO%20Status%20Monitor.png>

This data may also be useful to follow the LocoNet for each device.

The following pictures show some snapshots of the tests.  Here was tested with 8 µC (microcontrollers). The intention was to send as many 4 byte messages as possible per µC.

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/photo%20test%20extreme%20testing%20LocoNet.png>

I also use the LocoNet monitor from JMRI for testing.  On this image you can see how 8 µC each with its Switch address LT1 ... LT8 try to send a change of state via random access:

<img alt="open opps 1" src=https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20Monitor%20printout%20DATA.png>

...


### Other projects:
-[LocoNet PIC12F683](https://github.com/GeertGiebens/LocoNet-12F683)
