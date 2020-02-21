bool anyTrue(const double a[], int n)
{
	if (n <= 0)
		return false;
	if (n > 1)
	{
		if (somePredicate(a[n - 1]))
			return true;
		anyTrue(a, n - 1);
	}
	else
		return somePredicate(a[n - 1]);
}

int countTrue(const double a[], int n)
{
	if (n <= 0)
		return 0;
	int count = 0;
	if (somePredicate(a[n - 1]))
		count++;
	return count + countTrue(a, n - 1);
}

int firstTrue(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n > 1)
	{
		if (firstTrue(a, n - 1) != -1)
			return firstTrue(a, n - 1);
		else if (somePredicate(a[n - 1]))
			return (n - 1);
		else
			return -1;
	}
	else if (somePredicate(a[0]))
		return 0;
	else
		return -1;
}

int positionOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;
	int m = n - 1;
	if (n > 1)
	{
		if (m > positionOfMin(a, n - 1))
			m = positionOfMin(a, n - 1);
		if (a[m] > a[n - 1])
			return n - 1;
	}
	else if (a[m] < a[0])
		return m;
	else
		return 0;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return true;
	if (n1 <= 0 || n2 > n1)
		return false;
	if (n1 > 1 && n2 > 1)
	{
		if (a2[n2 - 1] != a1[n1 - 1])
			return includes(a1, n1 - 1, a2, n2);
		else
			return includes(a1, n1 - 1, a2, n2 - 1);
	}
	else if (n2 == 1 && n1 > 1)	
	{
		if (a2[0] == a1[n1 - 1])
			return true;
		else
			return includes(a1, n1 - 1, a2, 1);
	}
	else
	{
		if (a2[n2 - 1] != a1[n1 - 1])
			return false;
		else
			return true;
	}
}
