#include <string>

std::string generateRandomAlphaNumericString(int length) {
  std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::string output = "";

  for(int i=0; i<length; i++) {
    output += chars[rand() % chars.length()];
  }

  return output;
}
