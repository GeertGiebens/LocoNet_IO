import java
import javax.swing
typePacket = 0
#Date: 30/april/2018    Version:1.1   File: LocoNet IO V1p1.py

NOP           ="NOP = NO Operation"
SWITCH        ="SWITCH DIR=1 closed  DIR=0 opened  (+ S88 bus)"
BUTTON        ="BUTTON DIR= alternate between 1 and 0"
BUTTON_ON     ="BUTTON DIR=1"
BUTTON_OFF    ="BUTTON DIR=0"
BUTTON_ON_OFF ="BUTTON Send multiple addresses with DIR=1 or 0"
LED           ="LED   DIR=1 LED ON         DIR=0 LED OFF"
LED_BLINKING  ="LED   DIR=1 LED BLINKING   DIR=0 LED OFF"
RELAY         ="RELAY  DIR=1 RELAY ON      DIR=0 RELAY OFF"
COIL1         ="COIL1  DIR=1  (PORT+1 ==> COIL2 DIR=0)"
COIL2         ="COIL2  DIR=0  (Do not select it, use COIL1 DIR=1)" 
SERVO         ="SERVO  DIR=1 time=FV1   DIR=0 time=FV2"

OPCODE = 0xE5

class MyLnListener (jmri.jmrix.loconet.LocoNetListener) :

    def message(self,msg) :

         if ((msg.getElement(0)==OPCODE) and (msg.getElement(2)==0x12)) : 
              lFunction.setText(str(msg.getElement(7)+((msg.getElement(5) & 2)*64)))
              lConst1.setText(str(msg.getElement(8)+((msg.getElement(5) & 4)*32)))
              lConst2.setText(str(msg.getElement(9)+((msg.getElement(5) & 8)*16)))
              if int(lFunction.text) == 1 :
                     lConst2.setText(str(msg.getElement(9)+((msg.getElement(5) & 8)*16)+1))
              lConst3.setText(str(msg.getElement(11)+((msg.getElement(10) & 1)*128)))
              lBinair.setText(str(msg.getElement(12)+((msg.getElement(10) & 2)*64)))
              lLNAddress.setText(str(1+msg.getElement(13)+(msg.getElement(14)*128)))

              if   int(lFunction.text) == 0 :
         		lConst1.setEnabled(False)
         		lConst2.setEnabled(False)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(False)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(NOP)
              elif  int(lFunction.text) == 1 :
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(True)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Only use for S88 bus: range 1-255 example S88: ###.FV2")
         		lConst2.setToolTipText("Only use for S88 bus: range 1-16 example S88: FV1.##")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(SWITCH)
              elif  int(lFunction.text) == 3 :
         		lConst1.setEnabled(False)
         		lConst2.setEnabled(False)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(BUTTON)
              elif  int(lFunction.text) == 5 :
         		lConst1.setEnabled(False)
         		lConst2.setEnabled(False)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(BUTTON_ON)
              elif  int(lFunction.text) == 7 :
		        lConst1.setEnabled(False)
		        lConst2.setEnabled(False)
		        lConst3.setEnabled(False)
		        lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(BUTTON_OFF)
              elif  int(lFunction.text) == 33 :
         		lConst1.setEnabled(False)
         		lConst2.setEnabled(False)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(True)
			adresBox.setEnabled(True)
         		lConst1.setToolTipText("")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(BUTTON_ON_OFF)
			if int(lLNAddress.text)/2048 == 3 :
                             selectBox.setSelectedItem("DIR=1")
			elif int(lLNAddress.text)/2048 == 1 :
                             selectBox.setSelectedItem("DIR=0")
			else :
                             selectBox.setSelectedItem("NOP")
			lLNAddress.setText(str(int(lLNAddress.text)%2048))
              elif  int(lFunction.text) == 64 :
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(True)
         		lConst3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         		lConst2.setToolTipText("Setting the intensity when on:  range FV1-255")
         		lConst3.setToolTipText("Transistion period:  range 1-255  1=slow; 255=fast")
         		functieBox.setSelectedItem(LED)
              elif  int(lFunction.text) == 66 :
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(True)
         		lConst3.setEnabled(True)
         		lLNAddress.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         		lConst2.setToolTipText("Setting the intensity when on:  range FV1-255")
         		lConst3.setToolTipText("Transistion period:  range 1-255  1=slow; 255=fast")
         		functieBox.setSelectedItem(LED_BLINKING)
              elif  int(lFunction.text) == 8 :
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(True)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Time in seconds before output set high:  range 0-255")
         		lConst2.setToolTipText("Time in seconde how long output stays high : range 0-255 if 0 then output stays high")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(RELAY)
              elif  int(lFunction.text) == 16 :
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(False)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(COIL1)
              elif  int(lFunction.text) == 18 :
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(False)
         		lConst3.setEnabled(False)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Time in milliseconds how long output stays ON  range 1-255")
         		lConst2.setToolTipText("")
         		lConst3.setToolTipText("")
         		functieBox.setSelectedItem(COIL2)
	      else :#int(lFunction.text) == 128
         		lConst1.setEnabled(True)
         		lConst2.setEnabled(True)
         		lConst3.setEnabled(True)
         		lLNAddress.setEnabled(True)
			selectBox.setEnabled(False)
			adresBox.setEnabled(False)
         		lConst1.setToolTipText("Minimum pulse time servo:  range 50-FV2  (100=1ms)")
         		lConst2.setToolTipText("Maximum pulse time servo:  range FV1-250  (200=2ms)")
         		lConst3.setToolTipText("Transistion period:  range 1-255")
         		functieBox.setSelectedItem(SERVO)

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
     ARG5 =  (int(lFunction.text)/128)*2 + (int(lConst1.text)/128)*4 + (int(lConst2.text)/128)*8
     ARG6 =  int(adresBox.getSelectedItem())   
     ARG7 =  int(lFunction.text)%128
     ARG8 =  int(lConst1.text)%128
     ARG9 =  int(lConst2.text)%128
     ARG10 = int(lConst3.text)/128 + (int(lBinair.text)/128)*2
     ARG11 = int(lConst3.text)%128
     ARG12 = int(lBinair.text)%128
     ARG13 = int(lLNAddress.text)%128 
     ARG14 = int(lLNAddress.text)/128
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
     ARG5 =  (int(lFunction.text)/128)*2 + (int(lConst1.text)/128)*4 + (int(lConst2.text)/128)*8
     ARG6 =  int(adresBox.getSelectedItem())   
     ARG7 =  int(lFunction.text)%128
     ARG8 =  int(lConst1.text)%128
     ARG9 =  int(lConst2.text)%128
     ARG10 = int(lConst3.text)/128 + (int(lBinair.text)/128)*2
     ARG11 = int(lConst3.text)%128
     ARG12 = int(lBinair.text)%128
     ARG13 = (int(lLNAddress.text)-1)%128 
     ARG14 = (int(lLNAddress.text)-1)/128
     ARG15 = 0
     if int(lFunction.text) == 33 and selectBox.getSelectedItem() == "DIR=1" :
              ARG14 = ARG14+48
     if int(lFunction.text) == 33 and selectBox.getSelectedItem() == "DIR=0" :
              ARG14 = ARG14+16            
     if int(lFunction.text) == 1 :
              ARG9 =  (int(lConst2.text)-1)%128
              ARG5 =  (int(lFunction.text)/128)*2 + (int(lConst1.text)/128)*4 + ((int(lConst2.text)-1)/128)*8
     sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)

     if   functieBox.getSelectedItem() == COIL1 : # IF select COIL1: Next port must be COIL2 with same LN address!
        lFunction.setText("18")
 	ARG4 =  int(portBox.getSelectedItem())+1  
  	ARG7 =  18
	sendLoconetMsg(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)

     return

