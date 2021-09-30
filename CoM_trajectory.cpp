#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
using namespace std;

int main()
{
  double xCoM;                         //重心位置
  double Tsup = 0.8;                   //一歩にかかる時間
  double steps_Tsup = 800;             //一歩のステップ数
  double total_t = 0.0;                //総時間
  vector<double> xi{0};                //n歩目の初期重心位置
  vector<double> px_rev{0};            //修正された着地点
  vector<double> vi{0};                //n歩目の初期重心速度
  double zc = 0.75;                    //LIPMの重心高さ
  double g = 9.81;                     //重力加速度
  double Tc = sqrt(zc/g);
  double c = cosh(Tsup/Tc);
  double s = sinh(Tsup/Tc);
  int dummy;                           //使わない値をここに捨てる
  char split;

  ifstream fin("foot_pattern.csv");    //着地点が書かれたファイルを読み込む
  ofstream ofs;
  ofs.open("CoM_trajectory.csv");      //重心軌道を格納する

  //重心軌道を計算する過程で同時に取得すればいいのだが、今回はバグ発見のしやすさ、わかりやすさを優先して別々に行う。
  //この下のvectorに格納する部分についてもっと賢いやり方を知ってたら適当に変えて
  int n=0;
  while(1){
    fin >> dummy >> split >> px_rev[n] >> split >> xi[n] >> split >> vi[n];     //重心軌道を算出するのに必要な値を取得
    if(fin.eof()) break;
    n++;
    px_rev.push_back(0);
    xi.push_back(0);
    vi.push_back(0);
  }

  //重心軌道を計算。
  //一歩ごとに、
  //  t=0からt=Tsupまでステップ時間0.001秒で計算
  //tを0にリセット.総時間をtotal_tに格納.
  for(int n=0;n<px_rev.size()-2;n++){
    for(int steps=0; steps<steps_Tsup;steps+=1){
    //for(double t=0.0;t<=Tsup;t+=0.001){
      total_t += 0.001;
      double t = steps*0.001;     //stepはint型なので/1000はダメ
      xCoM = (xi[n] - px_rev[n])*cosh(t/Tc) + Tc*vi[n]*sinh(t/Tc) + px_rev[n];
      ofs << total_t << "," << xCoM << endl;
    }
  }
  return 0;
}
