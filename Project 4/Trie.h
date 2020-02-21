#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

template<typename ValueType>
class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const ValueType& value);
	std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;
private:
	struct Node
	{
	    Node()
		{
			for (int i = 0; i < 127; i++)
				children[i] = nullptr;
		}
		vector<ValueType> data;
		Node* children[127];
	};

	Node* root;
	void destroy(Node* p);
	void insertHelper(const std::string& key, const ValueType& value, Node* p);
	vector<ValueType> findHelper(const string& key, bool match, Node* p, vector<ValueType>& v) const;
};

template <typename ValueType>
Trie<ValueType>::Trie()
{
	root = new Node;
}

template <typename ValueType>
void Trie<ValueType>::destroy(Node* p)
{
	if (p == nullptr)
		return;
	for (int i = 0; i < 127; i++)
		destroy(p->children[i]);
	delete p;
}

template <typename ValueType>
Trie<ValueType>::~Trie()
{
	destroy(root);
}

template <typename ValueType>
void Trie<ValueType>::reset()
{
	destroy(root);
	root = new Node();
}

template<typename ValueType>
void Trie<ValueType>::insertHelper(const std::string& key, const ValueType& value, Node* p)
{
	if (key.size() == 0)
	{
		p->data.push_back(value);
		return;
	}
	if (p->children[key[0]] != nullptr)
	{
		insertHelper(key.substr(1, key.size()), value, p->children[key[0]]);
	}
	else
	{
		p->children[key[0]] = new Node();
		insertHelper(key.substr(1, key.size()), value, p->children[key[0]]);
	}
}

template <typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
	insertHelper(key, value, root);
}

template <typename ValueType>
vector<ValueType> Trie<ValueType>::findHelper(const string& key, bool match, Node* p, vector<ValueType>& v) const
{
	if (p->children[key[0]] == nullptr && match)
		return {};		
	if (match)		
	{
		if (key.size() == 1)
		{
			v.insert(v.end(), p->children[key[0]]->data.begin(), p->children[key[0]]->data.end());
			return v;
		}
		else
			return findHelper(key.substr(1, key.size()), match, p->children[key[0]], v);
	}
	else
	{
		for (int i = 0; i < 127; i++)
		{
			Node* temp;
			if (p->children[i] != nullptr)
			{
				temp = p->children[i];
				if (key.size() == 1)
					v.insert(v.end(), temp->data.begin(), temp->data.end());
				else if (i == (int)key[0])
					findHelper(key.substr(1, key.size()), match, p->children[key[0]], v);
				else if (key.size() > 1 && temp->children[key[1]] != nullptr)
					vector<ValueType> tempVec = findHelper(key.substr(1, key.size()), true, temp, v);
			}
		}
		return v;
	}
}

template <typename ValueType>
vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
	if (root->children[key[0]] == nullptr)
		return {};
	vector<ValueType> v;
	v = findHelper(key.substr(1, key.size()), exactMatchOnly, root->children[key[0]], v);
	return v;
}

#endif // TRIE_INCLUDED