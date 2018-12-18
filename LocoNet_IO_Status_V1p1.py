import java
import javax.swing
import java.awt
typePacket = 0

OPC_PEER_XFER = 0xE5

CODE_UPLOAD    = 0x10    #MASTER asks Port-Data from Device
CODE_UPDATE    = 0x11    #MASTER update new Port Data in Device
CODE_DATA      = 0x12    #Device transmit requested data to MASTER
CODE_RESET     = 0x13    #RESET Device
CODE_COUNTERS  = 0x14    #MASTER asks for the status counters from Device
CODE_UPDATE_L  = 0x15    #MASTER update new Logical-Port Data in Device
CODE_UPLOAD_L  = 0x16    #MASTER asks Logical-Port-Data from Device
CODE_DATA_L    = 0x17    #Device transmit requested Logical-Port-Data to MASTER
CODE_DATA_DC1  = 0x20    #DEVICE transmit requested status COUNTERS to MASTER:[LBCL,LBCH,SBCL,SBCH,SBCHH,RBCL,RBCH,RBCHH]
CODE_DATA_DC2  = 0x21    #DEVICE transmit requested status COUNTERS to MASTER:[FECL,FECH,CECL,CECH,DSCL,DSCH,UAACL,UAACH]
CODE_DATA_DC3  = 0x22    #DEVICE transmit requested status COUNTERS to MASTER:[SMCL,SMCH,RMCL,RMCH,OACL,OACH,RFCL,RFCH]
CODE_RESET_C   = 0x30    #RESET DEVICE Counters

class MyLnListener_S (jmri.jmrix.loconet.LocoNetListener) :


    def message(self,msg) :

         if ((msg.getElement(0)==OPC_PEER_XFER) and (msg.getElement(2)==CODE_DATA_DC1) and (msg.getElement(3) == int(lDevice_S.text))) : 
              lRFC.setText(str( (msg.getElement(6) +(msg.getElement(5) & 1)*128) + ( (msg.getElement(7)+(msg.getElement(5) & 2)*64) *256 )))
              lSBC.setText(str( (msg.getElement(8) +(msg.getElement(5) & 4)*32) + ((msg.getElement(9)+(msg.getElement(5) & 8)*16)*256) + ((msg.getElement(11) +(msg.getElement(10) & 1)*128)*65536)))
              lRBC.setText(str( (msg.getElement(12)+(msg.getElement(10) & 2)*64) + ((msg.getElement(13) +(msg.getElement(10) & 4)*32)*256) + ((msg.getElement(14)+(msg.getElement(10) & 8)*16)*65536)))
			  
         if ((msg.getElement(0)==OPC_PEER_XFER) and (msg.getElement(2)==CODE_DATA_DC2) and (msg.getElement(3) == int(lDevice_S.text))) : 
              lFEC.setText(str((msg.getElement(6) +(msg.getElement(5) & 1)*128) + ((msg.getElement(7)+(msg.getElement(5) & 2)*64)*256)))
              lCEC.setText(str((msg.getElement(8) +(msg.getElement(5) & 4)*32) + ((msg.getElement(9)+(msg.getElement(5) & 8)*16)*256)))
              lOAC.setText(str((msg.getElement(11) +(msg.getElement(10) & 1)*128) + ((msg.getElement(12)+(msg.getElement(10) & 2)*64)*256)))
              lUAAC.setText(str((msg.getElement(13) +(msg.getElement(10) & 4)*32) + ((msg.getElement(14)+(msg.getElement(10) & 8)*16)*256)))	

         if ((msg.getElement(0)==OPC_PEER_XFER) and (msg.getElement(2)==CODE_DATA_DC3) and (msg.getElement(3) == int(lDevice_S.text))) : 
              lSMC.setText(str((msg.getElement(6) +(msg.getElement(5) & 1)*128) + ((msg.getElement(7)+(msg.getElement(5) & 2)*64)*256)))		  
              lRMC.setText(str((msg.getElement(8) +(msg.getElement(5) & 4)*32) + ((msg.getElement(9)+(msg.getElement(5) & 8)*16)*256)))
              lDSC.setText(str((msg.getElement(11) +(msg.getElement(10) & 1)*128) + ((msg.getElement(12)+(msg.getElement(10) & 2)*64)*256)))			  	
              lLBC.setText(str((msg.getElement(13) +(msg.getElement(10) & 4)*32) + ((msg.getElement(14)+(msg.getElement(10) & 8)*16)*256)))	
	
              lSMs.setText(str(round((float(lSMC.text)/float(lDSC.text)),3)))
              lRMs.setText(str(round((float(lRMC.text)/float(lDSC.text)),3)))
              lRMLB.setText(str(round((float(lRMC.text)/float(lLBC.text)),2)))
  
         return

