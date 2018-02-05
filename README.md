# LocoNet_IO


!!! My native language is not English. I hope I made the explanation how it work clear enough. !!!

The assembler code is in Flemish. In a while I will translate it into English. 


The goal of this project is a simple LocoNet device for 30 Inputs or Outputs. You can choose for every port which function these have.  I shall explain how simple it is to upload the HEX code in the PIC µC. I shall explain how you can set up the function of the port with JMRI software. 

The hardware is very simple for DIY projects. You can perform it with stripboard or with PCB board. 

## Wat can you do with it:

### You can choose between different  inputs functions:

- TOGLE SWITCH:   CLOSED--> OPC_SW_REQ (ADRES+DIR=1) / OPEN--> OPC_SW_REQ(ADRES+DIR=0)  (AND OPTIONAL:  OPC_INPUT_REP() for block detector)

- PUSH BUTTON SWITCH:   PRESSED--> OPC_SW_REQ (ADRES+DIR=1) / PRESSED AGAIN --> OPC_SW_REQ(ADRES+DIR=0) and so on …

- PUSH BUTTON SWITCH ON:   PRESSED--> OPC_SW_REQ (ADRES+DIR=1)

- PUSH BUTTON SWITCH OFF:   PRESSED--> OPC_SW_REQ (ADRES+DIR=0)

- PUSH BUTTON SWITCH ON/OFF:  PRESSED--> OPC_SW_REQ(ADRES1+DIR(‘0’ OR ‘1’)) + OPC_SW_REQ(ADRES2+DIR(‘0’ OR ‘1’)) and so on till OPC_SW_REQ(ADRES12+DIR(‘0’ OR ‘1’)) (if  needed)

 
### Or you can choose between different output functions: 

- RELAY: OPC_SW_REQ(ADRES+DIR=1) then output is 'ON' / OPC_SW_REQ(ADRES+DIR=0) then output is 'OFF' 

- COIL1: OPC_SW_REQ(ADRES+DIR=1) then output1is 'ON' for 1-255ms (=parameter)

- COIL2: OPC_SW_REQ((ADRES+DIR=0) then output2 is 'ON' for 1-255ms (=parameter)

- LED: OPC_SW_REQ(ADRES+DIR=1) then LED is 'ON' / OPC_SW_REQ(ADRES+DIR=0) then LED is
'OFF'

- LED BLINKING: OPC_SW_REQ(ADRES+DIR=1) then LED is toggle 'ON'/'OFF' with 1Hz frequency  / OPC_SW_REQ(ADRES+DIR=0) then LED is 'OFF'

- SERVO: OPC_SW_REQ(ADRES+DIR=1) then servo is in state1 (parameter1) / OPC_SW_REQ(ADRES+DIR=0) then servo is in state2 (parameter2)

Each output function has various parameters, later more about it. 




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

[Photo PCB V3 cupperside](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet_IO%20Print%20V3%20koperzijd.jpg)

[Photo PCB V3 componentside](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet_IO%20Print%20V3%20bovenzijde.jpg)
 
  
  # Software:
  
## Communication with LocoNet:  LOCONET_18F4620_V1p0.INC

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

---

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
  

...
under construction (5 feb 2018)
...
