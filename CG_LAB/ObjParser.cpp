#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main(void)
{
	ifstream ifs("../../final/octahedron.obj", ifstream::in);
	string str;
	float f[3];
	int idx;

	while (ifs >> str)
	{
		if (str == "f" || str == "v")
		{
			idx = 0;
			while (ifs >> f[idx++] && idx < 3);
			cout << f[0] << " " << f[1] << " " << f[2] << endl;
		}

	}
	ifs.close();
	return 0;
}