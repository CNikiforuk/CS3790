#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;



int main (void){

string mask = "172.20.121.";

for(int i=1; i<256;i++){
	string c = "arping -I eth0 -f -c 1 -V"+mask+to_string(i);
	system(c.c_str());
}


}
