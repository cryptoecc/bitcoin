#pragma once
#include <cmath>
#include <uint256.h>

class LDPC
{
public:

  LDPC();
  ~LDPC();

  void set_difficulty(int level);
  bool set_difficulty(int n, int wc, int wr);
  bool initialization();
  bool is_regular(int n, int wc, int wr);

  int  generate_seed(char phv[]);  //the date type of the previous hash value included in the bitcoin header is the char array of size 32
  void generate_seed(unsigned long long hash);
  bool generate_H();
  bool generate_Q();
  void generate_hv(const unsigned char Seralized_Block_Header_with_Nonce[]);
//  bool CheckProofOfWork(uint256 currHash, uint256 prevHash, unsigned int nBits);

  void decoding();
  bool decision();

  void print_H(const char name[]);
  void print_Q(const char name[], int type);
  void print_word(const char name[], int type);



private:
  #define BIG_INFINITY    1000000.0
  #define Inf          64.0
  int  *hash_vector = NULL;
  int  *output_word = NULL;

  unsigned char tmp_hash_vector[32];  //32bytes => 256bits

  int **H = NULL;
  int **row_in_col = NULL;
  int **col_in_row = NULL;

  int n, m, wc, wr;
  unsigned long long seed;

  // these parameters are only used for the decoding function.
  int    max_iter = 20;   // the maximum number of iteration in the decoding function. We fix it.
  double cross_err = 0.01; // a transisient error probability. this is also fixed as a small value.

  double *LRft = NULL;
  double *LRpt = NULL;
  double **LRrtl = NULL;
  double **LRqtl = NULL;

  // these functions are only used for the decoding function.
  double func_f(double x);
  double infinity_test(double x);
};
