import jmri

import java
import java.awt
import java.awt.event
import javax.swing

typePacket = 0
# set the intended LocoNet connection by its index; when you have just 1 connection index = 0
connectionIndex = 0

#Date: 15/januari/2019    Version:1.60   File: LocoNet IO V1p60.py

OPC_PEER_XFER = 0xE5

CODE_UPLOAD    = 0x15    #MASTER asks Port-Data from Device
CODE_UPDATE    = 0x16    #MASTER update new Port Data in Device
CODE_DATA      = 0x17    #Device transmit requested data to MASTER
CODE_RESET     = 0x13    #RESET Device
CODE_CLEAR     = 0x31    #CLEAR DEVICE: No functions for all Ports

NOP           ="No function"
SWITCH        ="SWITCH DIR=1 or 0 closed/open (+ S88 bus)"
BUTTON        ="BUTTON DIR= alternate between 1 and 0"
BUTTON_ON     ="BUTTON DIR=1"
BUTTON_OFF    ="BUTTON DIR=0"
BUTTON_ON_OFF ="BUTTON Send multiple addresses with DIR=1 or 0"
LED           ="PWM (LED)  DIR=1 --> ON  DIR=0 --> OFF"
LED_BLINKING  ="LED Blinking 1Hz  DIR=1 LED Blinking  DIR=0 LED OFF"
RELAY         ="DIGITAL OUT  DIR=1 PORT ON      DIR=0 PORT OFF"
COIL1         ="COIL1  DIR=1  (PORT+1 ==> COIL2 DIR=0)"
COIL2         ="COIL2  DIR=0  (Do not select it, use COIL1 DIR=1)" 
SERVO         ="SERVO  DIR=1 time=FV1   DIR=0 time=FV2"

myLocoNetConnection = jmri.InstanceManager.getList(jmri.jmrix.loconet.LocoNetSystemConnectionMemo).get(0);

