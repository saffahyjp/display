#include "../include/BitMatrix.h"

int BitMatrix::popcnt() const
{
	// TODO: can be improved
	int ans = 0;
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			ans += (int) get(i, j);
	return ans;
}

OStream &operator <<(OStream &ost, const BitMatrix &bmx)
{
	ost << "BitMatrix " << bmx.height << " * " << bmx.width << "\n";
	for(int i = 0; i < bmx.height; i++)
	{
		for(int j = 0; j < bmx.width; j++)
			ost << (int) bmx.get(i, j);
		ost << "\n";
	}
	return ost;
}

bool operator ==(const BitMatrix &lhs, const BitMatrix &rhs)
{
	int sz = lhs.height * lhs.width;
	if(sz != rhs.height * rhs.width)
		return false;
	return lhs.data == rhs.data;
}

bool operator <(const BitMatrix &lhs, const BitMatrix &rhs)
{
	int sz = lhs.height * lhs.width;
	if(sz != rhs.height * rhs.width)
		return sz < rhs.height * rhs.width;
	return lhs.data < rhs.data;
}

bool operator !=(const BitMatrix &lhs, const BitMatrix &rhs)
{
	return !(lhs == rhs);
}

BitMatrix &BitMatrix::operator |=(const BitMatrix &other)
{
	assert(height == other.height);
	assert(width == other.width);
	int tot = (height * width + 31) >> 5;
	unsigned *cur = &data[0];
	const unsigned *oth = &other.data[0];
	for(int i = 0; i < tot; i++)
		cur[i] |= oth[i];
	return *this;
}
