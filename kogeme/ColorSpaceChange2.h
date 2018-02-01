#pragma once

#include "Linker.h"
#include <fstream>

namespace ColorSpaceChange {
	class Cal {
	public:
		// コンストラクタ宣言
		Cal();
		Cal(vector<double> calibData);
		Cal(string filename);

	private:
		//メンバ変数
		double m_rx; //赤点x座標
		double m_ry; //赤点y座標
		double m_gx; //緑点x座標
		double m_gy; //緑点y座標
		double m_bx; //青点x座標
		double m_by; //青点y座標
		double m_Rgamma; //赤ガンマ係数
		double m_Ggamma; //緑ガンマ係数
		double m_Bgamma; //青ガンマ係数
		double m_RYmax; //赤最大輝度
		double m_GYmax; //緑最大輝度
		double m_BYmax; //青最大輝度
		double m_wX; //白色点X
		double m_wY; //白色点Y
		double m_wZ; //白色点Z
		double m_wpX; //白色点X
		double m_wpY; //白色点Y
		double m_wpZ; //白色点Z

	public:
		//メンバ関数
		void setCalibData(vector<double> calibData); // キャリブレーションデータセット関数
		Mat setYmax(); // Ymaxベクトルセット関数
		Mat setGamma();	// ガンマ係数ベクトルセット関数
		Mat setMatrix(); // 変換行列セット関数
		Mat setWhitePoint(Mat_<Vec3b> src, Cal calibData, int whiteflag, int funcflag); // 白色点セット関数
		
	};

	Mat BGR2Gray(Mat_<Vec3b> src, Cal calibdata); // BGRからグレースケールへの変換関数(キャリブレーションデータ有)
	Mat BGR2Gray(Mat_<Vec3b> src); // BGRからグレースケールへの変換関数(キャリブレーションデータ無)
	Mat BGR2Y(Mat_<Vec3b> src, Cal calibData); // BGRからYへの変換関数(キャリブレーションデータ有)
	Mat BGR2Y(Mat_<Vec3b> src); // BGRからYへの変換関数(キャリブレーションデータ無)
	Mat BGR2XYZ(Mat_<Vec3b> src, Cal calibData); // BGRからXYZへの変換関数(キャリブレーションデータ有)
	Mat BGR2XYZ(Mat_<Vec3b> src); // BGRからXYZへの変換関数(キャリブレーションデータ無)
	Mat XYZ2BGR(Mat_<Vec3d> src, Cal calibData); // XYZからBGRへの変換関数(キャリブレーションデータ有)
	Mat BGR2Lab(Mat_<Vec3b> src, Cal calibData, int flag = 0); // BGRからLabへの変換関数(キャリブレーションデータ有)(1ならXn, Yn, Znを画像内から求める)
	Mat BGR2Lab(Mat_<Vec3b> src, int flag = 0); // BGRからLabへの変換関数(キャリブレーションデータ無)(1ならXn, Yn, Znを画像内から求める)
	Mat Lab2BGR(Mat_<Vec3d> src, Cal calibData, int flag = 0); // LabからBGRへの変換関数(キャリブレーションデータ有)(1ならXn, Yn, Znを変換前の画像内最大輝度を用いる)
	Mat BGR2Yxy(Mat_<Vec3b> src, Cal calibData); // BGRからYxyへの変換関数(キャリブレーションデータ有)
	Mat Yxy2BGR(Mat_<Vec3d> src, Cal calibData); // YxyからBGRへの変換関数(キャリブレーションデータ有)
	//Mat setWhitePoint(Mat_<Vec3b> src, int whiteflag, int funcflag); // 白色点セット関数(sRGB用)
}