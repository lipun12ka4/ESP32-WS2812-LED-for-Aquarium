#This project is aimed to Build my Own Aquarum LED Light, with adjustable Intensity and Color 

#Planned Features  

1.Control over Wi-Fi Locally  
2.Control Remotely using MQTT  
3.WS18B2 LED Strips (8x8 Matrix X 2 and 8x8 Matrix  X 4)  


#Implemented Features  

1. UART0 Debug (Command-Response)  
2. State Machine to Apply Commands
3. JSON Structure of Command-Response
4. LED Driver by NeoPixel for ESP32
5. Command to set LED Color

	{  
		"Command":"FILL LED STRIP COLOR",  
		"Red":"255",  
		"Green":"0",  
		"Blue":"0"  	
	};  
	


#TODO: Update this File
