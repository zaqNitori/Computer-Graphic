//#include <iostream>
//#include <fstream>
//#include <string>
//
//using namespace std;
//
//typedef struct
//{
//    float r, g, b;
//	float x, y, z;
//}Vertex;
//
//int main(void)
//{
//	ifstream ifs;
//	string str;
//	float f[3];
//	int idx;
//
//	ifs.open("../../final/octahedron.obj", ifstream::in);
//	while (ifs >> str)
//	{
//		if (str == "f" || str == "v")
//		{
//			idx = 0;
//			while (ifs >> f[idx++] && idx < 3);
//			cout << f[0] << " " << f[1] << " " << f[2] << endl;
//		}
//
//	}
//	ifs.close();
//	return 0;
//}