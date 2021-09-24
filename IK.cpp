#include<iostream>
#include<cmath>
#include<fstream>
using namespace std;
//LIPM,矢状面のみ

//太もも回転角
template <typename T>
T IK_theta1(T x_waist, T z_waist, T x_ankle, T z_ankle, T l_1, T l_2);

//ひざ下回転角
template <typename T>
T IK_theta2(T x_waist, T z_waist, T x_ankle, T z_ankle, T l_1, T theta_1);

int main(void){
  //以下の座標はワールド座標
  double xCoM[10000];                               //重心のx座標
  double zCoM = 0.75;                               //重心の高さ
  double r = 0.1;                                   //重心と腰リンクの差
  double zwaist = zCoM - r;                         //腰リンクの高さ
  double xwaist;                                    //腰リンクのｘ位置
  double zswing_l[10000];                           //左足首の高さ
  double xswing_l[10000];                           //左足首のｘ位置
  double zswing_r[10000];                           //右足首の高さ
  double xswing_r[10000];                           //右足首の位置
  double l1 = 0.3;                                 //太ももの長さ
  double l2 = 0.3;                                 //ひざ下長さ
  double theta1_l;                                  //左太もも回転角
  double theta2_l;                                  //左ひざ下回転角
  double theta3_l;                                  //左足首回転角
  double theta1_r;                                  //右太もも回転角
  double theta2_r;                                  //右ひざ下回転角
  double theta3_r;                                  //右足首回転角
  double dummy;

  char split;
  ifstream fin1("CoM_trajectory.csv");
  int n=0;
  while(1){
    fin1 >> dummy >> split >> xCoM[n];
    if(fin1.eof()) break;
    n++;
  }
  n = 0;
  ifstream fin2("leg_trajectory.csv");
  while(1){
    fin2 >> dummy >> split >> xswing_r[n] >> split >> zswing_r[n] >> split >> xswing_l[n] >> split >> zswing_l[n];
    if(fin2.eof()) break;
    n++;
  }

  ofstream ofs;
  ofs.open("angle_trajectory.csv");

  for(int i=0;i<n;i++){
    xwaist = xCoM[i];
    theta1_r = IK_theta1(xwaist, zwaist, xswing_r[i], zswing_r[i], l1, l2);
    theta2_r = IK_theta2(xwaist, zwaist, xswing_r[i], zswing_r[i], l1, theta1_r);
    theta3_r = -theta1_r - theta2_r;

    theta1_l = IK_theta1(xwaist, zwaist, xswing_l[i], zswing_l[i], l1, l2);
    theta2_l = IK_theta2(xwaist, zwaist, xswing_l[i], zswing_l[i], l1, theta1_l);
    theta3_l = -theta1_l - theta2_l;

    ofs << theta1_l << "," << theta2_l << "," << theta3_l << "," << theta1_r << "," << theta2_r << "," << theta3_r << endl;
  }

  return 0;
}

//太もも回転角
template <typename T>
T IK_theta1(T x_waist, T z_waist, T x_ankle, T z_ankle, T l_1, T l_2){
  //numはnumaerator(分子), denはdenominator(分母)の略
  double acos_num,acos_den,atan_num,atan_den;
  //式については、別資料を確認せよ
  acos_num = pow(z_waist - z_ankle, 2.0) + pow(x_waist - x_ankle, 2.0) + pow(l_1, 2.0) - pow(l_2, 2.0);
  acos_den = 2 * l_1 * sqrt(pow(z_waist - z_ankle, 2.0) + pow(x_waist - x_ankle, 2.0));
  atan_num = x_waist - x_ankle;
  atan_den = z_waist - z_ankle;

  return -1 * acos(acos_num/acos_den) + atan2(atan_num, atan_den);
}

//ひざ下回転角
template <typename T>
T IK_theta2(T x_waist, T z_waist, T x_ankle, T z_ankle, T l_1, T theta_1){
  double atan_num,atan_den;

  atan_num = x_waist - x_ankle - l_1 * sin(theta_1);
  atan_den = z_waist - z_ankle - l_1 * cos(theta_1);

  return atan2(atan_num,atan_den) - theta_1;
}
