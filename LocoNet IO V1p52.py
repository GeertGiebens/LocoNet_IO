import java
import javax.swing
typePacket = 0
#Date: 10/oktober/2018    Version:1.52   File: LocoNet IO V1p52.py

NOP           ="NOP = NO Operation"
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

AND           ="AND: Out = In1 AND In2"
NAND          ="NAND: Out = In1 AND In2 Inverted"
OR            ="OR: Out = In1 OR In2"
NOR           ="NOR: Out = In1 OR In2 Inverted"
EXOR          ="EXOR: Out = In1 EXOR In2"
NEXOR         ="NEXOR:  Out = In1 EXOR In2 Inverted"
FlipFlop      ="Flip-Flop"

OPCODE = 0xE5

class MyLnListener (jmri.jmrix.loconet.LocoNetListener) :

    def message(self,msg) :

         if ((msg.getElement(0)==OPCODE) and (msg.getElement(2)==0x12)) : 
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(True)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
         		lFV1.setToolTipText("Time in seconds before output set high:  range 0-255")
         		lFV2.setToolTipText("Time in seconde how long output stays high : range 0-255 if 0 then output stays high")
         		lFV3.setToolTipText("<html>If '0' = Output Active High; '1' = Output Active Low<br>If '2' = Output Active High; '3' = Output Active Low {Repeated FV1-FV2}")
         		functieBox.setSelectedItem(RELAY)
              elif  int(lFunction.text) == 16 :
         		lFV1.setEnabled(True)
         		lFV2.setEnabled(False)
         		lFV3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
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
			adresBox.setEnabled(False)
         		lFV1.setToolTipText("Minimum pulse time servo:  range 50-FV2  (100=1ms)")
         		lFV2.setToolTipText("Maximum pulse time servo:  range FV1-250  (200=2ms)")
         		lFV3.setToolTipText("Transistion period:  range 1-255")
         		functieBox.setSelectedItem(SERVO)

              return
         elif ((msg.getElement(0)==OPCODE) and (msg.getElement(2)==0x17)) : 
	      lLF_Binair.setText(str(msg.getElement(6)))
              lLF_Function.setText(str(msg.getElement(7)))
              lLF_AddressIn1.setText(str(1+msg.getElement(8)+(msg.getElement(9)*128)))
              lLF_AddressIn2.setText(str(1+msg.getElement(11)+(msg.getElement(12)*128)))
              lLF_AddressOut.setText(str(1+msg.getElement(13)+(msg.getElement(14)*128)))
              if   int(lLF_Function.text) == 0 :
         		lLF_AddressIn1.setEnabled(False)
         		lLF_AddressIn2.setEnabled(False)
         		lLF_AddressOut.setEnabled(False)
         		lLF_AddressIn1.setToolTipText("")
         		lLF_AddressIn2.setToolTipText("")
         		lLF_AddressOut.setToolTipText("")
         		lLF_functiebox.setSelectedItem(NOP)
              elif   int(lLF_Function.text) == 2 :
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Set Switch Address AND Output, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(AND)
              elif   int(lLF_Function.text) == 3 :
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Set Switch Address Inverted AND Output, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(NAND)
              elif   int(lLF_Function.text) == 4 :
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Set Switch Address OR Output, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(OR)
              elif   int(lLF_Function.text) == 5 :
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Set Switch Address Inverted OR Output, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(NOR)
              elif   int(lLF_Function.text) == 8 :
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Set Switch Address EXOR Output, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(EXOR)
              elif   int(lLF_Function.text) == 9 :
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Set Switch Address Inverted EXOR Output, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(NEXOR)
              else :   # int(lLF_Function.text) == 16 
         		lLF_AddressIn1.setEnabled(True)
         		lLF_AddressIn2.setEnabled(True)
         		lLF_AddressOut.setEnabled(True)
                        lLF_AddressIn1.setToolTipText("Switch Address SET Flip-Flop, select from 1 to 2048")
                        lLF_AddressIn2.setToolTipText("Switch Address RESET Flip-Flop, select from 1 to 2048")
                        lLF_AddressOut.setToolTipText("Flip-Flop Output Switch Address, select from 1 to 2048")
         		lLF_functiebox.setSelectedItem(FlipFlop)
	      return
         return

my_LnTrafContInstance = jmri.jmrix.loconet.LnTrafficController.instance()
my_LnTrafContInstance.addLocoNetListener(0xFF,MyLnListener())



def whenLoadButtonClicked(event) :

     msgLength = 16
     opcode = OPCODE 
     ARG1 =  msgLength
     ARG2 =  0x10  #BRON
     ARG3 =  int(lDevice.text)  
     ARG4 =  int(portBox.getSelectedItem())  
     ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + (int(lFV2.text)/128)*8
     ARG6 =  int(adresBox.getSelectedItem())   
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


