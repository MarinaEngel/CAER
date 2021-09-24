#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<math.h>
#include<sstream>

#define M_PI 3.14159265358979323846

using std :: cout;
using std :: cin;
using std :: string;
using std :: ifstream;
using std :: ofstream;
using std :: endl;

class Bilinear
{
public:
    double do_bilinear(int num_ch, double aot, double zen_sun, double zen_sat, double dh, double delta, double ext, double refl)
    {
        num_channel = num_ch;
        pos[0] = aot;
        pos[1] = 1-cos(M_PI*zen_sun/180);
        pos[2] = 1-cos(M_PI*zen_sat/180);
        pos[3] = dh;
        pos[4] = delta;
        pos[5] = ext;
        pos[6] = refl;

        get_data_cloud(num_ch);
        initialize_nodes(num_ch);
        get_neighbourhood();

        return do_math();

    }

private:
    double pos[7];
    double data[4][4][4][5][5][5][5];
    double nodes[7][5] = {{0, 0.13, 0.43, 1.27, 2}, {1-cos(M_PI*0.0001/180), 1-cos(M_PI*30/180), 1-cos(M_PI*45/180), 1-cos(M_PI*60/180), 1},
    {1-cos(M_PI*0.0001/180), 1-cos(M_PI*30/180), 1-cos(M_PI*45/180), 1-cos(M_PI*60/180), 1}, {0, 0.5, 1.5, 4, 6}, {0, 0.05, 0.15, 0.3, 0.5},
    {0, 10, 20, 40, 60}, {0, 0.1, 0.3, 0.5, 1}};
    int neighbourhood[7];

    int num_channel;

    void input(){
        string temp_string;

        ifstream is("input.txt");
        is >> num_channel;
        getline(is, temp_string);

        for(int i = 0; i<7; i++){
            is >> pos[i];
            if (i == 1 or i == 2) pos[i] = 1-cos(M_PI*pos[i]/180);
            getline(is, temp_string);
        }

        is.close();
    }


    void get_data_cloud (int n){
        double temp_double;
        string filename = "MODIS"+std :: to_string(n)+".txt";
        ifstream is(filename);

        for (int i0=0; i0<4; i0++) for (int i1=0; i1<4; i1++) for (int i2=0; i2<4; i2++) for (int i3=0; i3<5; i3++)
        for (int i4=0; i4<5; i4++) for (int i5=0; i5<5; i5++) for (int i6=0; i6<5; i6++){
            if (i3 == 0 or i4 == 0 or i5 == 0) data[i0][i1][i2][i3][i4][i5][i6] = 0;
            else is >> temp_double >> temp_double >> temp_double >> temp_double >> temp_double
            >> temp_double >> temp_double >> temp_double >> data[i0][i1][i2][i3][i4][i5][i6];
        }

        is.close();
    }

    void initialize_nodes (int n){
        switch (n){
            case 1: {
                nodes[0][1] = 0.13;
                nodes[0][2] = 0.43;
                nodes[0][3] = 1.27;
                nodes[0][4] = 2;
                break;
            }
            case 2: {
                nodes[0][1] = 0.11;
                nodes[0][2] = 0.39;
                nodes[0][3] = 1.14;
                nodes[0][4] = 2;
                break;
            }
            case 3: {
                nodes[0][1] = 0.09;
                nodes[0][2] = 0.3;
                nodes[0][3] = 0.89;
                nodes[0][4] = 2;
                break;
            }
            case 4: {
                nodes[0][1] = 0.074;
                nodes[0][2] = 0.25;
                nodes[0][3] = 0.73;
                nodes[0][4] = 2;
                break;
            }
            case 5: {
                nodes[0][1] = 0.053;
                nodes[0][2] = 0.18;
                nodes[0][3] = 0.54;
                nodes[0][4] = 2;
                break;
            }
            default: {
                nodes[0][1] = 0.13;
                nodes[0][2] = 0.43;
                nodes[0][3] = 1.27;
                nodes[0][4] = 2;
                break;
            }
        }
    }


    void get_neighbourhood(){
        for (int i = 0; i < 3; i++){
            if ((pos[i] < nodes[i][0]) or (pos[i] > nodes[i][3])){
                    cout << "out of range " << i << endl;
                    neighbourhood[i] = 3 - 1;
            }
            else for (int j = 1; j < 4; j++) if (pos[i] <= nodes[i][j]) { neighbourhood[i] = j-1; break; }
        }

        for (int i = 3; i < 7; i++){
            if ((pos[i] < nodes[i][0]) or (pos[i] > nodes[i][4])){
                    cout << "out of range " << i << endl;
                    neighbourhood[i] = 4 - 1;
            }
            else for (int j = 1; j < 5; j++) if (pos[i] <= nodes[i][j]) { neighbourhood[i] = j-1; break; }
        }
    }

