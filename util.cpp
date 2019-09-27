#include <string>

std::string generateRandomString(int length) {
  std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::string output = "";

  for(int i=0; i<length; i++) {
    output += chars[rand() % chars.length()];
  }

  return output;
}
