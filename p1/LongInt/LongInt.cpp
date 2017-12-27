#include <iostream>
#include "StackLi.h"
#include "LongInt.h"

using namespace std;


LongInt::LongInt(){}

LongInt operator+(LongInt& n1, LongInt& n2){
    
    // declare variables
    LongInt sum;
    StackLi<int> num1 = n1.SLnum;
    StackLi<int> num2 = n2.SLnum;
    StackLi<int>& num3 = sum.SLnum;
    int digitSum = 0;
    bool carry = false;
    
    // start addition, pause when the small number becomes empty
    while(!num1.isEmpty() && !num2.isEmpty()){
        digitSum = num1.topAndPop() + num2.topAndPop();
        
        if(carry){
            carry = false;
            digitSum++;
        }
        
        if(digitSum > 9) {
            carry = true;
            digitSum -= 10;
        }
        
        num3.push(digitSum);
    }
    
    // swap the larger number to n1
    if(num1.isEmpty())
        num1 = num2;
    
    // proceed addition
    while(!num1.isEmpty()) {
        digitSum = num1.topAndPop();
        
        if(carry) {
            carry = false;
            digitSum++;
        }
        if(digitSum > 9) {
            carry = true;
            digitSum -= 10;
        }
        num3.push(digitSum);
    }
    
    // if the sum of two most significant digit carries
    if(carry) {
        carry = false;
        num3.push(1);
    }
    
    return sum;
}

istream& operator>>(istream& is, LongInt& num){
    
	// declare variables
    char c;
    
    // read in numbers
    do {
        is.get(c);
        if(isdigit(c))
            num.SLnum.push(c - '0');    // convert char to int
    }while(isdigit(is.peek()));

	return is;
}

ostream& operator<<(ostream& os, LongInt& num){
    
    // declare variables
    int digit;
	StackLi<int> SLnum = num.SLnum;

    // print out numbers
	while(!SLnum.isEmpty()){
		digit = SLnum.topAndPop();
		cout<<digit;
	}
    
	return os;
}
