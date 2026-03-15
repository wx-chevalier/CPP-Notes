// nested.cpp -- using a queue that has a nested class
#include <iostream>

#include "queuetp.h"
#include <string>

int main() {
  using std::cin;
  using std::cout;
  using std::string;

  QueueTP<string> cs(5);
  string temp;

  while (!cs.isfull()) {
    cout << "Please enter your name. You will be "
            "served in the order of arrival.\n"
            "name: ";
    getline(cin, temp);
    cs.enqueue(temp);
  }
  cout << "The queue is full. Processing begins!\n";

  while (!cs.isempty()) {
    cs.dequeue(temp);
    cout << "Now processing " << temp << "...\n";
  }
  // cin.get();
  return 0;
}
