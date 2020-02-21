#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include "Trie.h"
#include <fstream>
using namespace std;

//int main()
//{
//	Trie<int> t;
//	t.insert("test1", 5);
//	t.insert("suh", 4);
//	t.insert("eest1", 3);
//	t.insert("tsst1", 2);
//	t.insert("teet1", 69);
//	vector<int> v;
//	//v = t.find("sup", false);
//	for (int i = 0; i < v.size(); i++)
//		cout << v[i] << endl;
//
//	ifstream inflile("C:/Users/ryanr/OneDrive/Documents/project4data/Desulfurococcus_mucosus.txt");
//	if (!inflile)
//		cout << "ERROR LOADING FILE" << endl;
//	string s;
//	//getline(infile, s);
//	vector<Genome> genomes;
//	bool success = Genome::load(inflile, genomes);
//	if (success)
//	{
//		for (int k = 0; k != genomes.size(); k++)
//		{
//			//cout << genomes[k].name() << endl;
//		}
//	}
//	else
//		cout << "Did not load file successfully" << endl;
//
//	GenomeMatcher matcher(4);
//	Genome g("g1", "CAAGCTACGTGACGTCATTGAGATC");
//	Genome g2("g2", "CAAGCTACGTGACGTCATTGAGATC");
//	Genome g3("g3", "CAAGCTTCGTGTTTTATGGGGATC");
//	matcher.addGenome(g);
//	matcher.addGenome(g2);
//	matcher.addGenome(g3);
//	vector<DNAMatch> matches;
//	//matcher.findGenomesWithThisDNA("GAAG", 3, true, matches);
//	for (int i = 0; i < matches.size(); i++)
//		cout << matches[i].genomeName << " " << matches[i].position << " " << matches[i].length << endl;
//	vector<GenomeMatch> results;
//	matcher.findRelatedGenomes(g2, 4, true, 0, results);
//	for (int i = 0; i < results.size(); i++)
//		cout << results[i].genomeName << " " << results[i].percentMatch << endl;
//}

class GenomeImpl
{
public:
	GenomeImpl(const string& nm, const string& sequence);
	static bool load(istream& genomeSource, vector<Genome>& genomes);
	int length() const;
	string name() const;
	bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string m_sequence; 
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm;
	m_sequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
	string name;
	string s;
	string sequence;
	bool gotName = false;
	bool inGenome = false;
	genomes = {};

	while (getline(genomeSource, s))
	{
		if (s.size() == 0)
			return false;
		if (s[0] == '>')
		{
			if (s.size() == 1)
				return false;
			if (inGenome)
			{
				genomes.push_back(Genome(name, sequence));
				sequence = "";
			}
			name = s.substr(1, s.size());
			gotName = true;
			inGenome = true;
		}
		else if (!gotName)
			return false;
		else if (gotName)
		{
			for (int i = 0; i < s.length(); i++)
			{
				if (toupper(s[i]) != 'A' && toupper(s[i]) != 'C' && toupper(s[i]) != 'T' && toupper(s[i]) != 'G' && toupper(s[i]) != 'N')
					return false;
			}
			sequence += s;
		}
	}
	if (!sequence.empty())
	{
		genomes.push_back(Genome(name, sequence));
		return true;
	}
	else
		return false;
}

int GenomeImpl::length() const
{
	return m_sequence.length();
}

string GenomeImpl::name() const
{
	return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	fragment = "";
	if (position + length > m_sequence.length() || position < 0 || length < 0)
		return false;
	fragment = m_sequence.substr(position, length);
	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}