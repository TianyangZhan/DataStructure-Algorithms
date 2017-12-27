#include "StackLi.h"
using namespace std;

class LongInt{
    public:
        // Constructors
        LongInt();
    
        // Functions
        friend LongInt operator+(LongInt& num1, LongInt& num2);
        friend istream& operator>>(istream& is, LongInt& num);
        friend ostream& operator<<(ostream& os, LongInt& num);
    
        // Variables
        StackLi<int> SLnum; // create a Stack List to store values
};