def whenLoad_LF_ButtonClicked(event) :

     msgLength = 16
     opcode = OPCODE 
     ARG1 =  msgLength
     ARG2 =  0x16  #BRON
     ARG3 =  int(lDevice.text)  
     ARG4 =  int(lLF_portBox.getSelectedItem())  
     ARG5 =  0
     ARG6 =  0  
     ARG7 =  0
     ARG8 =  0
     ARG9 =  0
     ARG10 = 0
     ARG11 = 0
     ARG12 = 0
     ARG13 = 0
     ARG14 = 0
     ARG15 = 0 

     sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)
     return


def whenResetButtonClicked(event) :

     msgLength = 16
     opcode = OPCODE 
     ARG1 =  msgLength
     ARG2 =  0x13  #BRON = RESET DEVICE
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

 

def whenSendButtonClicked(event) :

     msgLength = 16
     opcode =OPCODE  
     ARG1 =  msgLength
     ARG2 =  0x11  #BRON
     ARG3 =  int(lDevice.text)  
     ARG4 =  int(portBox.getSelectedItem())  
     ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + (int(lFV2.text)/128)*8
     ARG6 =  int(adresBox.getSelectedItem())   
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
        ARG4 =  int(portBox.getSelectedItem()) + 1  
        ARG5 =  (int(lFunction.text)/128)*2 + (int(lFV1.text)/128)*4 + (int(lFV2.text)/128)*8
        ARG6 =  int(adresBox.getSelectedItem())   
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


def whenSend_LF_ButtonClicked(event) :

     msgLength = 16
     opcode =OPCODE  
     ARG1 =  msgLength
     ARG2 =  0x15  #BRON
     ARG3 =  int(lDevice.text)  
     ARG4 =  int(lLF_portBox.getSelectedItem())  
     ARG5 =  0
     ARG6 =  int(lLF_Binair.text)   
     ARG7 =  int(lLF_Function.text)
     ARG8 =  (int(lLF_AddressIn1.text)-1)%128 
     ARG9 =  (int(lLF_AddressIn1.text)-1)/128
     ARG10 = 0
     ARG11 = (int(lLF_AddressIn2.text)-1)%128
     ARG12 = (int(lLF_AddressIn2.text)-1)/128
     ARG13 = (int(lLF_AddressOut.text)-1)%128 
     ARG14 = (int(lLF_AddressOut.text)-1)/128
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("")
         lFunction.setText("0")
	 adresBox.setSelectedItem("1")
     elif functieBox.getSelectedItem() == SWITCH :
         lLNAddress.setText("1")     
         lFV1.setText("0")
         lFV2.setText("1")
         lFV3.setText("0")
         lFV1.setEnabled(True)
         lFV2.setEnabled(True)
         lFV3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Only use for S88 bus: range 1-255 example S88: ###.FV2")
         lFV2.setToolTipText("Only use for S88 bus: range 1-16 example S88: FV1.##")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("1")
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("3")
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("5")
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("7")
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(True)
         lFV1.setToolTipText("")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("When '0' then Normal Open contact NO; when '1' then Normal Closed contact NC")
         lFunction.setText("33")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         lFV2.setToolTipText("Setting the intensity when on:  range FV1-255")
         lFV3.setToolTipText("<html>Transistion period: <br>-  Range: 1-60 respectively 1-60sec<br>-  Range: 101-109 respectively 0,1-0,9sec")
         lFunction.setText("64")
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         lFV2.setToolTipText("Setting the intensity when on:  range FV1-255")
         lFV3.setToolTipText("<html>Transistion period: <br>-  Range: 1-60 respectively 1-60sec<br>-  Range: 101-109 respectively 0,1-0,9sec")
         lFunction.setText("66")
	 adresBox.setSelectedItem("1")  
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Time in seconds before output set high:  range 0-255")
         lFV2.setToolTipText("Time in seconde how long output stays high : range 0-255 if 0 then output stays high")
         lFV3.setToolTipText("<html>If '0' = Output Active High; '1' = Output Active Low<br>If '2' = Output Active High; '3' = Output Active Low {Repeated FV1-FV2}")
         lFunction.setText("8") 
	 adresBox.setSelectedItem("1") 
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("If '0' = Output Active High; '1' = Output Active Low")
         lFunction.setText("16") 
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         lFV2.setToolTipText("")
         lFV3.setToolTipText("If '0' = Output Active High; '1' = Output Active Low")
         lFunction.setText("18") 
	 adresBox.setSelectedItem("1")
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
	 adresBox.setEnabled(False)
         lFV1.setToolTipText("Minimum pulse time servo:  range 50-FV2  (100=1ms)")
         lFV2.setToolTipText("Maximum pulse time servo:  range FV1-250  (200=2ms)")
         lFV3.setToolTipText("Transistion period:  range 1-255")
         lFunction.setText("128")
	 adresBox.setSelectedItem("1") 
     return

