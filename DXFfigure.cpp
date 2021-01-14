#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <graphics.h>
#include <conio.h>
using namespace std;

void main(void)
{
    //获取文件地址
    string path = "c: / users / hx893 / desktop / Sketch1.dxf";
    char DXF_line_data[40];//假设是16位精度，载加上几位整数位，估计40位是没问题了

    cout << "请输入文件（.dxf）的路径，理论上支持绝对路径和相对路径：";
    cin >> path;// c:/users/hx893/desktop/Sketch1.dxf

    //建立一个列表，识别不同几何实体
    string AcDbGeo[3] = {//Geo
        "LINE", "CIRCLE", "ARC"
    };
    string AcDbMod[2] = {//Mod
        "AcDbLine", "AcDbCircle"
    };

    //读文件，解析并寻找几何信息
    ifstream DXF_file(path, ios::in);
    string DXFxxxx;
    int inGeo(string a, string b[3]);
    int inMod(string a, string b[3]);
    string Geo[10];
    string Mod[10];

    int k = 0;
    int lines = 0;//文本行数

    double Start_location[5][3];
    double End_location[5][3];
    int Get_location(string path, int lines, double Start_location[5][3], double End_location[5][3], int k);

    while (DXF_file.getline(DXF_line_data, sizeof(DXF_line_data)))
    {
        stringstream DXF_word(DXF_line_data);
        DXF_word >> DXFxxxx;
        
        if (inGeo(DXFxxxx, AcDbGeo)) {
            Geo[k] = DXFxxxx;
        }
        if (inMod(DXFxxxx, AcDbMod)) {
            Mod[k] = DXFxxxx;
            Get_location(path, lines, Start_location, End_location, k);
            k += 1;
        }

        lines += 1;
    }
    cout << lines << endl;

    cout << "此DXF文件中包含的几何实体元素如下：" << endl;
    cout << k << endl;
    for (int i = 0; i < k; i++) {
        cout <<i+1<<" : "<< Geo[i] << " , " << Mod[i]<< endl;
    }

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < 3; j ++ ) {
            cout << End_location[i][j] << endl;
        }
    }

    //利用easyX进行简单绘图，缺点是存在坐标系翻转的问题，尚未加上翻转算法
    initgraph(1800, 800);
    for (int i = 0; i < k; i++) {
        line(int(Start_location[i][0]) * 50,
            int(Start_location[i][1]) * 50,
            int(End_location[i][0]) * 50,
            int(End_location[i][1]) * 50);
    }
    _getch();
    closegraph();

    system("pause");
}

int inGeo(string a, string b[3]) {
    for (int i = 0; i < 3; i++) {
        if (a == b[i]) {
            return 1;
        }
    }
    return 0;
}

int inMod(string a, string b[2]) {
    for (int i = 0; i < 2; i++) {
        if (a == b[i]) {
            return 1;
        }
    }
    return 0;
}



int Get_location(string path, int lines, double Start_location[5][3], double End_location[5][3], int k) {

    cout << "这一行输出说明正在进行子迭代以寻找坐标信息" << endl;
    cout << "目标对象的起始行是：" << lines << endl;

    int location_line = 0;
    ifstream location_file(path, ios::in);
    char line_data[40];
    string location;
    while (location_file.getline(line_data, sizeof(line_data)))
    {
        stringstream location_word(line_data);
        location_word >> location;
        location_line += 1;
        if (location_line == (lines + 3)) {
            Start_location[k][0] = stod(location)/32.0;
        }
        else if (location_line == (lines + 5)) {
            Start_location[k][1] = stod(location) / 32.0;
        }
        else if (location_line == (lines + 7)) {
            Start_location[k][2] = stod(location) / 32.0;
        }
        else if (location_line == (lines + 9)) {
            End_location[k][0] = stod(location) / 32.0;
        }
        else if (location_line == (lines + 11)) {
            End_location[k][1] = stod(location) / 32.0;
        }
        else if (location_line == (lines + 13)) {
            End_location[k][2] = stod(location) / 32.0;
            break;
        }
        else {
            continue;
        }

    }

    cout << "此DXF文件中包含的几何实体元素的坐标如下：" << endl;
    cout << "x" << " : " << Start_location[k][0] << " , " << End_location[k][0] << endl;
    cout << "y" << " : " << Start_location[k][1] << " , " << End_location[k][1] << endl;
    cout << "z" << " : " << Start_location[k][2] << " , " << End_location[k][2] << endl;

    return 0;
}
