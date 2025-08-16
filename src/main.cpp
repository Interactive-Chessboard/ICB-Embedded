//#include <Arduino.h> //uncomment pour esp32
#include <iostream>
#include "func.hpp"


void setup() {
  int result = Func::myFunction(2, 3);
  std::cout << "hi from set up " << result << std::endl;
}

void loop() {
  std::cout << "hi from loop" << std::endl;
}





// Mimics the arduino ide process
int main() 
{
  setup();
  while (true)
  {
    loop();
    break;
  }
  return 0;
}