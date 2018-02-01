//�F�ϊ����C�u����
//2017�N6��30���쐬

#include "ColorSpaceChange2.h"

namespace ColorSpaceChange {
#pragma region Cal
	// �R���X�g���N�^�̎���
	Cal::Cal()
	{
	}

	Cal::Cal(vector<double> calibData)
		: m_rx(calibData[0])
		, m_ry(calibData[1])
		, m_gx(calibData[2])
		, m_gy(calibData[3])
		, m_bx(calibData[4])
		, m_by(calibData[5])
		, m_Rgamma(calibData[6])
		, m_Ggamma(calibData[7])
		, m_Bgamma(calibData[8])
		, m_RYmax(calibData[9])
		, m_GYmax(calibData[10])
		, m_BYmax(calibData[11])
		, m_wX(calibData[9] * calibData[0] / calibData[1] + calibData[10] * calibData[2] / calibData[3] + calibData[11] * calibData[4] / calibData[5])
		, m_wY(calibData[9] + calibData[10] + calibData[11])
		, m_wZ(calibData[9] * (1.0 - calibData[0] - calibData[1]) / calibData[1] + calibData[10] * (1.0 - calibData[2] - calibData[3]) / calibData[3] + calibData[11] * (1.0 - calibData[4] - calibData[5]) / calibData[5])
		, m_wpX(0.0)
		, m_wpY(0.0)
		, m_wpZ(0.0)
	{
	}

	// csv�t�@�C�����璼�ړǂݍ���
	Cal::Cal(string filename)
	{
		ifstream csvFile;
		string str;
		vector<double> calibData(12);

		csvFile.open(filename);
		if (csvFile.fail()) { // csv���J���Ȃ��ꍇ
			cout << "csv�t�@�C����ǂݍ��߂܂���." << endl;
			exit(-1);
		}

		for (int i = 0; i < 12; i++) {
			getline(csvFile, str);
			string token;
			istringstream stream(str);

			getline(stream, token, ',');
			getline(stream, token, ',');

			stringstream ss;
			ss << token;
			ss >> calibData[i];
		}

		csvFile.close();
		
		setCalibData(calibData);
	}

	// �L�����u���[�V�����f�[�^�Z�b�g�֐�
	void Cal::setCalibData(vector<double> calibData) {
		m_rx = calibData[0];
		m_ry = calibData[1];
		m_gx = calibData[2];
		m_gy = calibData[3];
		m_bx = calibData[4];
		m_by = calibData[5];
		m_Rgamma = calibData[6];
		m_Ggamma = calibData[7];
		m_Bgamma = calibData[8];
		m_RYmax = calibData[9];
		m_GYmax = calibData[10];
		m_BYmax = calibData[11];
		m_wX = calibData[9] * calibData[0] / calibData[1] + calibData[10] * calibData[2] / calibData[3] + calibData[11] * calibData[4] / calibData[5];
		m_wY = calibData[9] + calibData[10] + calibData[11];
		m_wZ = calibData[9] * (1.0 - calibData[0] - calibData[1]) / calibData[1] + calibData[10] * (1.0 - calibData[2] - calibData[3]) / calibData[3] + calibData[11] * (1.0 - calibData[4] - calibData[5]) / calibData[5];
		m_wpX = 0.0;
		m_wpY = 0.0;
		m_wpZ = 0.0;
	}

	// Ymax�x�N�g���Z�b�g�֐�
	Mat Cal::setYmax() {
		Mat Ymax = (Mat_<double>(3, 1) << m_RYmax, m_GYmax, m_BYmax);

		return Ymax;
	}

	// �K���}�W���x�N�g���Z�b�g�֐�
	Mat Cal::setGamma() {
		Mat gamma = (Mat_<double>(3, 1) << m_Rgamma, m_Ggamma, m_Bgamma);

		return gamma;
	}

	// �ϊ��s��Z�b�g�֐�
	Mat Cal::setMatrix() {
		Mat matrix = (Mat_<double>(3, 3) << m_rx / m_ry, m_gx / m_gy, m_bx / m_by, 1.0, 1.0, 1.0, (1.0 - m_rx - m_ry) / m_ry, (1.0 - m_gx - m_gy) / m_gy, (1.0 - m_bx - m_by) / m_by);

		return matrix;
	}

