#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

int main()
{
  double xCoM;
  double Tsup = 0.8;
  double total_t = 0.0;
  //double t = 0.000;
  int Nmax = 5;

  double xi[Nmax + 1];
  double px_rev[Nmax + 1];
  double vi[Nmax + 1];
  double zc = 0.75;                                              //LIPMの重心高さ
  double g = 9.81;                                               //重力加速度
  double Tc = sqrt(zc/g);
  double c = cosh(Tsup/Tc);
  double s = sinh(Tsup/Tc);
  int dummy;
  char split;

  ifstream fin("foot_pattern.csv");                             //着地点が書かれたファイルを読み込む
  ofstream ofs;
  ofs.open("CoM_trajectory.csv");                               //重心軌道を格納する

  //重心軌道を計算する過程で同時に取得すればいいのだが、今回はバグ発見のしやすさ、わかりやすさを優先して別々に行う。
  int n=0;
  while(1){
    fin >> dummy >> split >> px_rev[n] >> split >> xi[n] >> split >> vi[n];     //重心軌道を算出するのに必要な値を取得
    if(fin.eof()) break;
    n++;
  }

  //重心軌道を計算。
  //一歩ごとに、
  //  t=0からt=Tsupまでステップ時間0.001秒で計算
  //tを0にリセット.総時間をtotal_tに格納.
  for(int n=0;n<=Nmax;n++){
    for(double t=0.0; t<=Tsup;t+=0.001){
      total_t += 0.001;
      xCoM = (xi[n] - px_rev[n])*cosh(t/Tc) + Tc*vi[n]*sinh(t/Tc) + px_rev[n];
      ofs << total_t << "," << xCoM << endl;//もしかしたら、total_tよりtの方が都合がいいのでは?
      //ofs << t << "," << xCoM << "," << endl;
    }
  }
  return 0;
}
