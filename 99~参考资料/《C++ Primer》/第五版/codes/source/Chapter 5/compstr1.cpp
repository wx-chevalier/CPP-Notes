// compstr1.cpp -- comparing strings using arrays
#include <cstring> // prototype for strcmp()
#include <iostream>
int main() {
  using namespace std;
  char word[5] = "?ate";

  for (char ch = 'a'; strcmp(word, "mate"); ch++) {
    cout << word << endl;
    word[0] = ch;
  }
  cout << "After loop ends, word is " << word << endl;
  // cin.get();
  return 0;
}
