#include<bitset>


class Conversor
{
public:

// static double DisToV(void * dis);//bitset<3>
  static double DisToV(std::bitset<3> dis);

//  static double PosToW(void * pos);//bitset<8>
//  static double PosToW(std::bitset<8> pos);
  static double PosToW(unsigned char *pos);

};


