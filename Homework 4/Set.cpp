#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

class Coord
{
public:
	Coord(int r, int c) : m_r(r), m_c(c) {}
	Coord() : m_r(0), m_c(0) {}
	double r() const { return m_r; }
	double c() const { return m_c; }
	bool operator==(const Coord& rhs)
	{
		if (m_r == rhs.r() && m_c == rhs.c())
			return true;
		else
			return false;
	}
private:
	double m_r;
	double m_c;
};

void test()
{
	Set<int> si;
	Set<string> ss;
	assert(si.empty());
	assert(ss.empty());
	assert(si.size() == 0);
	assert(ss.size() == 0);
	assert(si.insert(10));
	assert(ss.insert("hello"));
	assert(si.contains(10));
	assert(ss.contains("hello"));
	int i;
	assert(si.get(0, i) && i == 10);
	string s;
	assert(ss.get(0, s) && s == "hello");
	assert(si.erase(10));
	assert(ss.erase("hello"));
	Set<int> si2(si);
	Set<string> ss2(ss);
	si.swap(si2);
	ss.swap(ss2);
	si = si2;
	ss = ss2;
	unite(si, si2, si);
	unite(ss, ss2, ss);
	subtract(si, si2, si);
	subtract(ss, ss2, ss);
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}