my_LnTrafContInstance_S = jmri.jmrix.loconet.LnTrafficController.instance()
my_LnTrafContInstance_S.addLocoNetListener(0xFF,MyLnListener_S())


def whenResetButtonClicked_S(event) :

     msgLength = 16
     opcode = OPC_PEER_XFER
     ARG1 =  msgLength
     ARG2 =  CODE_RESET_C #BRON = RESET DEVICE COUNTERS
     ARG3 =  0x00	#int(lDevice_S.text)  
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

     sendLoconetMsg_S(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)
     return
	 
def whenLoadButtonClicked_S(event) :

     msgLength = 16
     opcode = OPC_PEER_XFER
     ARG1 =  msgLength
     ARG2 =  CODE_COUNTERS  #LOAD Status Counters
     ARG3 =  int(lDevice_S.text)  
     ARG4 =  0x00 
     ARG5 =  0x00
     ARG6 =  0x00 
     ARG7 =  0x00
     ARG8 =  0x00
     ARG9 =  0x00
     ARG10 = 0x00
     ARG11 = 0x00
     ARG12 = 0x00
     ARG13 = 0x00
     ARG14 = 0x00
     ARG15 = 0 

     sendLoconetMsg_S(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15)
     return	 

 

def sendLoconetMsg_S(msgLength,opcode,ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,ARG11,ARG12,ARG13,ARG14,ARG15) :

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


loadButton_S = javax.swing.JButton("LOAD")
loadButton_S.actionPerformed = whenLoadButtonClicked_S
loadButton_S.setToolTipText("Load Port data from Device")

resetButton_S = javax.swing.JButton("RESET ALL DEVICE STATUS COUNTERS")
resetButton_S.actionPerformed = whenResetButtonClicked_S
resetButton_S.setToolTipText("Reset device Status Counters")
resetButton_S.setForeground(java.awt.Color.BLUE)


# create fields

lDevice_S  = javax.swing.JTextField("1",3)
lDevice_S.setToolTipText("Device number, select from 1 to 127")

lLBC = javax.swing.JTextField("0",5)	#LineBreak Counter

lSBC  = javax.swing.JTextField("0",8)	#Sent Byte Counter

lRBC  = javax.swing.JTextField("0",8)	#read Byte Counter

lRMC  = javax.swing.JTextField("0",5)	#Read Message Counter

lSMC  = javax.swing.JTextField("0",5)	#Send Message Counter

lFEC  = javax.swing.JTextField("0",5)	#Framing Error Counter

lCEC  = javax.swing.JTextField("0",5)	#Checksum Error Counter
lCEC.setForeground(java.awt.Color.RED)

lDSC  = javax.swing.JTextField("0",5)	#Device Seconds Counter

lUAAC  = javax.swing.JTextField("0",5)	#Unsuccessful Access Attempts Counter

lOAC = javax.swing.JTextField("0",5)	#Output Actions Counter

lRFC = javax.swing.JTextField("0",5)	#Ring buffer Full Counter
lRFC.setForeground(java.awt.Color.RED)

lSMs = javax.swing.JTextField("0",5)	#Send Message / Device Secondes

lRMs = javax.swing.JTextField("0",5)	#Read Message / Device Secondes

