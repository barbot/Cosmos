

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <math.h>
//#include <unistd.h>

using namespace std;



int main(int argc, char** argv) {
  string ss;
  
  ofstream file("linear.txt", ios::out | ios::trunc);
  file.close();
  for(int i=4;i<=8;i++){
    std::ostringstream st;
    st<<"./swn linear "<<i<<" "<<i;
    ss=st.str();
    system(ss.c_str());
  }
    
    return(EXIT_SUCCESS);
}