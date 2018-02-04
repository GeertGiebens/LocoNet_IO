# LocoNet_IO
LocoNet In/Out device with PIC microcontroller for DIY projects.

Written in ASSEMBLER for PIC 18F4620 MicroController.

This device has 30 Inputs/Outputs that can be set as different functions


# Hardware:

[Basic layout](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20IN%20UIT.png)
  
[Components needed to communicate with LocoNet](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20HARDWARE.png)
 
[Total schematic hardware](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20HARDWARE%20II.png)
 
[Hardware comparator](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20personal%20use%20edition%20conditions.png)
 
[Hardware on stripboard](https://github.com/GeertGiebens/LocoNet_IO/blob/master/PCB%20LOCONET%20II.png)
 
[Back side stripboard](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20IO%20Stripboard%20backside.png)
  
[Foto stripboard](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LocoNet%20foto%20PCB.png)
 
  
  # Software:
  
Communication with LocoNet:  LOCONET_18F4620_V1p0.INC

I explain here the relation between hardware and software interrupt program.

Main program must first initiate LocoNet communication:  ‘CALL  INIT_LOCONET’

For the communication with LocoNet bus I use hardware inside de microcontroller. The hardware reduce a lot of software code:
-Hardware comparator:  detect  when LocoNet signal is goes below 4V, the output comparator goes to hardware byte-receiver.

-Hardware Timer (T1) for keep time of Linebreak  and CD BackOff.

-Hardware Timer (T3) for Random generator. Used for calculate new CD BackOff time.

-Hardware Byte-transmitter for sending LocoNet data bytes.

-Hardware Byte-receiver for reception LocoNet data bytes .

-Hardware detect LocoNet is ‘IDLE’ --> register: BAUDCON,RCIDL=’0’. No receiving data.

-Hardware detect  ‘Linebreak’ --> register: RCSTA,FERR=’1’. If another device send a LineBreak when this device read bit’s, then Byte-receiver have a framing error.

There are 2 interrupts in the communication part of LocoNet:

Timer1 interrupt:
-Interrupt from LineBreak send by this device: then start timer with new CD BackOff time.

-Interrupt from CD BackOff time: if there is no new data to send --> restart timer with new CD BackOff time.

(I use for the CD BackOff time always a fixed time and a variable time consisting of CD+MD+PD   1200µs+360µs+random(1µs-1024µs). The  timer restart always with new CD BackOff time: CD+MD+PD (PD with new random time).  Device continues to try to gain access to LocoNet!)

Byte-receiver interrupt:
If the read Byte is a Opcode (bit7=‘1’), the interrupt know how much bytes there will follow. The read bytes are stored in a LocoNet receive buffer.  After read the last byte and the Error check is OK, a flag is SET so the MAIN program now that there is new LocoNet data.
If this device is sending data through the transmitter, this data is also read by the receiver. The receiver interrupt also regulate the next Byte to send. (The first Byte to send is started after Timer1 CD BackOff time). To check if there is ‘COLLISION’, the sending Byte must be the same with the received Byte. If not, then send a ‘Line Break’. The LocoNet data must then be resend.

  
  
[Hardware <--> INTERRUPT <--> Software](https://github.com/GeertGiebens/LocoNet_IO/blob/master/LOCONET%20INTERRUPT%20PROGRAM.png)

...
under construction (4 feb 2018)
...
