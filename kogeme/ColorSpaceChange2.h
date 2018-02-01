#pragma once

#include "Linker.h"
#include <fstream>

namespace ColorSpaceChange {
	class Cal {
	public:
		// �R���X�g���N�^�錾
		Cal();
		Cal(vector<double> calibData);
		Cal(string filename);

	private:
		//�����o�ϐ�
		double m_rx; //�ԓ_x���W
		double m_ry; //�ԓ_y���W
		double m_gx; //�Γ_x���W
		double m_gy; //�Γ_y���W
		double m_bx; //�_x���W
		double m_by; //�_y���W
		double m_Rgamma; //�ԃK���}�W��
		double m_Ggamma; //�΃K���}�W��
		double m_Bgamma; //�K���}�W��
		double m_RYmax; //�ԍő�P�x
		double m_GYmax; //�΍ő�P�x
		double m_BYmax; //�ő�P�x
		double m_wX; //���F�_X
		double m_wY; //���F�_Y
		double m_wZ; //���F�_Z
		double m_wpX; //���F�_X
		double m_wpY; //���F�_Y
		double m_wpZ; //���F�_Z

	public:
		//�����o�֐�
		void setCalibData(vector<double> calibData); // �L�����u���[�V�����f�[�^�Z�b�g�֐�
		Mat setYmax(); // Ymax�x�N�g���Z�b�g�֐�
		Mat setGamma();	// �K���}�W���x�N�g���Z�b�g�֐�
		Mat setMatrix(); // �ϊ��s��Z�b�g�֐�
		Mat setWhitePoint(Mat_<Vec3b> src, Cal calibData, int whiteflag, int funcflag); // ���F�_�Z�b�g�֐�
		
	};

	Mat BGR2Gray(Mat_<Vec3b> src, Cal calibdata); // BGR����O���[�X�P�[���ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2Gray(Mat_<Vec3b> src); // BGR����O���[�X�P�[���ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)
	Mat BGR2Y(Mat_<Vec3b> src, Cal calibData); // BGR����Y�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2Y(Mat_<Vec3b> src); // BGR����Y�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)
	Mat BGR2XYZ(Mat_<Vec3b> src, Cal calibData); // BGR����XYZ�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2XYZ(Mat_<Vec3b> src); // BGR����XYZ�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)
	Mat XYZ2BGR(Mat_<Vec3d> src, Cal calibData); // XYZ����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2Lab(Mat_<Vec3b> src, Cal calibData, int flag = 0); // BGR����Lab�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)(1�Ȃ�Xn, Yn, Zn���摜�����狁�߂�)
	Mat BGR2Lab(Mat_<Vec3b> src, int flag = 0); // BGR����Lab�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)(1�Ȃ�Xn, Yn, Zn���摜�����狁�߂�)
	Mat Lab2BGR(Mat_<Vec3d> src, Cal calibData, int flag = 0); // Lab����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)(1�Ȃ�Xn, Yn, Zn��ϊ��O�̉摜���ő�P�x��p����)
	Mat BGR2Yxy(Mat_<Vec3b> src, Cal calibData); // BGR����Yxy�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat Yxy2BGR(Mat_<Vec3d> src, Cal calibData); // Yxy����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	//Mat setWhitePoint(Mat_<Vec3b> src, int whiteflag, int funcflag); // ���F�_�Z�b�g�֐�(sRGB�p)
}