def whenInitButtonClicked(event) :
     
     if   functieBox.getSelectedItem() == NOP :  
         lLNAddress.setText("1")
         lConst1.setText("0")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(False)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(False)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("0")
	 adresBox.setSelectedItem("1")
     elif functieBox.getSelectedItem() == SWITCH :
         lLNAddress.setText("2048")     
         lConst1.setText("0")
         lConst2.setText("1")
         lConst3.setText("0")
         lConst1.setEnabled(True)
         lConst2.setEnabled(True)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Only use for S88 bus: range 1-255 example S88: ###.FV2")
         lConst2.setToolTipText("Only use for S88 bus: range 1-16 example S88: FV1.##")
         lConst3.setToolTipText("")
         lFunction.setText("1")
	 adresBox.setSelectedItem("1")
     elif  functieBox.getSelectedItem() == BUTTON :
         lLNAddress.setText("1")    
         lConst1.setText("0")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(False)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("3")
	 adresBox.setSelectedItem("1")
     elif  functieBox.getSelectedItem() == BUTTON_ON :
         lLNAddress.setText("1")     
         lConst1.setText("0")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(False)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("5")
	 adresBox.setSelectedItem("1")
     elif  functieBox.getSelectedItem() == BUTTON_OFF :
         lLNAddress.setText("1")     
         lConst1.setText("0")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(False)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("7")
	 adresBox.setSelectedItem("1")
     elif  functieBox.getSelectedItem() == BUTTON_ON_OFF:
         lLNAddress.setText("1")    
         lConst1.setText("0")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(False)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(True)
	 adresBox.setEnabled(True)
         lConst1.setToolTipText("")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("33")
     elif  functieBox.getSelectedItem() == LED :
         lLNAddress.setText("1")    
         lConst1.setText("0")
         lConst2.setText("255")
         lConst3.setText("4")
         lConst1.setEnabled(True)
         lConst2.setEnabled(True)
         lConst3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         lConst2.setToolTipText("Setting the intensity when on:  range FV1-255")
         lConst3.setToolTipText("Transistion period:  range 1-255  1=slow; 255=fast")
         lFunction.setText("64")
	 adresBox.setSelectedItem("1")
     elif  functieBox.getSelectedItem() == LED_BLINKING :
         lLNAddress.setText("1")    
         lConst1.setText("0")
         lConst2.setText("255")
         lConst3.setText("12")
         lConst1.setEnabled(True)
         lConst2.setEnabled(True)
         lConst3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Setting the intensity when off:  range 0-FV2")
         lConst2.setToolTipText("Setting the intensity when on:  range FV1-255")
         lConst3.setToolTipText("Transistion period:  range 1-255  1=slow; 255=fast")
         lFunction.setText("66")
	 adresBox.setSelectedItem("1")  
     elif  functieBox.getSelectedItem() == RELAY :
         lLNAddress.setText("1")    
         lConst1.setText("0")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(True)
         lConst2.setEnabled(True)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Time in seconds before output set high:  range 0-255")
         lConst2.setToolTipText("Time in seconde how long output stays high : range 0-255 if 0 then output stays high")
         lConst3.setToolTipText("")
         lFunction.setText("8") 
	 adresBox.setSelectedItem("1") 
     elif  functieBox.getSelectedItem() == COIL1 :
         lLNAddress.setText("1")    
         lConst1.setText("255")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(True)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("16") 
	 adresBox.setSelectedItem("1")
     elif  functieBox.getSelectedItem() == COIL2 :
         lLNAddress.setText("1")   
         lConst1.setText("255")
         lConst2.setText("0")
         lConst3.setText("0")
         lConst1.setEnabled(True)
         lConst2.setEnabled(False)
         lConst3.setEnabled(False)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Time in milliseconds how long output stays ON:  range 1-255")
         lConst2.setToolTipText("")
         lConst3.setToolTipText("")
         lFunction.setText("18") 
	 adresBox.setSelectedItem("1")
     else : #functieBox.getSelectedItem() == SERVO
         lLNAddress.setText("1")
         lConst1.setText("100")     
         lConst2.setText("200")
         lConst3.setText("255")
         lConst1.setEnabled(True)
         lConst2.setEnabled(True)
         lConst3.setEnabled(True)
         lLNAddress.setEnabled(True)
	 selectBox.setEnabled(False)
	 adresBox.setEnabled(False)
         lConst1.setToolTipText("Minimum pulse time servo:  range 50-FV2  (100=1ms)")
         lConst2.setToolTipText("Maximum pulse time servo:  range FV1-250  (200=2ms)")
         lConst3.setToolTipText("Transistion period:  range 1-255")
         lFunction.setText("128")
	 adresBox.setSelectedItem("1") 
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


