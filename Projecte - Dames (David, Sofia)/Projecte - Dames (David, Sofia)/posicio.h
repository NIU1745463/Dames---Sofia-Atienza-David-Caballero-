#ifndef POSICIO_H
#define POSICIO_H

#include <iostream>
using namespace std;

class Posicio
{
public:
	Posicio(const string& posicio);
	bool operator==(const Posicio& posicio) const;
private:

};

#endif