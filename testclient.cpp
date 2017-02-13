#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fifo.h"

using namespace std;
string receive_fifo = "Namereply";
string send_fifo = "Namerequest";

int main() {
  string message, name;
  string reply=" ";
	cout << "Enter a word:";
	cin >>  name;
	message = name;
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
	cout << "Send:" << message << endl;
	sendfifo.openwrite();
	sendfifo.send(message);
	recfifo.openread();
	while (reply.find("$END") == string::npos) {
	reply = recfifo.recv();
	cout << "Server sent: " << reply << endl;
  }
recfifo.fifoclose();
sendfifo.fifoclose();

}
