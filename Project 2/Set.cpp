#include "Set.h"
#include <iostream>
using namespace std;


Set::Set()
{
	head = nullptr;                       //initializes a Set with no Nodes and a size of 0
	tail = nullptr;
	m_size = 0;
}

Set::~Set()
{
	for (Node* p = tail; p != nullptr; p = p->prev)                  // runs through the Nodes in reverse order and deletes the Node in front of the one p points to, then deletes head at end
		delete p->next;
	delete head;
	head = nullptr;                                    
	tail = nullptr;
}

Set::Set(const Set& other)
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
	for (Node* p = other.head; p != nullptr; p = p->next)                          // runs through right hand side Set and inserts each of the values in each Node into the left hand side Set
	{
		this->insert(p->data);
	}
}

Set& Set::operator=(const Set& rhs)
{
	if (this != &rhs)                                  // checks if the right Set and left Set are the same Set
	{		
		ItemType x;
		for (int i = m_size; i > 0; i--)                   // runs through the left hand side Set and erases each element
		{
			this->get((i - 1), x);
			this->erase(x);
		}
		if (rhs.head == nullptr)                                        // if the rhs is an empty Set, then this Set is set to be empty
		{
			head = nullptr;
			tail = nullptr;
			m_size = 0;
			return *this;
		}
		for (Node* p = rhs.head; p != nullptr; p = p->next)                     // goes thrugh the right hand side Set and inserts each of the values in each Node into the left hand side
		{
			this->insert(p->data);
		}
	}
	return *this;

}

bool Set::empty() const
{
	if (m_size == 0)                      // returns true if size is empty, else returns false
		return true;
	else
		return false;
}

int Set::size() const
{
	return m_size;                       // returns size of set
}

bool Set::insert(const ItemType& value)
{
	if (this->contains(value))               // checks if value is already in Set
		return false;
	if (head == nullptr)                    
	{
		head = new Node;                    // if head is nullptr, then Set is empty, so a new Node is created and head is set to point to it  
		head->data = value;
		head->next = nullptr;
		head->prev = nullptr;
		tail = head;
		m_size++;
		return true;
	}
	else
	{
		Node* temp = tail;               // if head is not nullptr, then a new node is added to end of the list and tail is set to point to it, as well as the previous nodes next pointer
		tail = new Node;
		tail->data = value;
		tail->prev = temp;
		tail->next = nullptr;
		temp->next = tail;
		m_size++;
		return true;
	}

}

bool Set::erase(const ItemType& value)
{
	for (Node* p = head; p != nullptr; p = p->next)            // runs through the list             
	{
		if (p->data == value)                                 // checks if any of the items in the list match the value
		{
			Node* temp1 = p->next;                           // creates 2 temp pointers that save the Nodes before and after the one needed to be erased
			Node* temp2 = p->prev;
			if (p == head && p == tail)                    // case where there is only one item in the list
			{
				delete p;
				head = nullptr;                               // deletes the Node and sets the head and tail to nullptr
				tail = nullptr;
				m_size--;
				return true;
			}
			else if (p == head)                          // case where value is at beginning of list
			{
				delete p;
				temp1->prev = temp2;               // sets prev to nullptr, and sets head to point to element after one being deleted
				head = temp1;
				m_size--;
				return true;
			}
			else if (p == tail)                   // case where value is at end of the list
			{
				delete p;
				temp2->next = temp1;               // sets next to nullptr and tail to point to element before one being deleted
				tail = temp2;
				m_size--;
				return true;
			}
			else
			{
				delete p;
				temp1->prev = temp2;                   // general case where the values before and after one being deleted are connected
				temp2->next = temp1;	
				m_size--;
				return true;
			}
		}
	}
	return false;
}

bool Set::contains(const ItemType& value) const
{
	Node* p;
	for (p = head; p != nullptr && p->data != value; p = p->next)              // runs through list until p is either nullptr or points to the Node that contains value
		;
	if (p != nullptr)                                                // if p points to Node with value, then the Set contains the value, otherwise return false
		return true;
	else
		return false;
}

bool Set::get(int i, ItemType& value) const
{
	for (Node* p = head; p != nullptr; p = p->next)                     // runs through the list
	{
		int count = 0;
		for (Node* j = head; j != nullptr; j = j->next)           // run through list again
		{
			if (p->data > j->data)                              // Each value in the Set is compared to the other values and if it is greater, then count is incremented 
				count++;
		}
		if (count == i)                                       // if count is equal to i, then the item p points to is the one that should set to value
		{
			value = p->data;
			return true;
		}
	}
	return false;
}

void Set::swap(Set& other)       
{
	int s = m_size;
	m_size = other.m_size;
	other.m_size = s;                                   // swaps the m_sizes
	Node* temp1 = head;                          
	Node* temp2 = tail;
	head = other.head;                                // swaps the head and tail pointers of the two sets
	tail = other.tail;
	other.head = temp1;
	other.tail = temp2;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	ItemType x;                                        
	if (&result == &s2)                                        // special case where Set s2 and result are the same set
	{
		for (int i = 0; i < s1.size(); i++)                        // in this case, the contents of Set s1 which are not in result/s2 are added to result
		{
			s1.get(i, x);
			result.insert(x);
		}
		return;
	}
	result = s1;                                           // assigns result to equal to s1
	for (int i = 0; i < s2.size(); i++)                  // runs through s2 and adds every element of s2 that is not already in s1 into result
	{
		s2.get(i, x);
		result.insert(x);
	}
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	ItemType x;
	if (&result == &s2)                                   // special case where Set s2 and result are the same set
	{
		Set temp = result;                                 // result/s2 is saved in temp
		for (int i = 0; i < s1.size(); i++)               // runs through s1 and adds all elements of it to result
		{
			s1.get(i, x);
			result.insert(x);
		}
		for (int i = 0; i < temp.size(); i++)                   // runs through s2(temp) and removes all the elements in result that are contained in s2 
		{
			temp.get(i, x);
			result.erase(x);
		}
		return;
	}
	result = s1;                                             // for general case, result is set to s1
	for (int i = 0; i < s2.size(); i++)                      // runs through s2 and removes all the elements in result that are contained in s2
	{
		s2.get(i, x);                               
		result.erase(x);
	}
}

void Set::dump() const
{
	for (Node* p = head; p != nullptr; p = p->next)                      // used to output the values in a set
		cerr << p->data << endl;
}