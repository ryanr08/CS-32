class Landmark
{
public:
	Landmark(string name);
	virtual ~Landmark();
	virtual string color() const;
	virtual string icon() const = 0;
	string name() const;
private:	
	string m_name; 
};

Landmark::Landmark(string name)
	: m_name(name)
{}

Landmark::~Landmark()
{}

string Landmark::name() const
{
	return m_name;
}

string Landmark::color() const
{
	return "yellow";
}

class Hotel : public Landmark
{
public:
	Hotel(string name);
	~Hotel();
	virtual string icon() const;
};

Hotel::Hotel(string name)
	: Landmark(name)
{}

Hotel::~Hotel()
{
	cout << "Destroying the hotel " << this->name() << "." << endl;
}

string Hotel::icon() const
{
	return "bed";
}

class Restaurant : public Landmark
{
public:
	Restaurant(string name, int capacity);
	~Restaurant();
	virtual string icon() const;
private:
	int m_capacity;
};

Restaurant::Restaurant(string name, int capacity)
	:Landmark(name)
{
	m_capacity = capacity;
}

Restaurant::~Restaurant()
{
	cout << "Destroying the restaurant " << this->name() << "." << endl;
}

string Restaurant::icon() const
{
	if (m_capacity < 40)
		return "small knife/fork";
	else
		return "large knife/fork";
}


class Hospital : public Landmark
{
public:
	Hospital(string name);
	~Hospital();
	virtual string color() const;
	virtual string icon() const;
};

Hospital::Hospital(string name)
	: Landmark(name)
{}

Hospital::~Hospital()
{
	cout << "Destroying the hospital " << this->name() << "." << endl;
}

string Hospital::color() const
{
	return "blue";
}

string Hospital::icon() const
{
	return "H";
}