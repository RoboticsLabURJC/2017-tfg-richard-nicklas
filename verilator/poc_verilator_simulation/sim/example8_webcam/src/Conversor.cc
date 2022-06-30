//ROS & opencv
#include <Conversor.h>
#include <iostream>
#include <bitset>


using namespace std;

  //static double Conversor::DisToV(void * dis){
  double Conversor::DisToV(bitset<3> dis){

	cout << dis << endl;
	
	return 0.1; 
  }


  //double PosToW(void * pos){
  //double PosToW(bitset<8> pos){
  double PosToW(unsigned char *pos){
  

	cout << pos << endl;

	return 0.1;
  }