class MyLnListener (jmri.jmrix.loconet.LocoNetListener) :

    def message(self,msg) :

         if ((msg.getElement(0)==OPC_PEER_XFER) and (msg.getElement(2)==CODE_DATA)) : 
              lFunction.setText(str(msg.getElement(7)+((msg.getElement(5) & 2)*64)))
              lFV1.setText(str(msg.getElement(8)+((msg.getElement(5) & 4)*32)))
              lFV2.setText(str(msg.getElement(9)+((msg.getElement(5) & 8)*16)))
              if int(lFunction.text) == 1 :
                     lFV2.setText(str(msg.getElement(9)+((msg.getElement(5) & 8)*16)+1))
              lFV3.setText(str(msg.getElement(11)+((msg.getElement(10) & 1)*128)))
              lBinair.setText(str(msg.getElement(12)+((msg.getElement(10) & 2)*64)))
              lLNAddress.setText(str(1+msg.getElement(13)+(msg.getElement(14)*128)))


              if   int(lFunction.text) == 0 :
         		lFV1.setEnabled(False)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(False)
         		lLNAddress.setEnabled(False)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("")
         		lFV2.setToolTipText("")
         		lFV3.setToolTipText("")
         		functieBox.setSelectedItem(NOP)
              elif  int(lFunction.text) == 1 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(True)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("Only use for S88 bus: range 1-255 example S88: ###.FV2")
         		lFV2.setToolTipText("Only use for S88 bus: range 1-16 example S88: FV1.##")
        		lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         		functieBox.setSelectedItem(SWITCH)
              elif  int(lFunction.text) == 3 :
         		lFV1.setEnabled(False)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("")
         		lFV2.setToolTipText("")
        		lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         		functieBox.setSelectedItem(BUTTON)
              elif  int(lFunction.text) == 5 :
         		lFV1.setEnabled(False)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("")
         		lFV2.setToolTipText("")
        		lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         		functieBox.setSelectedItem(BUTTON_ON)
              elif  int(lFunction.text) == 7 :
		        lFV1.setEnabled(False)
		        lFV2.setEnabled(False)
		        lFV3.setEnabled(True)
		        lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("")
         		lFV2.setToolTipText("")
        		lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         		functieBox.setSelectedItem(BUTTON_OFF)
              elif  int(lFunction.text) == 33 :
         		lFV1.setEnabled(False)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(True)
			lAdres.setEnabled(True)
         		lFV1.setToolTipText("")
         		lFV2.setToolTipText("")
        		lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         		functieBox.setSelectedItem(BUTTON_ON_OFF)
			if int(lLNAddress.text)/2048 == 3 :
                             selectBox.setSelectedItem("DIR=1")
			elif int(lLNAddress.text)/2048 == 1 :
                             selectBox.setSelectedItem("DIR=0")
			else :
                             selectBox.setSelectedItem("NOP")
			lLNAddress.setText(str(int(lLNAddress.text)%2048))
              elif  int(lFunction.text) == 64 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(True)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         		lFV2.setToolTipText("Setting the intensity when on:  range FV1-255")
         	        lFV3.setToolTipText("<html>Transistion period: <br>-  Range: 1-60 respectively 1-60sec<br>-  Range: 101-109 respectively 0,1-0,9sec")
         		functieBox.setSelectedItem(LED)
              elif  int(lFunction.text) == 66 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(True)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)         		
                        lFV1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         		lFV2.setToolTipText("Setting the intensity when on:  range FV1-255")
                        lFV3.setToolTipText("<html>Transistion period: <br>-  Range: 1-60 respectively 1-60sec<br>-  Range: 101-109 respectively 0,1-0,9sec")
         		functieBox.setSelectedItem(LED_BLINKING)
              elif  int(lFunction.text) == 8 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(True)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("Time in seconds before output set high:  range 0-255")
         		lFV2.setToolTipText("Time in seconde how long output stays high : range 0-255 if 0 then output stays high")
         		lFV3.setToolTipText("<html>If '0' = Output Active High; '1' = Output Active Low<br>If '2' = Output Active High; '3' = Output Active Low {Repeated FV1-FV2}<br>If '4' = Output Active High; '5' = Output Active Low {Output stays FV2 sec. extra high}")
         		functieBox.setSelectedItem(RELAY)
              elif  int(lFunction.text) == 16 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         		lFV2.setToolTipText("")
         		lFV3.setToolTipText("If '0' = Output Active High; '1' = Output Active Low")
         		functieBox.setSelectedItem(COIL1)
              elif  int(lFunction.text) == 18 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("Time in milliseconds how long output stays ON  range 1-255")
         		lFV2.setToolTipText("")
         		lFV3.setToolTipText("If '0' = Output Active High; '1' = Output Active Low")
         		functieBox.setSelectedItem(COIL2)
	      else :   # int(lFunction.text) == 128 
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(True)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			lAdres.setEnabled(False)
         		lFV1.setToolTipText("Minimum pulse time servo:  range 50-FV2  (100=1ms)")
         		lFV2.setToolTipText("Maximum pulse time servo:  range FV1-250  (200=2ms)")
         		lFV3.setToolTipText("Transistion period:  range 1-255, 1=very slow; 255=fast")
         		functieBox.setSelectedItem(SERVO)

              functieBox.setEnabled(True)
	      lPort.setEnabled(True)

              return

         return

myLocoNetConnection.getLnTrafficController().addLocoNetListener(0xFF,MyLnListener())



def whenLoadButtonClicked(event) :

     functieBox.setEnabled(False)
     lPort.setEnabled(False)

     msgLength = 16
     opcode = OPC_PEER_XFER
     ARG1 =  msgLength
     ARG2 =  CODE_UPLOAD 
     ARG3 =  int(lDevice.text)  
     ARG4 =  lPort.getValue() 
     ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + (int(lFV2.text)/128)*8
     ARG6 =  lAdres.getValue()  
     ARG7 =  int(lFunction.text)%128
     ARG8 =  int(lFV1.text)%128
     ARG9 =  int(lFV2.text)%128
     ARG10 = int(lFV3.text)/128 + (int(lBinair.text)/128)*2
     ARG11 = int(lFV3.text)%128
     ARG12 = int(lBinair.text)%128
     ARG13 = int(lLNAddress.text)%128 
     ARG14 = int(lLNAddress.text)/128
     ARG15 = 0 

     sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)
     return


def whenResetButtonClicked(event) :

     msgLength = 16
     opcode = OPC_PEER_XFER 
     ARG1 =  msgLength
     ARG2 =  CODE_RESET  #BRON = RESET DEVICE
     ARG3 =  int(lDevice.text)  
     ARG4 =  0x40 
     ARG5 =  0x00
     ARG6 =  0x20 
     ARG7 =  0x52
     ARG8 =  0x45
     ARG9 =  0x53
     ARG10 = 0x00
     ARG11 = 0x45
     ARG12 = 0x54
     ARG13 = 0x21
     ARG14 = 0x20
     ARG15 = 0 

     sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)
     return


