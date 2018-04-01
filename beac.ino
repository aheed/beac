/*
 * Copyright (c) Anders Heed 2018
 * Modified web server example. See below.
 * 
 * 
 * ----------------------------------------------------
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "morse.h"



const char *ssid = "yourWifiNetwork";
const char *password = "yourPassword";

MorseBlinker blinker;

ESP8266WebServer server ( 80 );



const char *response = " \
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\"> \
<html> \
<head> \
 <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/> \
  <title>Morse blinker</title> \
</head> \
<body lang=\"en-US\" dir=\"ltr\"> \
<p><br/> \
<br/> \
</p> \
<h1>Blink it!</h1> \
<p>Morse code blinker</p> \
<p><br/> \
<br/> \
</p> \
<hr/> \
<p><br/> \
<br/> \
</p> \
<p>Text message to blink:</p> \
<form name=\"texmsgform\" action=\"../morse\" method=\"post\"> \
  <p><input type=\"text\" name=\"textinput\" value=\"SOS\"  size=\"23\" style=\"width: 2in; height: 0.33in; font-family: 'Liberation Sans', sans-serif; font-size: 12pt\"></p> \
  <p style=\"margin-bottom: 0in\"><br/> \
  </p> \
  <p style=\"margin-bottom: 0in\"><input type=\"submit\" name=\"submit\" value=\"submit\"  style=\"width: 1.35in; height: 0.34in; font-family: 'Liberation Sans', sans-serif; font-size: 12pt\"></p> \
</form> \
</body> \
</html> \
";



void handleMorse()
{
  server.send ( 200, "text/html", response );
}

void handleMorsePost()
{

  int i;
  String morseWord;
  Serial.println("<<<< nofargs: " + String(server.args()));
  for (i = 0; i < server.args(); i++)
  {
    Serial.println( server.argName(i));
    Serial.println( server.arg(i));
    Serial.println( String(server.arg(i).length()));

    if(server.argName(i) == "textinput")
    {
      Serial.println("Got new message: \"" + server.arg(i) + "\"");
      morseWord = blinker.Encode(server.arg(i).c_str());
      morseWord += blinker.Encode("  "); //Adding double word separation at end of message
      blinker.SetMorseWord(morseWord);
    }
  }

 
  server.send ( 200, "text/html", response );
}


void handleNotFound() {

	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );

}

void setup ( void ) {

  blinker.Setup();
  
	Serial.begin ( 115200 );
	WiFi.mode ( WIFI_STA );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	server.on ( "/", HTTP_GET, handleMorse );
  server.on ( "/morse", HTTP_GET, handleMorse );
  server.on ( "/morse", HTTP_POST, handleMorsePost );
  server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}


void loop ( void ) {
  blinker.HandleBlink();
	server.handleClient();
}


