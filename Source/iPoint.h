#pragma once

struct iPoint
{
	iPoint(int px, int py)
	{
		x = px; y = py; 
	}

	iPoint() {}

	~iPoint() {}

	int x = -1;
	int y = -1; 
};
