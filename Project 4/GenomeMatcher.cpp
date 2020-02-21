#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Trie.h"
#include <map>
using namespace std;

class GenomeMatcherImpl
{
public:
	GenomeMatcherImpl(int minSearchLength);
	void addGenome(const Genome& genome);
	int minimumSearchLength() const;
	bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
	bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int m_minSearchLength;
	map<string, Genome> genomes;
	Trie<DNAMatch> trie;
	struct DNA
	{
		string name;
		int position;
	};
};

inline
bool comparePercent(const GenomeMatch& first, const GenomeMatch& second)
{
	if (first.percentMatch > second.percentMatch)
		return true;
	else if (first.genomeName > second.genomeName)
		return true;
	return false;
}

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	m_minSearchLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	genomes.insert({genome.name(), genome});
	for (int i = 0; i + m_minSearchLength <= genome.length(); i++)
	{
		string key;
		genome.extract(i, m_minSearchLength, key);
		DNAMatch temp;
		temp.genomeName = genome.name();
		temp.length = genome.length();
		temp.position = i;
		trie.insert(key, temp);
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{

	if (fragment.length() < m_minSearchLength || minimumLength < m_minSearchLength)
		return false;
	matches = {};
	vector<DNAMatch> v = trie.find(fragment.substr(0, m_minSearchLength), exactMatchOnly);
	map<string, DNAMatch> matchLengths;
	for (int i = 0; i < v.size(); i++)
	{
		auto p = genomes.find({ v[i].genomeName });
		string seqFrag;
		int numMatches;
		if (exactMatchOnly)
		{
			if (fragment.length() <= v[i].length)
				p->second.extract(v[i].position + m_minSearchLength, fragment.length() - m_minSearchLength, seqFrag);
			else
				p->second.extract(v[i].position + m_minSearchLength, v[i].length - m_minSearchLength - v[i].position, seqFrag);
			numMatches = m_minSearchLength;
			for (int i = 0; i < fragment.length() - m_minSearchLength && i < seqFrag.length(); i++)
			{
				if (seqFrag[i] == fragment[i + m_minSearchLength])
					numMatches++;
				else
					break;
			}
		}
		else
		{
			if (fragment.length() <= v[i].length)
				p->second.extract(v[i].position, fragment.length(), seqFrag);
			else
				p->second.extract(v[i].position, v[i].length - v[i].position, seqFrag);
			numMatches = 0;
			bool mismatch = false;
			for (int i = 0; i < fragment.length() && i < seqFrag.length(); i++)
			{
				if (seqFrag[i] == fragment[i])
					numMatches++;
				else if (!mismatch)
				{
					numMatches++;
					mismatch = true;
				}
				else
					break;
			}
		}
		if (numMatches >= minimumLength)
		{
			v[i].length = numMatches;
			auto q = matchLengths.find({ v[i].genomeName });
			if (q == matchLengths.end())
				matchLengths.insert({ v[i].genomeName, v[i] });
			else if (q->second.length < numMatches)
			{
				matchLengths.erase(q);
				matchLengths.insert({ v[i].genomeName, v[i] });
			}
		}	
	}

	for (auto p = matchLengths.begin(); p != matchLengths.end(); p++)
	{
		matches.push_back(p->second);
	}

	if (matches.size() > 0)
		return true;
	else
		return false;

}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < m_minSearchLength)
		return false;
	results = {};
	int numSequences = query.length() / fragmentMatchLength;
	vector<DNAMatch> matches;
	map<string, int> countOfMatches;
	for (int i = 0; i < numSequences; i++)
	{
		string extract;
		query.extract(i * fragmentMatchLength, fragmentMatchLength, extract);
		findGenomesWithThisDNA(extract, fragmentMatchLength, exactMatchOnly, matches);
		for (int k = 0; k < matches.size(); k++)
		{
			auto p = countOfMatches.find(matches[k].genomeName);
			if (p == countOfMatches.end())
				countOfMatches.insert({matches[k].genomeName, 1});
			else
				p->second++;
		}
	}
	for (auto p = countOfMatches.begin(); p != countOfMatches.end(); p++)
	{
		if (p->first == query.name())
			continue;
		double percentage = ((double)p->second / numSequences) * 100;
		GenomeMatch g;
		g.genomeName = p->first;
		g.percentMatch = percentage;
		if (percentage >= matchPercentThreshold)
			results.push_back(g);
	}
	if (results.size() > 0)
	{
		std::sort(results.begin(), results.end(), comparePercent);
		return true;
	}
	else
		return false;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
	m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
	delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
	m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
	return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}