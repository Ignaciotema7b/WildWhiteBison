#pragma once
#include <random>
using namespace std;

class Prize
{
public:
	Prize();
	~Prize();

	unsigned symbol_ID;

	unsigned xN;
	unsigned payout;
	bool isScatter;

	vector<unsigned> wildSubst;

};

//Para ordenar premios en forma descendente por "payout"
struct greater_than_payout
{
	inline bool operator() (const Prize& struct1, const Prize& struct2)
	{
		return (struct1.payout > struct2.payout);
	}
};

 