loadButton = javax.swing.JButton("LOAD")
loadButton.actionPerformed = whenLoadButtonClicked
loadButton.setToolTipText("Load Port data from Device")

initButton = javax.swing.JButton("INIT")
initButton.actionPerformed = whenInitButtonClicked
initButton.setToolTipText("Init Port function")


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

selectBox = javax.swing.JComboBox()
selectBox.addItem("NOP")
selectBox.addItem("DIR=1")
selectBox.addItem("DIR=0")
selectBox.setToolTipText("Select DIR LN Address")


# create fields

lDevice  = javax.swing.JTextField("1",3)
lFunction = javax.swing.JTextField("1",3)   
lLNAddress = javax.swing.JTextField("1",4)
lConst1 = javax.swing.JTextField("0",3)
lConst2 = javax.swing.JTextField("0",3)
lConst3 = javax.swing.JTextField("0",3)
lBinair = javax.swing.JTextField("0",3)

lDevice.setToolTipText("Device number, select from 1 to 127")

lLNAddress.setToolTipText("LocoNet Address, select from 0 to 2047")
   
# create a frame to  display the buttons and panels:

f = javax.swing.JFrame("LocoNet IO device program tool.           Version 1.1           by Geert Giebens")
f.contentPane.setLayout(javax.swing.BoxLayout(f.contentPane, javax.swing.BoxLayout.Y_AXIS))

panelDeviceNr= javax.swing.JPanel()    
panelDeviceNr.add(javax.swing.JLabel("Device:"))         
panelDeviceNr.add(lDevice)

panelConst1 = javax.swing.JPanel()	
panelConst1.add(javax.swing.JLabel("FV1:"))	
panelConst1.add(lConst1)

panelConst2 = javax.swing.JPanel()	
panelConst2.add(javax.swing.JLabel("FV2:"))	
panelConst2.add(lConst2)

panelConst3 = javax.swing.JPanel()	
panelConst3.add(javax.swing.JLabel("FV3:"))	
panelConst3.add(lConst3)

panelLNAdres = javax.swing.JPanel()	
panelLNAdres.add(javax.swing.JLabel("LN Address:"))	
panelLNAdres.add(lLNAddress)

panelDevice = javax.swing.JPanel()
panelDevice.add(panelDeviceNr)
panelDevice.add(resetButton)

panelPort = javax.swing.JPanel()	
panelPort.add(portBox)
panelPort.add(loadButton)
panelPort.add(functieBox)
panelPort.add(initButton)
panelPort.add(panelConst1)
panelPort.add(panelConst2)
panelPort.add(panelConst3)
panelPort.add(adresBox)
panelPort.add(selectBox)
panelPort.add(panelLNAdres)
panelPort.add(sendButton)

f.contentPane.add(panelDevice)
f.contentPane.add(panelPort)

f.pack()
f.show()