def whenClearButtonClicked(event) :

     if lClear.getText() == "CLEAR" :

     	msgLength = 16
     	opcode = OPC_PEER_XFER 
     	ARG1 =  msgLength
     	ARG2 =  CODE_CLEAR  #BRON = CLEAR DEVICE
     	ARG3 =  int(lDevice.text)
     	ARG4 =  0x40 
     	ARG5 =  0x00
     	ARG6 =  0x43 
     	ARG7 =  0x4C
     	ARG8 =  0x45
     	ARG9 =  0x41
     	ARG10 = 0x00
     	ARG11 = 0x52
     	ARG12 = 0x20
     	ARG13 = 0x20
     	ARG14 = 0x20
     	ARG15 = 0 

     	sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)

     lClear.setText("")
     return
 

def whenSendButtonClicked(event) :

     msgLength = 16
     opcode = OPC_PEER_XFER 
     ARG1 =  msgLength
     ARG2 =  CODE_UPDATE
     ARG3 =  int(lDevice.text)  
     ARG4 =  lPort.getValue() 
     ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + (int(lFV2.text)/128)*8
     ARG6 =  lAdres.getValue()
     ARG7 =  int(lFunction.text)%128
     ARG8 =  int(lFV1.text)%128
     ARG9 =  int(lFV2.text)%128
     ARG10 = int(lFV3.text)/128 + (int(lBinair.text)/128)*2
     ARG11 = int(lFV3.text)%128
     ARG12 = int(lBinair.text)%128
     ARG13 = (int(lLNAddress.text)-1)%128 
     ARG14 = (int(lLNAddress.text)-1)/128
     ARG15 = 0
     if int(lFunction.text) == 33 and selectBox.getSelectedItem() == "DIR=1" :
              ARG14 = ARG14+48
     if int(lFunction.text) == 33 and selectBox.getSelectedItem() == "DIR=0" :
              ARG14 = ARG14+16            
     if int(lFunction.text) == 1 :
              ARG9 =  (int(lFV2.text)-1)%128
              ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + ((int(lFV2.text)-1)/128)*8
     sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)

     if   functieBox.getSelectedItem() == COIL1 : # IF select COIL1: Next port must be COIL2 with same LN address!
        lFunction.setText("18")
        ARG4 =  lPort.getValue() + 1  
        ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + (int(lFV2.text)/128)*8
        ARG6 =  lAdres.getValue() 
        ARG7 =  int(lFunction.text)%128
        ARG8 =  int(lFV1.text)%128
        ARG9 =  int(lFV2.text)%128
        ARG10 = int(lFV3.text)/128 + (int(lBinair.text)/128)*2
        ARG11 = int(lFV3.text)%128
        ARG12 = int(lBinair.text)%128
        ARG13 = (int(lLNAddress.text)-1)%128 
        ARG14 = (int(lLNAddress.text)-1)/128
        ARG15 = 0
	sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)

     return


