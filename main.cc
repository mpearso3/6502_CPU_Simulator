#include <iostream>
#include <vector>

using namespace std;

struct OneBitAdder {
  unsigned sum;
  unsigned carry_out;

  OneBitAdder (const unsigned sum, const unsigned carry_out) : sum(sum & 0x1), carry_out(carry_out & 0x1) {}
};

OneBitAdder fullBitAdder (unsigned A, unsigned B, unsigned carry_in)
{
  A &= 0x1;
  B &= 0x1;
  carry_in &= 0x1;

  const unsigned xor1_out = A ^ B;
  const unsigned sum = xor1_out ^ carry_in;

  const unsigned and1_out = A & B;
  const unsigned and2_out = xor1_out & carry_in;

  const unsigned carry_out = and1_out | and2_out;

  return OneBitAdder (sum, carry_out);
}

unsigned multipleBitAdder (const unsigned num_bits, const unsigned A, const unsigned B)
{
  unsigned result = 0;

  OneBitAdder adder(0, 0);

  for (unsigned bit = 0; bit < num_bits; bit++) {
    const unsigned a = (A & (0x1 << bit)) >> bit;
    const unsigned b = (B & (0x1 << bit)) >> bit;
    adder = fullBitAdder (a, b, adder.carry_out);
    result |= (adder.sum << bit);
  }

  return result;
}

void UNIT_TEST_1_bit_adder ()
{
  cout << endl << "UNIT_TEST_" << 1 << "_bit_adder" << endl;

  vector <OneBitAdder> added_bits;
  added_bits.push_back (fullBitAdder (0x0, 0x0, 0x0));
  added_bits.push_back (fullBitAdder (0x0, 0x1, 0x0));
  added_bits.push_back (fullBitAdder (0x1, 0x0, 0x0));
  added_bits.push_back (fullBitAdder (0x1, 0x1, 0x0));
  added_bits.push_back (fullBitAdder (0x0, 0x0, 0x1));
  added_bits.push_back (fullBitAdder (0x0, 0x1, 0x1));
  added_bits.push_back (fullBitAdder (0x1, 0x0, 0x1));
  added_bits.push_back (fullBitAdder (0x1, 0x1, 0x1));

  for (vector <OneBitAdder>::const_iterator it = added_bits.begin(); it != added_bits.end(); ++it) {
    OneBitAdder bit = *it;
    cout << bit.carry_out << " " << bit.sum << endl;
  }
}

void UNIT_TEST_x_bit_adder (const unsigned num_bits)
{
  cout << endl << "UNIT_TEST_" << num_bits << "_bit_adder" << endl;

  struct Equations {
    unsigned A;
    unsigned B;

    Equations (unsigned A, unsigned B) : A(A), B(B) {}
  };

  vector <Equations> equations;
  for (unsigned a = 0; a < 0xf; a++) {
    equations.push_back (Equations (a, 0));
  }
  for (unsigned b = 0; b < 0xf; b++) {
    equations.push_back (Equations (0, b));
  }
  for (unsigned i = 0; i < 0xf; i++) {
    equations.push_back (Equations (i, i));
  }

  for (vector <Equations>::const_iterator it = equations.begin(); it != equations.end(); ++it) {
    const unsigned A = it->A;
    const unsigned B = it->B;

    cout << A << " + " << B << " = " << multipleBitAdder (num_bits, A, B) << endl;
  }
}

int main ()
{
  UNIT_TEST_1_bit_adder ();
  UNIT_TEST_x_bit_adder (4);
  UNIT_TEST_x_bit_adder (8);

  return 0;
}