    double do_math(){
        double matr[128][128];
        double rightpart[128];
        double initial[128];
        double* seidel_result = new double[128];
        double result;

        int i[7];
        int ii[7];
        int c[7];
        int r_counter = 0;
        int c_counter = 0;

        //gathering a matrix
        for (i[0]=0; i[0]<2; i[0]++) for (i[1]=0; i[1]<2; i[1]++) for (i[2]=0; i[2]<2; i[2]++) for (i[3]=0; i[3]<2; i[3]++) \
        for (i[4]=0; i[4]<2; i[4]++) for (i[5]=0; i[5]<2; i[5]++) for (i[6]=0; i[6]<2; i[6]++)
        {
            for (int j = 0; j<7; j++)
                c[j] = neighbourhood[j]+i[j];

            rightpart[r_counter] = data[c[0]][c[1]][c[2]][c[3]][c[4]][c[5]][c[6]];
            initial[r_counter] = 0;

            for (ii[0]=0; ii[0]<2; ii[0]++) for (ii[1]=0; ii[1]<2; ii[1]++) for (ii[2]=0; ii[2]<2; ii[2]++) for (ii[3]=0; ii[3]<2; ii[3]++) \
            for (ii[4]=0; ii[4]<2; ii[4]++) for (ii[5]=0; ii[5]<2; ii[5]++) for (ii[6]=0; ii[6]<2; ii[6]++)
            {
                matr[r_counter][c_counter]=1;
                for (int j = 0; j<7; j++) matr[r_counter][c_counter] *= pow(nodes[j][c[j]],ii[j]);
                c_counter++;
            }

            c_counter = 0;
            r_counter++;
        }

        seidel_result = gauss(matr, rightpart);

        //getting the interpolation
        result = 0;
        double prod = 1;
        c_counter = 0;
        for (ii[0]=0; ii[0]<2; ii[0]++) for (ii[1]=0; ii[1]<2; ii[1]++) for (ii[2]=0; ii[2]<2; ii[2]++) for (ii[3]=0; ii[3]<2; ii[3]++) \
        for (ii[4]=0; ii[4]<2; ii[4]++) for (ii[5]=0; ii[5]<2; ii[5]++) for (ii[6]=0; ii[6]<2; ii[6]++){
            prod=1;
            for (int j = 0; j<7; j++) prod *= pow(pos[j],ii[j]);
            result += prod * seidel_result[c_counter];
            c_counter++;
        }

        delete seidel_result;
        return result;
    }

    double* gauss(double matr[128][128], double rightpart[128])
    {
        int i, j, k;
        double U[128][128+1];
        double* x = new double[128];

        for (i = 0; i < 128; i++){
            U[i][128] = rightpart[i];
            for (j = 0; j < 128; j++) U[i][j] = matr[i][j];
        }

        for (k = 0; k < 128; k++){
            int p = k;
            double maxPivot = 0.0;
            for (i = k; i < 128; i++){
                if (fabs(U[i][k] > maxPivot)){
                    maxPivot = fabs(U[i][k]);
                    p = i;
                }
            }

            if (p != k){
                for (i = k; i < 128 + 1; i++) std::swap(U[p][i], U[k][i]);
            }

            for (i = k + 1; i < 128; i++){
                double m = U[i][k] / U[k][k];
                for (j = k; j < 128 + 1; j++)
                    U[i][j] -= m * U[k][j];
            }
        }

        for (int k = 128 - 1; k >= 0; k--){
            double sum = U[k][128];
            for (int j = k + 1; j < 128; j++) sum -= U[k][j] * x[j];
            x[k] = sum / U[k][k];
        }

        return x;
    }
};

double pow(double a, int b){
    double c=1;
    for (int i = 0; i < b; i++) c=c*a;
    return c;
}

main(){
    ifstream is("input.txt");
    ofstream os("res.txt");
    string str;
    Bilinear A;

    int num_ch = 0;
    double aot, zen_sun, zen_sat, dh, delta, ext, refl;

    double res;

    is >> num_ch;
    getline(is, str);
    is >> aot;
    getline(is, str);
    is >> zen_sun;
    getline(is, str);
    is >> zen_sat;
    getline(is, str);
    is >> dh;
    getline(is, str);
    is >> delta;
    getline(is, str);
    is >> ext;
    getline(is, str);
    is >> refl;;
    getline(is, str);

    res = A.do_bilinear(num_ch, aot, zen_sun, zen_sat, dh, delta, ext, refl);

    double wavelength;
    switch(num_ch)
    {
        case 1: { wavelength = 0.41; break; }
        case 2: { wavelength = 0.47; break; }
        case 3: { wavelength = 0.55; break; }
        case 4: { wavelength = 0.68; break; }
        case 5: { wavelength = 0.86; break; }
    }

    os << "Wavelength number " << num_ch << " (" << wavelength << " mkm)" << endl
    << "Aerosol optical depth: " << aot << endl << "Solar zenith angle: " << zen_sun << " deg" << endl
    << "Satellite zenith angle: " << zen_sat << " deg" << endl
    << "Cloud geometric thickness: " << dh << " km" << endl
    << "Cloud cover index: " << delta << endl << "Cloud extinction: " << ext << " km-1" << endl
    << "Ground reflectance: " << refl << endl;

    os << "RCritical = " << res << endl;
    is.close();
}

