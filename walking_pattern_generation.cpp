#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
using namespace std;

//本プログラムは「ヒューマノイドロボット第2版」p152~156のアルゴリズムに従う。
//本プログラムは両足支持期ナシの矢状面に拘束したLIPMの歩行パターン生成
int main()
{
  //定数
  double pi = 3.141592;
  double Tsup = 0.8;                                             //一歩の時間
  double px0 = 0.0;                                              //初期脚位置
  double zc = 0.75;                                               //LIPMの重心高さ
  double g = 9.81;                                               //重力加速度
  double Tc = sqrt(zc/g);
  double c = cosh(Tsup/Tc);
  double s = sinh(Tsup/Tc);
  int Nmax = 5;                                                  //足を踏み出す回数。何歩まで歩くか。
  vector<double> Sx{0.0, 0.0, 0.15, 0.15, 0.15, 0.0, 0.0};       //n歩目の歩幅を新たにvectorで定義

  //変数
  //新たにvectorで定義.配列より扱いやすいため
  //殆どの変数がわざわざ配列,vectorにする必要が無いのだが、教科書を参照しやすいようにしている

  vector<double> px{px0};
  double x_bar;                                                 //歩行素片の終端位置
  double v_bar;                                                 //歩行素片の終端速度
  vector<double> xd{0};
  vector<double> vd{0};
  vector<double> xi{0};
  vector<double> vi{0};
  vector<double> xf{0};
  vector<double> vf{0};
  double a = 10.0;                                              //教科書p155の式4.47の重み係数
  double b = 1.0;                                               //同上。これらをいじると歩行パターンが変わる。
  double d = a * pow(c-1,2) + b * pow(s/Tc,2);
  vector<double> px_rev{0};

  ofstream ofs;
  ofs.open("foot_pattern.csv");                                 //歩行パターンをcsvで出力

  //2021年9月30日n歩目の定義を修正.px[n]が支持位置のときをn歩目とする。0から４まで動く

  ofs << 0 << "," << px_rev[0] << "," << xi[0] << "," << vi[0] << endl;

  for(int n = 1;n <= Nmax; n++){
    px.push_back(px[n-1] + Sx[n]);                                      //教科書p153の式4.39
    xi.push_back(xf[n-1]);
    vi.push_back(vf[n-1]);

    x_bar = Sx[n+1]/2;                                                  //教科書p153の式4.40
    v_bar = (c + 1)/(Tc * s)*x_bar;                                     //教科書p153の式4.41

    xd.push_back(px[n] + x_bar);
    vd.push_back(v_bar);

    double px_rev_1 = a*(c-1)/d * (xd[n] - c*xi[n] - Tc*s*vi[n]);       //教科書p155の式4.48の上の行
    double px_rev_2 = b*s/(Tc*d) * (vd[n] - s/Tc*xi[n] - c*vi[n]);      //教科書p155の式4.48の中の行

    px_rev[n]= -1*px_rev_1 - px_rev_2;                                  //教科書p155の式4.48全体


    xf.push_back(c*xi[n] + Tc*s*vi[n] + (1-c)*px_rev[n]);               //教科書p155の式4.45
    vf.push_back(s/Tc*xi[n] + c*vi[n] - s/Tc*px_rev[n]);                //教科書p155の式4.45

    ofs << n << "," << px_rev[n] << "," << xi[n] << "," << vi[n] << endl;//元の着地点と修正された着地点をファイルに書き出す。
    //xi,viの値がn+1歩目のモノになってしまうバグ発生。原因は不明
  }
}