def whenfunctionBoxItemChange(event) :
     
     if   functieBox.getSelectedItem() == NOP :  
         lLNAddress.setText("1")
         lFV1.setText("0")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(False)
         lFV2.setEnabled(False)
         lFV3.setEnabled(False)
         lLNAddress.setEnabled(False)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("")
         lFunction.setText("0")
         lAdres.setValue(int("1"))
     elif functieBox.getSelectedItem() == SWITCH :
         lLNAddress.setText("2048")     
         lFV1.setText("0")
         lFV2.setText("1")
         lFV3.setText("0")
         lFV1.setEnabled(True)
         lFV2.setEnabled(True)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Only use for S88 bus: range 1-255 example S88: ###.FV2")
         lFV2.setToolTipText("Only use for S88 bus: range 1-16 example S88: FV1.##")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("1")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == BUTTON :
         lLNAddress.setText("1")    
         lFV1.setText("0")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(False)
         lFV2.setEnabled(False)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("3")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == BUTTON_ON :
         lLNAddress.setText("1")     
         lFV1.setText("0")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(False)
         lFV2.setEnabled(False)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("5")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == BUTTON_OFF :
         lLNAddress.setText("1")     
         lFV1.setText("0")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(False)
         lFV2.setEnabled(False)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("7")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == BUTTON_ON_OFF:
         lLNAddress.setText("1")    
         lFV1.setText("0")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(False)
         lFV2.setEnabled(False)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(True)
	 lAdres.setEnabled(True)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("33")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == LED :
         lLNAddress.setText("1")    
         lFV1.setText("0")
         lFV2.setText("255")
         lFV3.setText("1")
         lFV1.setEnabled(True)
         lFV2.setEnabled(True)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         lFV2.setToolTipText("Setting the intensity when on:  range FV1-255")
         lFV3.setToolTipText("<html>Transistion period: <br>-  Range: 1-60 respectively 1-60sec<br>-  Range: 101-109 respectively 0,1-0,9sec")
         lFunction.setText("64")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == LED_BLINKING :
         lLNAddress.setText("1")    
         lFV1.setText("0")
         lFV2.setText("255")
         lFV3.setText("105")
         lFV1.setEnabled(True)
         lFV2.setEnabled(True)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         lFV2.setToolTipText("Setting the intensity when on:  range FV1-255")
         lFV3.setToolTipText("<html>Transistion period: <br>-  Range: 1-60 respectively 1-60sec<br>-  Range: 101-109 respectively 0,1-0,9sec")
         lFunction.setText("66")
         lAdres.setValue(int("1"))  
     elif  functieBox.getSelectedItem() == RELAY :
         lLNAddress.setText("1")    
         lFV1.setText("0")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(True)
         lFV2.setEnabled(True)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Time in seconds before output set high:  range 0-255")
         lFV2.setToolTipText("Time in seconde how long output stays high : range 0-255 if 0 then output stays high")
         lFV3.setToolTipText("<html>If '0' = Output Active High; '1' = Output Active Low<br>If '2' = Output Active High; '3' = Output Active Low {Repeated FV1-FV2}<br>If '4' = Output Active High; '5' = Output Active Low {Output stays FV2 sec. extra high}")
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == COIL1 :
         lLNAddress.setText("1")    
         lFV1.setText("255")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(True)
         lFV2.setEnabled(False)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("If '0' = Output Active High; '1' = Output Active Low")
         lFunction.setText("16") 
         lAdres.setValue(int("1"))
     elif  functieBox.getSelectedItem() == COIL2 :
         lLNAddress.setText("1")   
         lFV1.setText("255")
         lFV2.setText("0")
         lFV3.setText("0")
         lFV1.setEnabled(True)
         lFV2.setEnabled(False)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("If '0' = Output Active High; '1' = Output Active Low")
         lFunction.setText("18") 
         lAdres.setValue(int("1"))
     else : #functieBox.getSelectedItem() == SERVO 
         lLNAddress.setText("1")
         lFV1.setText("100")     
         lFV2.setText("200")
         lFV3.setText("255")
         lFV1.setEnabled(True)
         lFV2.setEnabled(True)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 lAdres.setEnabled(False)
         lFV1.setToolTipText("Minimum pulse time servo:  range 50-FV2  (100=1ms)")
         lFV2.setToolTipText("Maximum pulse time servo:  range FV1-250  (200=2ms)")
         lFV3.setToolTipText("Transistion period:  range 1-255, 1=very slow; 255=fast")
         lFunction.setText("128")
         lAdres.setValue(int("1"))
     return


def sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15) :

     packet = jmri.jmrix.loconet.LocoNetMessage(16)
     packet.setElement(0, opcode)
     packet.setElement(1, ARG1)
     packet.setElement(2, ARG2)
     packet.setElement(3, ARG3)
     packet.setElement(4, ARG4)
     packet.setElement(5, ARG5)
     packet.setElement(6, ARG6)
     packet.setElement(7, ARG7)
     packet.setElement(8, ARG8)
     packet.setElement(9, ARG9)
     packet.setElement(10, ARG10)
     packet.setElement(11, ARG11)
     packet.setElement(12, ARG12)
     packet.setElement(13, ARG13)
     packet.setElement(14, ARG14)
     packet.setElement(15, ARG15)   
     jmri.InstanceManager.getList(jmri.jmrix.loconet.LocoNetSystemConnectionMemo).get(connectionIndex).getLnTrafficController().sendLocoNetMessage(packet)

     return


sendButton = javax.swing.JButton("SEND")
sendButton.actionPerformed = whenSendButtonClicked
sendButton.setToolTipText("Send Port data to Device")

resetButton = javax.swing.JButton("RESET DEVICE")
resetButton.actionPerformed = whenResetButtonClicked
resetButton.setToolTipText("Reset device, recommended after changes")


