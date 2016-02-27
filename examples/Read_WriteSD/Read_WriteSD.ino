#include <SD.h>
 
File file;
 
void setup(){
  
  Serial.begin(9600);
 
  Serial.print("Starting SD connection...");
  
  // declare default CS pin as OUTPUT
  pinMode(10, OUTPUT);
  digitalWrite(10,HIGH);
  
  // Is the connection successful?
  if (!SD.begin(4)){
    
    Serial.println("Error during connection...");
    return;
  }
  Serial.println("Successfully connected!");
  
  
  /*              WRITING DATA IN SD                   */
  
  // Open the file for the read and write.
  file = SD.open("data.txt", FILE_WRITE);
  
  
  // If the file is open, proceed...
  if (file){
    
    // Writie smth
    file.println("Writing in file");
    
    // Close file
    file.close();
    
    // Data saved.
    Serial.println("All data is saved");
  }
  
  // If something went wrong
  else{
 
    Serial.println("The file couldn't be opened");
  }
  
  
  /*            END OF WRITING IN SD          */
  
  
  /*                    READING DATA                 */
  
  
  // Openenig file
  file = SD.open("data.txt");
  
  // If the file is open, proceed...
  if (file){
    
    Serial.println("Data from file:");
    
    // Looping over the file until the end.
    // (file.available()==FALSE)
    while (file.available()){
      
     // Writing data from file
     Serial.write(file.read());
    }
    
    
    // If everything is okey, we close the file.
    file.close();
  }
  
  // If something went wrong...
  else{
 
    Serial.println("The file couldn't be opened");
  }
  
}
 
void loop()
{
  
}
