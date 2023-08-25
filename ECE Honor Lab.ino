#include <Keypad.h>
#include <String.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3', },
  { '4', '5', '6', },
  { '7', '8', '9', },
  { '*', '0', '#' }
};


uint8_t colPins[COLS] = { 4, 3, 2 }; 
uint8_t rowPins[ROWS] = { 12, 7, 6, 5 }; 
//pin 12, not 8, is used because physically there is a gap between pin 7 and pin8 
//so we connected the very left pin(R1) of the keypad to pin 12 with a jump wire.

char phoneNumber[20] = "9293916485"; //phone number to be sent to

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/*
  To-do codes/works:
  1. Power & unpower the linear actuator
  2. Send signal to the GSM sheild

  Notes:
  Serial Outputs will be displayed in the Serial Monitor, the shortcut keys are: Ctrl + Shift + M
  Auto scroll is the first icon at the right top corner, the double arrow pointing down
*/

/*---------------------------Headers only for our project---------------------------*/

String userInput = "";
int passcode = -1;

/*

  @desc: Checking whether the last 4 digits of string 'a' matches up with an integer 'x'.
  @param: String a, int x;
  @return: bool - true or false.

*/
bool passcodeCheck(String a, int x){ 

  int num = x;

	if (a.length() < 4) {Serial.println("Reason: the passcode entered is too short"); return false;}

  Serial.println("Checking: ");

  /* Unknown bug: when the passcode entered is exact 4 digit, the loop will run into position at -1
  for (int i = a.length() - 1; i >= a.length() - 4; i--){
      Serial.print("i = "); Serial.print(i); Serial.print(": ");
      Serial.print(num % 10);
      Serial.print(" and ");
      Serial.print(a[i] - '0');
      Serial.println();

      if (num % 10 != (a[i] - '0')) {Serial.println("Reason: the passcode entered doesn't match up"); return false;}
      num /= 10;
  }
  */

  for (int i = 1; i <= 4; i++){

      //Serial.print("i = "); Serial.print(i); Serial.print(": ");
      //Serial.print(num % 10);
      //Serial.print(" and ");
      //Serial.print(a[a.length() - i] - '0');
      //Serial.println();

    if (num % 10 != (a[a.length() - i] - '0')) {Serial.println("Reason: the passcode entered doesn't match up"); passcode = -1; return false;}
      num /= 10;
  }

  passcode = -1;
  return true;
}

/*
 
  @desc: Control the LED light on the Arduino board blinking x times.
  @param: int x;
  @return - void - no return.

*/
void blinking(int x){

  for (int i = 1; i <= x ; i++){
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);                      
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(100);   
  }

  return ;
}

/*
  Setting up:
  1. Start the Serial
  2. Set up the LED funtion
  3. Initialize the random number generator
*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  randomSeed(analogRead(0)); // randomSeed() initializes the pseudo-random number generator to start at an arbitrary point in its random sequence
                             // analogRead() on an unconnected pin(0 at this project) initializes the random number generator with a fairly random input

  //srand((unsigned) time(0)); // Set up command for real random number generator (tested to be useless)

  Serial.println("Program Set Up Ready");

}



// run repeatedly:
void loop() {

  // get the digit input
  char key = keypad.getKey();

  if (key != NO_KEY){

    blinking(3);
    //Toggle bottom actived, Door locked, Generate Code, Clear the previous inputs
    if (key == '#'){ 

      //Clear the input string:
      userInput = "";

      //The linear actuator extend to lock the door and we unpower it to keep it extended:


      //Generate a 4 digit passcode: From 1000 to 9999
      passcode = random()  % 9000 + 1000;  // Pseudo random

      //passcode = rand() % 9000 + 1000; // not working here
      Serial.print("New passcode generated: ");
      Serial.println(passcode);

      //Send passcode to the GSM: 


    }
    // '*' represents when the user want to check whether the passcode they inputed are correct
    else if (key == '*'){ 

      //Serial.print("Currect passcode entered: ");
      //Serial.println(userInput);

        //When the user correctly enter the code, we unlock the door:
        if (passcodeCheck(userInput, passcode) == true){

          Serial.println("The passcode is correct!");
          blinking(5);
          //unlock the door: power the linear actuator so the user can unlock it.


          //Clear the input string:
          //userInput = "";

        }
        else{
          Serial.println("The passcode is incorrect.");

        }
    }
    else { 

      // blinking((int)(key) - 48);
      
      Serial.print("Last input: ");
      Serial.println(key);

      // Store the inputs
      userInput = userInput + String(key); 

      //Serial.print("Currect passcode entered: ");
      //Serial.println(userInput);

      // if (passcodeCheck(userInput, 1234)) blinking(10); // test
    }
  }
}