def whenlLF_functieboxItemChanged(event) :

     if   lLF_functiebox.getSelectedItem() == NOP :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(False)
         lLF_AddressIn2.setEnabled(False)
         lLF_AddressOut.setEnabled(False)
         lLF_AddressIn1.setToolTipText("")
         lLF_AddressIn2.setToolTipText("")
         lLF_AddressOut.setToolTipText("")
         lLF_Function.setText("0")
     elif  lLF_functiebox.getSelectedItem() == AND :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Set Switch Address AND Output, select from 1 to 2048")
         lLF_Function.setText("2")
     elif  lLF_functiebox.getSelectedItem() == NAND :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Set Switch Address Inverted AND Output, select from 1 to 2048")
         lLF_Function.setText("3")
     elif  lLF_functiebox.getSelectedItem() == OR :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Set Switch Address OR Output, select from 1 to 2048")
         lLF_Function.setText("4")
     elif  lLF_functiebox.getSelectedItem() == NOR :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Set Switch Address Inverted OR Output, select from 1 to 2048")
         lLF_Function.setText("5")
     elif  lLF_functiebox.getSelectedItem() == EXOR :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Set Switch Address EXOR Output, select from 1 to 2048")
         lLF_Function.setText("8")
     elif  lLF_functiebox.getSelectedItem() == NEXOR :  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Set Input Switch Address 1, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Set Input Switch Address 2, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Set Switch Address Inverted EXOR Output, select from 1 to 2048")
         lLF_Function.setText("9")
     else:#   lLF_functiebox.getSelectedItem() == FlipFlop  
         lLF_AddressIn1.setText("1")
         lLF_AddressIn2.setText("1")
         lLF_AddressOut.setText("1")
         lLF_AddressIn1.setEnabled(True)
         lLF_AddressIn2.setEnabled(True)
         lLF_AddressOut.setEnabled(True)
         lLF_AddressIn1.setToolTipText("Switch Address SET Flip-Flop, select from 1 to 2048")
         lLF_AddressIn2.setToolTipText("Switch Address RESET Flip-Flop, select from 1 to 2048")
         lLF_AddressOut.setToolTipText("Flip-Flop Output Switch Address, select from 1 to 2048")
         lLF_Function.setText("16")
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
     jmri.jmrix.loconet.LnTrafficController.instance().sendLocoNetMessage(packet)
     return


sendButton = javax.swing.JButton("SEND")
sendButton.actionPerformed = whenSendButtonClicked
sendButton.setToolTipText("Send Port data to Device")

sendButton_LF = javax.swing.JButton("SEND")
sendButton_LF.actionPerformed = whenSend_LF_ButtonClicked
sendButton_LF.setToolTipText("Send Logical Port data to Device")

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

portBox = javax.swing.JComboBox()
portBox.addItem("1")
portBox.addItem("2")
portBox.addItem("3")
portBox.addItem("4")
portBox.addItem("5")
portBox.addItem("6")
portBox.addItem("7")
portBox.addItem("8")
portBox.addItem("9")
portBox.addItem("10")
portBox.addItem("11")
portBox.addItem("12")
portBox.addItem("13")
portBox.addItem("14")
portBox.addItem("15")
portBox.addItem("16")
portBox.addItem("17")
portBox.addItem("18")
portBox.addItem("19")
portBox.addItem("20")
portBox.addItem("21")
portBox.addItem("22")
portBox.addItem("23")
portBox.addItem("24")
portBox.addItem("25")
portBox.addItem("26")
portBox.addItem("27")
portBox.addItem("28")
portBox.addItem("29")
portBox.addItem("30")
portBox.setToolTipText("Select Port number")
portBox.setMaximumRowCount(30)
portBox.addItemListener(whenLoadButtonClicked)

adresBox = javax.swing.JComboBox()
adresBox.addItem("1") 
adresBox.addItem("2") 
adresBox.addItem("3") 
adresBox.addItem("4") 
adresBox.addItem("5") 
adresBox.addItem("6") 
adresBox.addItem("7") 
adresBox.addItem("8") 
adresBox.addItem("9") 
adresBox.addItem("10") 
adresBox.addItem("11") 
adresBox.addItem("12")
adresBox.setToolTipText("Select address BUTTON DIR=1 or 0")
adresBox.setMaximumRowCount(12)
adresBox.setEnabled(False)
adresBox.addItemListener(whenLoadButtonClicked)