functieBox = javax.swing.JComboBox()
functieBox.addItem(NOP)
functieBox.addItem(SWITCH)
functieBox.addItem(BUTTON)
functieBox.addItem(BUTTON_ON)
functieBox.addItem(BUTTON_OFF)
functieBox.addItem(BUTTON_ON_OFF)
functieBox.addItem(LED)
functieBox.addItem(LED_BLINKING)
functieBox.addItem(RELAY)
functieBox.addItem(COIL1)
functieBox.addItem(COIL2)
functieBox.addItem(SERVO)
functieBox.setToolTipText("Select function Port")
functieBox.setMaximumRowCount(12)
#functieBox.setcellHight(20)
functieBox.addItemListener(whenfunctionBoxItemChange)
functieBox.setEnabled(False)

selectBox = javax.swing.JComboBox()
selectBox.addItem("NOP")
selectBox.addItem("DIR=1")
selectBox.addItem("DIR=0")
selectBox.setToolTipText("Select DIR SW_Address")
selectBox.setEnabled(False)


# create fields

Portinvoer = javax.swing.SpinnerNumberModel(1,1,30,1)
lPort = javax.swing.JSpinner(Portinvoer)
lPort.setToolTipText("Select Port number between 1 and 30")
lPort.addChangeListener(whenLoadButtonClicked)
lPort.setEnabled(False)

lDevice  = javax.swing.JTextField("1",3)
lDevice.addActionListener(whenLoadButtonClicked)
lDevice.addActionListener(whenLoad_LF_ButtonClicked)

lClear  = javax.swing.JTextField("",5)
lClear.addActionListener(whenClearButtonClicked)
lClear.setToolTipText("Enter CLEAR and click Enter for clear device: no function for all Ports")

lFunction = javax.swing.JTextField("0",3)  
 
lLNAddress = javax.swing.JTextField("1",4)
lLNAddress.setEnabled(False)

Adresinvoer = javax.swing.SpinnerNumberModel(1,1,12,1)
lAdres =javax.swing.JSpinner(Adresinvoer)
lAdres.addChangeListener(whenLoadButtonClicked)
lAdres.setToolTipText("Select address BUTTON DIR=1 or DIR=0 between 1 and 12")
lAdres.setEnabled(False)



lFV1 = javax.swing.JTextField("0",3)
lFV1.setEnabled(False)
#lFV1.addActionListener(whenSendButtonClicked)

lFV2 = javax.swing.JTextField("0",3)
lFV2.setEnabled(False)

lFV3 = javax.swing.JTextField("0",3)
lFV3.setEnabled(False)

lBinair = javax.swing.JTextField("0",3)
lBinair.setEnabled(False)

lDevice.setToolTipText("Device number, select from 1 to 127")

lLNAddress.setToolTipText("Switch Address, select from 1 to 2048")


 
# create a frame to  display the buttons and panels:

f = javax.swing.JFrame("LocoNet IO device program tool.           Version 1.60           by Geert Giebens")
f.contentPane.setLayout(javax.swing.BoxLayout(f.contentPane, javax.swing.BoxLayout.Y_AXIS))

panelDeviceNr= javax.swing.JPanel()    
panelDeviceNr.add(javax.swing.JLabel("Device:"))         
panelDeviceNr.add(lDevice)

panelFV1 = javax.swing.JPanel()	
panelFV1.add(javax.swing.JLabel("FV1:"))	
panelFV1.add(lFV1)

panelFV2 = javax.swing.JPanel()	
panelFV2.add(javax.swing.JLabel("FV2:"))	
panelFV2.add(lFV2)

panelFV3 = javax.swing.JPanel()	
panelFV3.add(javax.swing.JLabel("FV3:"))	
panelFV3.add(lFV3)

panelLNAdres = javax.swing.JPanel()	
panelLNAdres.add(javax.swing.JLabel("SW_Address:"))	
panelLNAdres.add(lLNAddress)

panelDevice = javax.swing.JPanel()
panelDevice.add(panelDeviceNr)
panelDevice.add(resetButton)
panelDevice.add(lClear)


panelPort = javax.swing.JPanel()	
panelPort.add(lPort)
panelPort.add(functieBox)
panelPort.add(panelFV1)
panelPort.add(panelFV2)
panelPort.add(panelFV3)
panelPort.add(lAdres)
panelPort.add(selectBox)
panelPort.add(panelLNAdres)
panelPort.add(sendButton)

f.contentPane.add(panelDevice)
f.contentPane.add(javax.swing.JSeparator(javax.swing. JSeparator.HORIZONTAL))
f.contentPane.add(panelPort)

f.pack()
f.show()


#First load data from Device1: Port1

sendLoconetMsg(16,OPC_PEER_XFER,16,CODE_UPLOAD,1,1,0,1,0,0,0,0,0,0,0,0,0)