lRMLB = javax.swing.JTextField("0",5)	#Read Message / LineBreak
lRMLB.setToolTipText("Read Messages / LineBreaks")


# create a frame to  display the buttons and panels:

f = javax.swing.JFrame("LocoNet IO Status Device   by Geert Giebens")
f.contentPane.setLayout(javax.swing.BoxLayout(f.contentPane, javax.swing.BoxLayout.Y_AXIS))

panelDeviceNr_S= javax.swing.JPanel()    
panelDeviceNr_S.add(javax.swing.JLabel("Device:"))         
panelDeviceNr_S.add(lDevice_S)

panelDevice_S = javax.swing.JPanel()
panelDevice_S.add(panelDeviceNr_S)
panelDevice_S.add(loadButton_S)
panelDevice_S.add(resetButton_S)

panelLBC=javax.swing.JPanel()
panelLBC.add(javax.swing.JLabel("Detected Linebreaks"))
panelLBC.add(lLBC)
panelLBC.add(javax.swing.JLabel("Read Messages/Linebreaks"))
panelLBC.add(lRMLB)


panelSBC=javax.swing.JPanel()
panelSBC.add(javax.swing.JLabel("Sent Bytes, including re-sent"))
panelSBC.add(lSBC)

panelRBC=javax.swing.JPanel()
panelRBC.add(javax.swing.JLabel("Total Read Bytes, including re-sent"))
panelRBC.add(lRBC)

panelRMC=javax.swing.JPanel()
panelRMC.add(javax.swing.JLabel("Total Correct Read Messages"))
panelRMC.add(lRMC)
panelRMC.add(lRMs)
panelRMC.add(javax.swing.JLabel("/Second"))

panelSMC=javax.swing.JPanel()
panelSMC.add(javax.swing.JLabel("Correct Sent Messages"))
panelSMC.add(lSMC)
panelSMC.add(lSMs)
panelSMC.add(javax.swing.JLabel("/Second"))

panelFEC=javax.swing.JPanel()
panelFEC.add(javax.swing.JLabel("Detected Framing Errors (Hardware Receiver)"))
panelFEC.add(lFEC)

panelDSC=javax.swing.JPanel()
panelDSC.add(javax.swing.JLabel("Number of Seconds in Operation, since Last Reset (Device Clock)"))
panelDSC.add(lDSC)

panelUAAC=javax.swing.JPanel()
panelUAAC.add(javax.swing.JLabel("Unsuccessful Access Attempts (LocoNet not free)"))
panelUAAC.add(lUAAC)

panelOAC=javax.swing.JPanel()
panelOAC.add(javax.swing.JLabel("Received Output Actions"))
panelOAC.add(lOAC)

tMCE=javax.swing.JLabel("Messages Checksum Errors")
tMCE.setForeground(java.awt.Color.RED)
panelCEC=javax.swing.JPanel()
panelCEC.add(tMCE)
panelCEC.add(lCEC)

tMNS=javax.swing.JLabel("Messages not sent (Sent Buffer 255 byte Full")
tMNS.setForeground(java.awt.Color.RED)
panelRFC=javax.swing.JPanel()
panelRFC.add(tMNS)
panelRFC.add(lRFC)

f.contentPane.add(panelDevice_S)
f.contentPane.add(panelDSC)
f.contentPane.add(javax.swing.JSeparator(javax.swing. JSeparator.HORIZONTAL))
f.contentPane.add(panelRBC)
f.contentPane.add(panelSBC)
f.contentPane.add(panelRMC)
f.contentPane.add(panelSMC)
f.contentPane.add(panelOAC)
f.contentPane.add(javax.swing.JSeparator(javax.swing. JSeparator.HORIZONTAL))
f.contentPane.add(panelUAAC)
f.contentPane.add(panelFEC)
f.contentPane.add(panelLBC)
f.contentPane.add(javax.swing.JSeparator(javax.swing. JSeparator.HORIZONTAL))
f.contentPane.add(panelRFC)
f.contentPane.add(panelCEC)

f.pack()
f.show()