selectBox = javax.swing.JComboBox()
selectBox.addItem("NOP")
selectBox.addItem("DIR=1")
selectBox.addItem("DIR=0")
selectBox.setToolTipText("Select DIR SW_Address")
selectBox.setEnabled(False)

lLF_functiebox = javax.swing.JComboBox()
lLF_functiebox.addItem(NOP)
lLF_functiebox.addItem(AND)
lLF_functiebox.addItem(NAND)
lLF_functiebox.addItem(OR)
lLF_functiebox.addItem(NOR)
lLF_functiebox.addItem(EXOR)
lLF_functiebox.addItem(NEXOR)
lLF_functiebox.addItem(FlipFlop)
lLF_functiebox.setToolTipText("Select logical function")
lLF_functiebox.setMaximumRowCount(9)
lLF_functiebox.addItemListener(whenlLF_functieboxItemChanged)

lLF_portBox = javax.swing.JComboBox()
lLF_portBox.addItem("31")
lLF_portBox.addItem("32")
lLF_portBox.addItem("33")
lLF_portBox.addItem("34")
lLF_portBox.setToolTipText("Select logical port number")
lLF_portBox.addItemListener(whenLoad_LF_ButtonClicked)

# create fields

lDevice  = javax.swing.JTextField("1",3)

lFunction = javax.swing.JTextField("0",3)  
 
lLNAddress = javax.swing.JTextField("1",4)
lLNAddress.setEnabled(False)

lFV1 = javax.swing.JTextField("0",3)
lFV1.setEnabled(False)
#lFV1.addActionListener(whenSendButtonClicked)

lFV2 = javax.swing.JTextField("0",3)
lFV2.setEnabled(False)

lFV3 = javax.swing.JTextField("0",3)
lFV3.setEnabled(False)

lBinair = javax.swing.JTextField("0",3)
lBinair.setEnabled(False)


lLF_AddressIn1 = javax.swing.JTextField("1",4)
lLF_AddressIn1.setEnabled(False)
lLF_AddressIn2 = javax.swing.JTextField("1",4)
lLF_AddressIn2.setEnabled(False)
lLF_AddressOut = javax.swing.JTextField("1",4)
lLF_AddressOut.setEnabled(False)
lLF_Binair = javax.swing.JTextField("0",3)
lLF_Function = javax.swing.JTextField("0",3)

lDevice.setToolTipText("Device number, select from 1 to 127")

lLNAddress.setToolTipText("Switch Address, select from 1 to 2048")
 
 
# create a frame to  display the buttons and panels:

f = javax.swing.JFrame("LocoNet IO device program tool.           Version 1.52           by Geert Giebens")
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

panelLF_LNAdres1 = javax.swing.JPanel()	
panelLF_LNAdres1.add(javax.swing.JLabel("Input1 SW_Address:"))	
panelLF_LNAdres1.add(lLF_AddressIn1)

panelLF_LNAdres2 = javax.swing.JPanel()	
panelLF_LNAdres2.add(javax.swing.JLabel("Input2 SW_Address:"))	
panelLF_LNAdres2.add(lLF_AddressIn2)

panelLF_LNAdres3 = javax.swing.JPanel()	
panelLF_LNAdres3.add(javax.swing.JLabel("Output SW_Address:"))	
panelLF_LNAdres3.add(lLF_AddressOut)

panelDevice = javax.swing.JPanel()
panelDevice.add(panelDeviceNr)
panelDevice.add(resetButton)


panelPort = javax.swing.JPanel()	
panelPort.add(portBox)
panelPort.add(functieBox)
panelPort.add(panelFV1)
panelPort.add(panelFV2)
panelPort.add(panelFV3)
panelPort.add(adresBox)
panelPort.add(selectBox)
panelPort.add(panelLNAdres)
panelPort.add(sendButton)

panelLF_Port = javax.swing.JPanel()
panelLF_Port.add(lLF_portBox)
panelLF_Port.add(lLF_functiebox)
panelLF_Port.add(panelLF_LNAdres1)
panelLF_Port.add(panelLF_LNAdres2)
panelLF_Port.add(panelLF_LNAdres3)
panelLF_Port.add(sendButton_LF)


f.contentPane.add(panelDevice)
f.contentPane.add(javax.swing.JSeparator(javax.swing. JSeparator.HORIZONTAL))
f.contentPane.add(panelPort)
f.contentPane.add(javax.swing.JSeparator(javax.swing. JSeparator.HORIZONTAL))
f.contentPane.add(panelLF_Port)

f.pack()
f.show()


