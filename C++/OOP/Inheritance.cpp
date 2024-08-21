/*
  Inheritance
  Author: Seyed Sobhan Hosseini
  Date: Sunday, October 1, 2023, 5:21:32 PM
*/

#include <iostream>

using namespace std;

typedef char String20[21];

class Address{
	public:
		virtual void Write() const;
		Address(
			string newStreet,
			string newCity,
			string newState,
			string newZip
		);
	private:
		String20 street;
		String20 city;
		String20 state;
		String20 zipCode;
};

class InterAddress: public Address{
	public:
		virtual void Write() const override;
		Address override(
			string newStreet,
			string newCity,
			string newState,
			string newZip
		);
	private:
		String20 countryCode;
};

int main(){
	
	return 0;
}
