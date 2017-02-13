/***************************************************************************
* nameservercpp  -  Program to serve of last name statistics
 *
* copyright : (C) 2009 by Jim Skon
*
* This program runs as a background server to a CGI program, providinging US Census
* Data on the frequency of names in response to requestes.  The rational for this 
* is to demonstrait how a background process can serve up information to a web program,
* saving the web program from the work of intiallizing large data structures for every
* call.
* 
* 
*
***************************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include "fifo.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

/* Fifo names */
string receive_fifo = "Namerequest";
string send_fifo = "Namereply";

vector <int> indexSearch(string word);
map<string, vector<int> > refs;


vector <int> indexSearch(string word){
  map<string, vector<int> >::iterator it;  // iterator for find
  vector<int> blank; // return for no matches
  /* find the word and get the vector of references */
  it = refs.find(word);
  if (it == refs.end()) {
    return(blank);
  } else {
    return (refs[word]);
  }

}
int main() {
  
  string inMessage,outMessage,name,percent;

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
    /* Get a message from a client */
    recfifo.openread();
    name = recfifo.recv();
	cout << "Message: "<< name << endl;
	outMessage = "";
	sendfifo.openwrite();
	string word,line,searchWord,foundLine;
  	ifstream infile("/home/class/SoftDev/Shakespeare/Shakespeare.txt");// open the file
  	ifstream displayFile("/home/class/SoftDev/Shakespeare/Shakespeare.txt");
  	int position=0;
  	while (!infile.fail()) {
   		getline(infile,line); // get the next line of code
    	istringstream lineStream(line); // Create a string stream of the line
	    while (lineStream >> word) { // get the next word
   		 refs[word].push_back(position); // push the word and the line position on the vector for this word
   		}
   		position = infile.tellg(); // get the poistion of the next line
  	}
 	vector<int> index = indexSearch(name);
  	vector<int>::iterator myIntVectorIterator;
  	cout << "results: "<< index.size() << endl;
  	int i = 0;
  	for(myIntVectorIterator = index.begin();
      	myIntVectorIterator != index.end();
      	myIntVectorIterator++)
    {		
      i++;
      displayFile.seekg(*myIntVectorIterator, displayFile.beg);
      getline(displayFile,foundLine);
      outMessage = "<p>"+foundLine+"<p>";
      cout << " Result"<< i <<": " << outMessage << endl;
      sendfifo.send(outMessage);
    }	
	outMessage = "$END";
	sendfifo.send(outMessage);
	sendfifo.fifoclose();
	recfifo.fifoclose();
}