	// ���F�_�Z�b�g�֐�
	Mat Cal::setWhitePoint(Mat_<Vec3b> src, Cal calibData, int whiteflag, int funcflag) {
		Mat_<double> whitePoint(3, 1, CV_64F);

		if (funcflag && whiteflag) { // BGR��Lab�ϊ� ���� ���F�_���摜���ő�P�x
			Mat_<Vec3d> XYZ = BGR2XYZ(src, calibData);
			double max = 0;
			int max_x, max_y;

			// �ő�P�x�v�Z
			for (int y = 0; y < XYZ.rows; y++) {
				for (int x = 0; x < XYZ.cols; x++) {
					if (XYZ[y][x][1] > max) {
						max = XYZ[y][x][1];
						max_x = x;
						max_y = y;
					}
				}
			}

			for (int ch = 0; ch < 3; ch++) {
				whitePoint[ch][0] = XYZ[max_y][max_x][ch];
			}

			m_wpX = whitePoint[0][0];
			m_wpY = whitePoint[1][0];
			m_wpZ = whitePoint[2][0];
		}
		else if (whiteflag && m_wpX != 0.0) { // BGR��Lab�ϊ� ���� ���F�_���摜���ő�P�x
			whitePoint[0][0] = m_wpX;
			whitePoint[1][0] = m_wpY;
			whitePoint[2][0] = m_wpZ;
		}
		else { // (255, 255, 255)
			whitePoint[0][0] = m_wX;
			whitePoint[1][0] = m_wY;
			whitePoint[2][0] = m_wZ;
		}

		return whitePoint;
	}

#pragma endregion

