#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h"
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  4;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
const String CLIENT_ID = "1027676406829-jioum6bp454oomn41vlevv2mlfkm7m8p.apps.googleusercontent.com";
const String CLIENT_SECRET = "_viNzWO7a5xeNuwM6B9pBufv";
const String REFRESH_TOKEN = "1/K5Q3tTEO9DmVSG88IyoBHh78TwjkiQGo5tCcjZif3Nw";
const String SPREADSHEET_ID = "1U6WkNuvD1rrQgrVizimstRyrXRz8AVj5IWRV6YODDpc";
String alert = "intruder alert";

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Bridge.begin();  // make contact with the linux processor
  Console.begin();
  //Serial.begin(9600);
  delay(1000);
  //while(!Serial);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    
    Console.println("Getting sensor value...");

    // get the value we want to append to our spreadsheet
    int sensorValue = buttonState;

    Console.println("Appending value to spreadsheet...");

    // we need a Process object to send a Choreo request to Temboo
    TembooChoreo AppendValuesChoreo;

    // invoke the Temboo client
    // NOTE that the client must be reinvoked and repopulated with
    // appropriate arguments each time its run() method is called.
    AppendValuesChoreo.begin();

    // set Temboo account credentials
    AppendValuesChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendValuesChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendValuesChoreo.setAppKey(TEMBOO_APP_KEY);

    // identify the Temboo Library choreo to run (Google > Sheets > AppendValues)
    AppendValuesChoreo.setChoreo("/Library/Google/Sheets/AppendValues");

    // set the required Choreo inputs
    // see https://www.temboo.com/library/Library/Google/Sheets/AppendValues/
    // for complete details about the inputs for this Choreo

    // your Google application client ID
    AppendValuesChoreo.addInput("ClientID", CLIENT_ID);
    // your Google application client secret
    AppendValuesChoreo.addInput("ClientSecret", CLIENT_SECRET);
    // your Google OAuth refresh token
    AppendValuesChoreo.addInput("RefreshToken", REFRESH_TOKEN);

    // the ID of the spreadsheet you want to append to
    AppendValuesChoreo.addInput("SpreadsheetID", SPREADSHEET_ID);

    // convert the time and sensor values to a comma separated string
    String rowData = "[[\"" +  String(sensorValue) + "\", \"" + (alert) + "\"]]";

    // add the RowData input item
    AppendValuesChoreo.addInput("Values", rowData);

    // run the Choreo and wait for the results
    // The return code (returnCode) will indicate success or failure
    unsigned int returnCode = AppendValuesChoreo.run();

    Console.println("Success! Appended " + rowData);
    Console.println("");

    AppendValuesChoreo.close();
  }
      else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}



