#include <string>
#include <iostream>
using namespace std;
bool Guess(int number) {
}
int main()
{
	int guess;
	do {
		std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