	// BGR����O���[�X�P�[���ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2Gray(Mat_<Vec3b> src, Cal calibData) {
		Mat_<uchar> gray(src.size(), CV_8U); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				//3. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = Ymax[i][0] * pow(RGB[i][0], gamma[i][0]);
				}

				gray[y][x] = saturate_cast<uchar>(255.0 * (RGB[0][0] + RGB[1][0] + RGB[2][0]) / (Ymax[0][0] + Ymax[1][0] + Ymax[2][0]));
			}
		}

		return gray;
	}

	// BGR����O���[�X�P�[���ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)
	Mat BGR2Gray(Mat_<Vec3b> src) {
		Mat_<uchar> gray(src.size(), CV_8U); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 2. �␳
				for (int i = 0; i < 3; i++) {
					if (RGB[i][0] <= 0.040450) {
						RGB[i][0] = RGB[i][0] / 12.92;
					}
					else {
						RGB[i][0] = pow((RGB[i][0] + 0.055) / 1.055, 2.4);
					}
				}

				gray[y][x] = saturate_cast<uchar>(255.0 * (0.222485 * RGB[0][0] + 0.716905 * RGB[1][0] + 0.060610 * RGB[2][0]));
			}
		}

		return gray;
	}

	// BGR����Y�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2Y(Mat_<Vec3b> src, Cal calibData) {
		Mat_<double> Y(src.size(), CV_64F); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 2. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = Ymax[i][0] * pow(RGB[i][0], gamma[i][0]);
				}

				Y[y][x] = (RGB[0][0] + RGB[1][0] + RGB[2][0]) / (Ymax[0][0] + Ymax[1][0] + Ymax[2][0]);
			}
		}

		return Y;
	}

	// BGR����Y�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)
	Mat BGR2Y(Mat_<Vec3b> src) {
		Mat_<double> Y(src.size(), CV_64F); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 2. �␳
				for (int i = 0; i < 3; i++) {
					if (RGB[i][0] <= 0.040450) {
						RGB[i][0] = RGB[i][0] / 12.92;
					}
					else {
						RGB[i][0] = pow((RGB[i][0] + 0.055) / 1.055, 2.4);
					}
				}

				Y[y][x] = 0.222485 * RGB[0][0] + 0.716905 * RGB[1][0] + 0.060610 * RGB[2][0];
			}
		}

		return Y;
	}

	// BGR����XYZ�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2XYZ(Mat_<Vec3b> src, Cal calibData) {
		Mat_<Vec3d> dst(src.rows, src.cols, CV_64FC3); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();
		Mat M = calibData.setMatrix();

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 2. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = Ymax[i][0] * pow(RGB[i][0], gamma[i][0]);
				}

				// 3. XYZ�v�Z
				XYZ = M * RGB;

				dst[y][x] = Vec3d(XYZ[0][0], XYZ[1][0], XYZ[2][0]); // �l�̐ݒ�	
			}
		}

		return dst;
	}

	// BGR����XYZ�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)
	Mat BGR2XYZ(Mat_<Vec3b> src) {
		Mat_<Vec3d> dst(src.rows, src.cols, CV_64FC3); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> M = (Mat_<double>(3, 3) << 0.436041, 0.385113, 0.143046, 0.222485, 0.716905, 0.060610, 0.013920, 0.097067, 0.713913);

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 2. �␳
				for (int i = 0; i < 3; i++) {
					if (RGB[i][0] <= 0.040450) {
						RGB[i][0] = RGB[i][0] / 12.92;
					}
					else {
						RGB[i][0] = pow((RGB[i][0] + 0.055) / 1.055, 2.4);
					}
				}

				// 3. XYZ�v�Z
				XYZ = M * RGB;

				dst[y][x] = Vec3d(XYZ[0][0], XYZ[1][0], XYZ[2][0]); // �l�̐ݒ�	
			}
		}

		return dst;
	}

	// XYZ����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat XYZ2BGR(Mat_<Vec3d> src, Cal calibData) {
		Mat_<Vec3b> dst(src.rows, src.cols, CV_8UC3); //�ϊ���摜�f�[�^
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();
		Mat M = calibData.setMatrix();

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. XYZ����
				for (int i = 0; i < 3; i++) {
					XYZ[i][0] = src[y][x][i];
				}

				// 2. RGB�v�Z
				RGB = M.inv() * XYZ;

				// 3. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = 255.0 * pow(RGB[i][0] / Ymax[i][0], 1.0 / gamma[i][0]);

					if (RGB[i][0] > 255.0) {
						RGB[i][0] = 255;
					}

					if (RGB[i][0] < 0.0) {
						RGB[i][0] = 0;
					}
				}

				dst[y][x] = Vec3b(saturate_cast<uchar>(RGB[2][0]), saturate_cast<uchar>(RGB[1][0]), saturate_cast<uchar>(RGB[0][0])); // �l�̐ݒ�
			}
		}

		return dst;
	}

	// BGR����Lab�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)(1�Ȃ�Xn, Yn, Zn���摜�����狁�߂�)
	Mat BGR2Lab(Mat_<Vec3b> src, Cal calibData, int flag) {
		Mat_<Vec3d> Lab(src.rows, src.cols, CV_64FC3); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> fXYZ(3, 1, CV_64F); //fXYZ�x�N�g��
		double L, a, b; //�v�ZLab�l
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();
		Mat M = calibData.setMatrix();
		Mat_<double> whitePoint(3, 1, CV_64F);

		// 1. ���F�_�ݒ�
		whitePoint = calibData.setWhitePoint(src, calibData, flag, 1);

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {

				// 2. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 3. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = Ymax[i][0] * pow(RGB[i][0], gamma[i][0]);
				}

				// 4. XYZ�v�Z
				XYZ = M * RGB;

				// 5. ���F�_�␳
				for (int i = 0; i < 3; i++) {
					XYZ[i][0] = XYZ[i][0] / whitePoint[i][0];
				}

				// 6. fX,fY,fZ�v�Z
				for (int i = 0; i < 3; i++) {
					if (XYZ[i][0] > 0.008856) {
						fXYZ[i][0] = pow(XYZ[i][0], 1.0 / 3.0);
					}
					else {
						fXYZ[i][0] = (pow(29.0 / 3.0, 3.0) * XYZ[i][0] + 16.0) / 116.0;
					}
				}

				// 7. Lab�v�Z
				L = 116.0 * fXYZ[1][0] - 16.0;
				a = 500.0 * (fXYZ[0][0] - fXYZ[1][0]);
				b = 200.0 * (fXYZ[1][0] - fXYZ[2][0]);

				Lab[y][x] = Vec3d(L, a, b); // �l�̐ݒ�	
			}
		}

		return Lab;
	}

	// BGR����Lab�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)(1�Ȃ�Xn, Yn, Zn���摜�����狁�߂�)
	Mat BGR2Lab(Mat_<Vec3b> src, int flag) {
		Mat_<Vec3d> Lab(src.rows, src.cols, CV_64FC3); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> fXYZ(3, 1, CV_64F); //fXYZ�x�N�g��
		double L, a, b; //�v�ZLab�l
		Mat_<double> M = (Mat_<double>(3, 3) << 0.436041, 0.385113, 0.143046, 0.222485, 0.716905, 0.060610, 0.013920, 0.097067, 0.713913);
		Mat_<double> whitePoint(3, 1, CV_64F);

		//���F�_�ݒ�
		//whitePoint = setWhitePoint(src, flag, 1);
		if (flag) { // �摜���ő�P�x�𔒐F�_������
			whitePoint[0][0] = 0.9642;
			whitePoint[1][0] = 1.0;
			whitePoint[2][0] = 0.8249;
		}
		else {
			whitePoint[0][0] = 0.9642;
			whitePoint[1][0] = 1.0;
			whitePoint[2][0] = 0.8249;
		}

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {

				// 2. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 3. �␳
				for (int i = 0; i < 3; i++) {
					if (RGB[i][0] <= 0.040450) {
						RGB[i][0] = RGB[i][0] / 12.92;
					}
					else {
						RGB[i][0] = pow((RGB[i][0] + 0.055) / 1.055, 2.4);
					}
				}

				// 4. XYZ�v�Z
				XYZ = M * RGB;

				// 5. ���F�_�␳
				for (int i = 0; i < 3; i++) {
					XYZ[i][0] = XYZ[i][0] / whitePoint[i][0];
				}

				// 6. fX,fY,fZ�v�Z
				for (int i = 0; i < 3; i++) {
					if (XYZ[i][0] > 0.008856) {
						fXYZ[i][0] = pow(XYZ[i][0], 1.0 / 3.0);
					}
					else {
						fXYZ[i][0] = (pow(29.0 / 3.0, 3.0) * XYZ[i][0] + 16.0) / 116.0;
					}
				}

				// 7. Lab�v�Z
				L = 116.0 * fXYZ[1][0] - 16.0;
				a = 500.0 * (fXYZ[0][0] - fXYZ[1][0]);
				b = 200.0 * (fXYZ[1][0] - fXYZ[2][0]);

				Lab[y][x] = Vec3d(L, a, b); // �l�̐ݒ�	
			}
		}

		return Lab;
	}

	// Lab����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)(1�Ȃ�Xn, Yn, Zn��ϊ��O�̉摜���ő�P�x��p����)
	Mat Lab2BGR(Mat_<Vec3d> src, Cal calibData, int flag) {
		Mat_<Vec3b> dst(src.rows, src.cols, CV_8UC3); //�ϊ���摜�f�[�^
		Mat_<double> fXYZ(3, 1, CV_64F); //fXYZ�x�N�g��
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();
		Mat M = calibData.setMatrix();
		Mat_<double> whitePoint(3, 1, CV_64F);

		// 1. ���F�_�ݒ�
		whitePoint = calibData.setWhitePoint(src, calibData, flag, 0);

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. fX,fY,fZ�v�Z
				fXYZ[1][0] = ((src[y][x][0] + 16) / 116.0);
				fXYZ[0][0] = (fXYZ[1][0] + (src[y][x][1] / 500.0));
				fXYZ[2][0] = (fXYZ[1][0] - (src[y][x][2] / 200.0));

				// 2. XYZ�v�Z
				for (int i = 0; i < 3; i++) {
					if (fXYZ[i][0] > (6.0 / 29.0)) {
						XYZ[i][0] = pow(fXYZ[i][0], 3.0);
					}
					else {
						XYZ[i][0] = pow(3.0 / 29.0, 3.0) * (116.0 * fXYZ[i][0] - 16.0);
					}

					// 3. XYZ�␳
					if (XYZ[i][0] < 0) {
						XYZ[i][0] = 0;
					}
				}

				// 4.���F�_�␳
				for (int i = 0; i < 3; i++) {
					XYZ[i][0] = XYZ[i][0] * whitePoint[i][0];
				}

				// 5. RGB�v�Z
				RGB = M.inv() * XYZ;

				// 6. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = 255.0 * pow(RGB[i][0] / Ymax[i][0], 1.0 / gamma[i][0]);
				}

				// 7. RGB�␳
				for (int i = 0; i < 3; i++) {
					if (RGB[i][0] > 255.0) {
						RGB[i][0] = 255;
					}

					if (RGB[i][0] < 0.0) {
						RGB[i][0] = 0;
					}
				}

				dst[y][x] = Vec3b(saturate_cast<uchar>(RGB[2][0]), saturate_cast<uchar>(RGB[1][0]), saturate_cast<uchar>(RGB[0][0])); // �l�̐ݒ�
			}
		}

		return dst;
	}

	// Lab����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^��)(1�Ȃ�Xn, Yn, Zn��ϊ��O�̉摜���ő�P�x��p����)
	//Mat Lab2BGR(Mat_<Vec3d> src, int flag) {
	//	Mat_<Vec3b> dst(src.rows, src.cols, CV_8UC3); //�ϊ���摜�f�[�^
	//	Mat_<double> fXYZ(3, 1, CV_64F); //fXYZ�x�N�g��
	//	Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
	//	Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��

	//	for (int y = 0; y < src.rows; y++) {
	//		for (int x = 0; x < src.cols; x++) {
	//			// 1. fX,fY,fZ�v�Z
	//			fXYZ[1][0] = ((src[y][x][0] + 16) / 116.0);
	//			fXYZ[0][0] = (fXYZ[1][0] + (src[y][x][1] / 500.0));
	//			fXYZ[2][0] = (fXYZ[1][0] - (src[y][x][2] / 200.0));

	//			// 2. XYZ�v�Z
	//			for (int i = 0; i < 3; i++) {
	//				if (fXYZ[i][0] >(6.0 / 29.0)) {
	//					XYZ[i][0] = pow(fXYZ[i][0], 3.0);
	//				}
	//				else {
	//					XYZ[i][0] = pow(3.0 / 29.0, 3.0) * (116.0 * fXYZ[i][0] - 16.0);
	//				}

	//				// 3. XYZ�␳
	//				if (XYZ[i][0] < 0) {
	//					XYZ[i][0] = 0;
	//				}
	//			}

	//			// 4.���F�_�␳
	//			for (int i = 0; i < 3; i++) {
	//				XYZ[i][0] = XYZ[i][0] * m_whitePoint[i][0];
	//			}

	//			// 5. RGB�v�Z
	//			RGB = m_M.inv() * XYZ;

	//			if (!m_dataFlag) { // sRGB
	//							   // 6. �␳
	//				for (int i = 0; i < 3; i++) {
	//					if (RGB[i][0] <= 0.0031308) {
	//						RGB[i][0] = 255.0 * 12.92 * RGB[i][0];
	//					}
	//					else {
	//						RGB[i][0] = 255.0 * (1.055 * pow(RGB[i][0], 1.0 / 2.4) - 0.055);
	//					}
	//				}
	//			}
	//			else {  // �L�����u���[�V�����f�[�^
	//					// 6. �␳
	//				for (int i = 0; i < 3; i++) {
	//					RGB[i][0] = 255.0 * pow(RGB[i][0] / m_YMax[i][0], 1.0 / m_Gamma[i][0]);
	//				}

	//			}

	//			// 7. RGB�␳
	//			for (int i = 0; i < 3; i++) {
	//				if (RGB[i][0] > 255.0) {
	//					RGB[i][0] = 255;
	//				}

	//				if (RGB[i][0] < 0.0) {
	//					RGB[i][0] = 0;
	//				}
	//			}

	//			dst[y][x] = Vec3b(saturate_cast<uchar>(RGB[2][0]), saturate_cast<uchar>(RGB[1][0]), saturate_cast<uchar>(RGB[0][0])); // �l�̐ݒ�
	//		}
	//	}

	//	return dst;
	//}

	// BGR����Yxy�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat BGR2Yxy(Mat_<Vec3b> src, Cal calibData) {
		Mat_<Vec3d> Yxy(src.rows, src.cols, CV_64FC3); //�ϊ���摜�f�[�^
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();
		Mat M = calibData.setMatrix();
		double Y, xx, yy; //�v�ZYxy�l

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {

				// 1. RGB���K��
				RGB[0][0] = saturate_cast<double>(src[y][x][2]) / 255.0;
				RGB[1][0] = saturate_cast<double>(src[y][x][1]) / 255.0;
				RGB[2][0] = saturate_cast<double>(src[y][x][0]) / 255.0;

				// 2. �␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = Ymax[i][0] * pow(RGB[i][0], gamma[i][0]);
				}

				// 3. XYZ�v�Z
				XYZ = M * RGB;

				// 4. Yxy�v�Z
				Y = XYZ[1][0] / (Ymax[0][0] + Ymax[1][0] + Ymax[2][0]);
				xx = XYZ[0][0] / (XYZ[0][0] + XYZ[1][0] + XYZ[2][0]);
				yy = XYZ[1][0] / (XYZ[0][0] + XYZ[1][0] + XYZ[2][0]);

				Yxy[y][x] = Vec3d(Y, xx, yy); // �l�̐ݒ�
			}
		}

		return Yxy;
	}

	// Yxy����BGR�ւ̕ϊ��֐�(�L�����u���[�V�����f�[�^�L)
	Mat Yxy2BGR(Mat_<Vec3d> src, Cal calibData) {
		Mat_<Vec3b> dst(src.rows, src.cols, CV_8UC3); //�ϊ���摜�f�[�^
		Mat_<double> XYZ(3, 1, CV_64F); //XYZ�x�N�g��
		Mat_<double> RGB(3, 1, CV_64F); //RGB�x�N�g��
		Mat_<double> Ymax = calibData.setYmax();
		Mat_<double> gamma = calibData.setGamma();
		Mat M = calibData.setMatrix();

		for (int y = 0; y < src.rows; y++) {
			for (int x = 0; x < src.cols; x++) {
				// 1. XYZ�v�Z
				XYZ[0][0] = src[y][x][0] * src[y][x][1] / src[y][x][2];
				XYZ[1][0] = src[y][x][0];
				XYZ[2][0] = src[y][x][0] * (1.0 - src[y][x][1] - src[y][x][2]) / src[y][x][2];

				// 2. XYZ�␳
				for (int i = 0; i < 3; i++) {
					if (XYZ[i][0] < 0.0) XYZ[i][0] = 0.0;
					XYZ[i][0] = XYZ[i][0] * (Ymax[0][0] + Ymax[1][0] + Ymax[2][0]);
				}

				// 3. RGB�v�Z
				RGB = M.inv() * XYZ;

				// 4. RGB�␳
				for (int i = 0; i < 3; i++) {
					RGB[i][0] = 255.0 * pow(RGB[i][0] / Ymax[i][0], 1.0 / gamma[i][0]);

					if (RGB[i][0] > 255.0) {
						RGB[i][0] = 255;
					}

					if (RGB[i][0] < 0.0) {
						RGB[i][0] = 0;
					}
				}

				dst[y][x] = Vec3b(saturate_cast<uchar>(RGB[2][0]), saturate_cast<uchar>(RGB[1][0]), saturate_cast<uchar>(RGB[0][0])); // �l�̐ݒ�
			}
		}

		return dst;
	}

	// ���F�_�Z�b�g�֐�(sRGB�p)
	//Mat setWhitePoint(Mat_<Vec3b> src, Mat_<double> &whitePoint, int whiteflag, int funcflag) {
	//	Mat_<double> whitePoint(3, 1, CV_64F);

	//	if (funcflag && whiteflag) { // BGR��Lab�ϊ� ���� ���F�_���摜���ő�P�x
	//		Mat_<Vec3d> XYZ = BGR2XYZ(src);
	//		double max = 0;
	//		int max_x, max_y;

	//		// �ő�P�x�v�Z
	//		for (int y = 0; y < XYZ.rows; y++) {
	//			for (int x = 0; x < XYZ.cols; x++) {
	//				if (XYZ[y][x][1] > max) {
	//					max = XYZ[y][x][1];
	//					max_x = x;
	//					max_y = y;
	//				}
	//			}
	//		}

	//		for (int ch; ch < 3; ch++) {
	//			whitePoint[ch][0] = XYZ[max_y][max_x][ch];
	//		}
	//	}
	//	else if (whiteflag && m_wpX != 0.0) { // BGR��Lab�ϊ� ���� ���F�_���摜���ő�P�x
	//		whitePoint[0][0] = m_wpX;
	//		whitePoint[1][0] = m_wpY;
	//		whitePoint[2][0] = m_wpZ;
	//	}
	//	else { // (255, 255, 255)
	//		whitePoint[0][0] = m_wX;
	//		whitePoint[1][0] = m_wY;
	//		whitePoint[2][0] = m_wZ;
	//	}

	//	return whitePoint;
	//}


}