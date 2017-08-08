/*
* �ļ����ƣ�Matcher.h
* �����⣺opencv3.1.0�������汾
* ���Ի�����VS2015 x64
* ժ Ҫ��	������������������+������LK����+���7��SADƥ����������SAD����������������ض�Ӧ�Ҷ�ֵ��ľ���ֵ�ĺͣ�
*			�����ԣ�ָ���ǣ��������ڹ�������������㣬�Ǵ���ȡ�����ڶ���У�ѡ�������һ����Լ�������ĵ㣬�����֮������һ���ľ��룺maxPixShift+blockSize
*			������LK��ָ���ǣ�����������֮��ʹ�ý�����LK�㷨У����������ȡ��ֵ����ȷ��������
*			���7��SADƥ�䣺ÿһ������һ֡��Ӧ������ȡ������7���㣬Ȼ����7�����н���SADƥ�䣬ѡ����СSADֵ��Ϊƥ���
* �ؼ��㣺���ƾ��룬7��ƥ�䣬���³�ʼ�������³�ʼ���Ƿǳ���Ҫ�ģ��������ʱ������Ҫ���³�ʼ�������򽫼���������һ֡�еĵ㣩
* ע�⣺�������������Ĺ��������ǹ̶��ģ���ofXY��ά���ǲ���ģ��������ǵ����Ĺ������㺯�������漰�����ʲ����Լ���������ѡ��Ȳ���
* ʹ��ʵ������ʾ������main.cpp
* ע�⣺	1���������������Ĺ��������ǹ̶��ģ���ofXY��ά���ǲ���ģ��������ǵ����Ĺ������㺯�������漰�����ʲ����Լ���������ѡ��Ȳ���
*			2���������Ӧ��ѡ���������ӦΪ����XY�������򲻼Ӿ���ֵ������XY�����ϸ���ȥ2������2����С����ֵ�����ܵ���2����Ϊ���������趨
*			�ﵽ3�����������Ĺ��������趨���������λ��ʱ�����³�ʼ�����������������ȥ8���㣨�������ص�������Ȼ���������
*			�������϶�Ļ������Կ�����ȥ���ࣨ��Ϊ������û�д洢ÿ�����SADֵ�����Բ�֧��SAD����ѡ�������
*			3������������������һ��ΪnumOF=18���ң����ȡ18������ȥ8����󣬻���10�����õ���������������ٶȵ���Ϣ����Ȼ���������Ҫ��ô�����õ㣬
*			������Щ�������ͼƬ�����ȽϺã�����൫�����ң�ͼƬ�ߴ��ʵ��󣬿���ȡ��㣬����ʱҲ���Զ�Щ����ͼƬ��������ʱ��numOF�͵���Щ�ˣ���Ϊ
*			����ȡ������ô��ĵ㡣
* 2016.05.24
*/

#include "calcOpticalFlowFeaturesPyrLKSAD.h"

void calcOpticalFlowFeaturesPyrLKSAD(
	cv::Mat prev,
	cv::Mat curr,
	std::vector<cv::Point2f>& prevXY,
	std::vector<cv::Point2f>& currXY,
	float ofXY[][2],
	bool needInit,
	int numOF,
	int pyrLKSize,
	int sadBlockSize,
	int blockSize,
	int maxPixShift)
{
	int numPoints = 30 * numOF;
	if (needInit)
	{
		initGoodFeatures(
			prev,
			prevXY,
			numOF,
			numPoints,
			maxPixShift,
			blockSize);
	}

	calcOF(
		prev,
		curr,
		prevXY,
		currXY,
		ofXY,
		numOF,
		pyrLKSize,
		sadBlockSize,
		blockSize,
		maxPixShift);
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*----------------------------------------------------�����ָ���----------------------------------------------------*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void initGoodFeatures(
	cv::Mat prev,
	std::vector<cv::Point2f>& prevXY,
	int numOF,
	int numPoints,
	int maxPixShift,
	int blockSize)
{
	int minDis = maxPixShift + blockSize;
	cv::Mat prev_copy;
	prev.copyTo(prev_copy);

	std::vector<cv::Point2f> goodCorners;
	cv::goodFeaturesToTrack(
		prev,						//Ҫ��ȡgoodFeatures��ͼ������
		goodCorners,				//�����洢��ȡ���ĺýǵ�ʸ������
		numPoints,					//��ȡ�������뵽goodCorners�����ǵ������������numOF����Ϊ�����������ĸ���
		0.001,						//�����ȼ���Խ����ȡ���Ľǵ�����Խ�ã���ȡ���ǵ���Խ��
		0,							//��ȡ���ǵ�֮�����С���룬��Ϊ0��ʹ�ýǵ�֮�䲻�ܾ���Ӱ��
		cv::Mat(),					//������Ҫ�ҽǵ������mask��
		blockSize,					//blockSize���������Ӧ�þ��ǽ��з����ֵ���ƵĴ��ڣ�������Harris�еķ����ֵ���ƴ��ڣ���Խ����ȡ���Ľǵ�Խ�٣��ò����ܴ�̶���Ӱ����ȡ���Ľǵ�����
		true,						//�Ƿ�ʹ��Harris�ǵ���
		0.01);						//k������Harris�ǵ����еĲ��������opencv�ֲ���ߡ�ͼ��ֲ�������������������һ��50ҳ��k����alpha

	int detected = goodCorners.size();	//��ȡ���ĵ�ĸ�����һ����numPoints
	int rows = prev.rows;
	int cols = prev.cols;
	int counter = 0;

	for (int i = 0;i < detected;i++)
	{
		int xx = goodCorners[i].x;
		int yy = goodCorners[i].y;

		bool qualified = false;
		/******���if����е��б����ǳ���Ҫ���ж��Ƿ�ӽ���Ե������ӽ���Ե�����Ǻܺõĸ��ٵ㣬���Ը����䷶Χ��ʹ���ҳ��Ľǵ㳤ʱ���ʺϸ��٣��Լ��ٽ�Ϊ��ʱ�ĳ�ʼ������******/
		/*if (xx > (2 * minDis + 1) && xx < (cols - 2 * minDis + 1) && yy >(2 * minDis + 1) && yy < (rows - 2 * minDis + 1))*/
		if (xx >(2 * minDis + 1) && xx < (cols - 2 * minDis + 1) && yy >(2 * minDis + 1) && yy < (rows - 2 * minDis + 1))
		{
			qualified = true;
			for (int j = 0;j < i;j++)
			{
				//���if������жϸýǵ������ǵ���Ӧֵ��������нǵ��Ƿ����һ���ľ��룬���С�ڸ������������룬����������������Ч������Щ��Ӧֵ����ĵ�ĸ��ţ���1ֻ��Ϊ��ȷ��ÿ��Ҫ�Ŀ���Ӱ��
				if (abs(xx - goodCorners[j].x)<= minDis +1 && abs(yy - goodCorners[j].y) <= minDis +1)
				{
					qualified = false;
					break;
				}
			}
			if (qualified)
			{
				prevXY.push_back(goodCorners[i]);
				counter++;
				if (counter >= numOF)
				{
					break;					//����numOF���ʺϸ��ٵĺýǵ㣬����ѭ��
				}
			}
		}
	}

	//�����numPoints��û���ҵ����ʵĸ��ٵ㣬��ݹ���ñ�������numPoints�������ҳ�����ĵ���ѡ��
	if (prevXY.size() >= numOF)
	{
		//�ݹ麯����ҪҪ����ֹ����������������cornerSubPix()�������뵽else�У�����ֱ�ӷŵ�����
		//��ȷ��������
		cv::cornerSubPix(prev, prevXY, cv::Size(blockSize / 2, blockSize / 2), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 0.01, 30));

		std::vector<cv::Point2f>().swap(goodCorners);

		return;
	}
	else
	{
		prevXY.clear();					//���prevXY
		numPoints *= 2;
		
		//���numPoints>3000��û�ҵ��ǵ㣬�Ǿ��Ҳ�����
		if (numPoints > 3000)
		{
			std::cout << "��ⲻ���ʺϼ�����������и��ٵĽǵ㣡" << std::endl << std::endl;
			assert(0);
			return;
		}
		initGoodFeatures(
			prev,
			prevXY,
			numOF,
			numPoints,
			maxPixShift,
			blockSize);
	}
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*----------------------------------------------------�����ָ���----------------------------------------------------*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void calcOF(
	cv::Mat prev,
	cv::Mat curr, 
	std::vector<cv::Point2f>& prevXY,
	std::vector<cv::Point2f>& currXY,
	float ofXY[][2],
	int numOF, 
	int pyrLKSize,
	int sadBlockSize,
	int blockSize, 
	int maxPixShift)
{
	//ǰ������ȡ��ʱ�����Ƶľ�����maxPixShift+blockSize����Ϊ����������һ��������LK���̣���ˣ�Ҫ����pyrLKSize����ֹ����ͼ���Ե
	int minDis = maxPixShift + pyrLKSize + blockSize;		//���ڱ߽��飬��ֹ����ͼ���Ե
	int rows = curr.rows;
	int cols = curr.cols;
	int counterFailure = 0;						//����������쳣�ĵ�
	bool reInit = false;						//�Ƿ���Ҫ���³�ʼ����־λ

	//�����������������㺯���õ��Ĳ���
	std::vector<uchar> status;					//must be uchar, can't be char!!!!!
	std::vector<float> err;
	cv::Size winSize(7, 7);
	int maxLevel = 3;							//����������
	cv::TermCriteria termCrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	int flag = 0;
	double minEigThreshold = 1e-3;				//������LK������ֹ����2

	for (int i = 0;i < numOF;i++)
	{
		int xx = prevXY[i].x;
		int yy = prevXY[i].y;

		//��������Ƿ���ܸ������������������ͼ������֮�⣬�����³�ʼ��
		if ((xx - minDis) < 0 || (xx + minDis) >= cols || (yy - minDis) < 0 || (yy + minDis) >= rows)
		{
			//���³�ʼ��֮�󣬽���һ���µ�prevXY
			reInit = true;
			break;
		}

		//�����������ƫ�ƣ�ȷ����������+1����ΪRange()�Ҳ�������
		cv::Mat searchRegion = curr(cv::Range(prevXY[i].y - maxPixShift - blockSize, prevXY[i].y + maxPixShift + blockSize + 1),
			cv::Range(prevXY[i].x - maxPixShift - blockSize, prevXY[i].x + maxPixShift + blockSize + 1));

		std::vector<cv::Point2f> corner;
		cv::goodFeaturesToTrack(
			searchRegion,				//��������
			corner,						//�洢���ʸ������
			7,							//��7���㣬һ��������7�����ŵ㣬���ͼƬ����ʵ�ڱ仯�ľ��ң���������һ�������ĵ㣬����������ݻ��߻��۵�3�����³�ʼ���Ϳ�����
			0.001,						//�����ȼ���Խ����ȡ���Ľǵ�����Խ�ã���ȡ���ǵ���Խ��
			0,							//��ȡ���ǵ�֮�����Сŷʽ���ؾ��룬Խ����ȡ���Ľǵ�Խ��
			cv::Mat(),					//������Ҫ�ҽǵ������mask��
			blockSize,					//blockSize���������Ӧ�þ��ǽ��з����ֵ���ƵĴ��ڣ���ʵ�ߴ�
			true,						//�Ƿ�ʹ��Harris�ǵ���
			0.01);						//k������Harris�ǵ����еĲ��������opencv�ֲ���ߡ�ͼ��ֲ�������������������һ��50ҳ��k����alpha

		//��һ֡�н�����LK����������
		cv::Mat prev_patch = prev(cv::Range(prevXY[i].y - pyrLKSize, prevXY[i].y + pyrLKSize + 1),
			cv::Range(prevXY[i].x - pyrLKSize, prevXY[i].x + pyrLKSize + 1));

		//��һ֡������LK���������е�����꣨���Ե�Ϊ���ģ�������������������Ըõ�Ϊ���ĵ�һ�����������һ֡�е�������Ϊ������������ĵ��������꣩
		std::vector<cv::Point2f> preCorner;
		preCorner.push_back(cv::Point2f(pyrLKSize, pyrLKSize));

		int sad[7];							//7�����SADֵ

		int detectedNum = corner.size();	//��˵ָ����ȡ7���㣬���п��ܳ�����ȡ����7��������
		cv::Point2f matchedCorner;			//7����������ƥ��ɹ��ĵ�
		int minSAD = 10000;					//��ʼ��СSADֵ����΢ȡ��һ�㣩
		int counterOverflow = 0;
		for (int j = 0;j < detectedNum;j++)
		{
			//���curr_patch�ܹؼ�����ΪLK�������ڴ��˶�����Ȼʹ���˽������������ǰ����ʹ��goodFeaturesToTrack()���������λ�ã�
			//��ʹ��LK���������ʱ��curr_patch������goodFeaturesToTrack()�����ĵ�Ϊ���ĵ���Χ��һ��������������һ֡����һ֡�ڿ�ʼʱ�̣�
			//���λ�ö����������������ĵ㣬LK������������ȥ���ĵ���������LK�������̱仯�����꣬����goodFeaturesToTrack()���������꣬
			//�������յõ��������صĵ������
			corner[j].x += prevXY[i].x - maxPixShift - blockSize;	//����goodFeaturesToTrack()���������λ����ǰ֡����ͼƬ���������
			corner[j].y += prevXY[i].y - maxPixShift - blockSize;

			//��ȡLK��������
			cv::Mat curr_patch = curr(cv::Range(corner[j].y - pyrLKSize, corner[j].y + pyrLKSize+1),
				cv::Range(corner[j].x - pyrLKSize, corner[j].x + pyrLKSize+1));

			//������һ֡������LK���������е�����
			std::vector<cv::Point2f>nextCorner;	//pyrLK�����������

			cv::calcOpticalFlowPyrLK(
				prev_patch,
				curr_patch,
				preCorner,
				nextCorner,
				status,
				err,
				winSize,
				maxLevel,
				termCrit,
				flag,
				minEigThreshold);

			//��֪��ʲôԭ�򣬽�����LK�����һЩ��ֵ�ֵ������ͼƬ��Χ�����꣬�����趨һ�����ƣ��˳���Щ��
			//��������ֵ�ֵ���ǿ϶�����������⣬Ҫôͼ������������Ҫô��ǰ����������ȡ����ȷ��������prev_patch��curr_patch����ţ���༰��
			//���һ�����������LK�Ĳ��ȶ��ˣ���֮����Ҫ������Щ��
			if (nextCorner[0].x<0 || nextCorner[0].y<0 || nextCorner[0].x > 2*pyrLKSize || nextCorner[0].y > 2*pyrLKSize) 
			{ 
				counterOverflow++;
				//���LK���ȫ���������ֵ�������������򣩣���ֱ��ȡΪ��Ӧֵ���ĵ�λƥ��㣬������ƫ��������ڹ������û�����if��
				//��ô���������֡�������û�и�ֵ�ˣ������������Կ���Ĭ���ǣ�0,0�����������Ͳ����ˣ�
				if (counterOverflow == 7)
				{
					matchedCorner = corner[0];
				}
				continue; 
			}
			
			//������LK������仯������+����ǰ���ڵ�ǰ֡����ͼƬ�����꣬�������յĵ��ڵ�ǰ֡�е������꣨�����ؾ��ȣ�
			corner[j].x += nextCorner[0].x - preCorner[0].x;
			corner[j].y += nextCorner[0].y - preCorner[0].y;

			//�ȷֱ���ȡSAD����飬Ȼ�����SADֵ
			cv::Mat prevPatch = prev(cv::Range(prevXY[i].y - sadBlockSize, prevXY[i].y + sadBlockSize+1),
				cv::Range(prevXY[i].x - sadBlockSize, prevXY[i].x + sadBlockSize+1));
			cv::Mat currPatch = curr(cv::Range(corner[j].y - sadBlockSize, corner[j].y + sadBlockSize+1),
				cv::Range(corner[j].x - sadBlockSize, corner[j].x + sadBlockSize+1));

			sad[j] = cv::norm(prevPatch, currPatch, cv::NORM_L1);

			if (sad[j] < minSAD)
			{
				minSAD = sad[j];
				matchedCorner = corner[j];
			}
		}

		//std::cout << counterOverflow <<", "<< detectedNum<< std::endl;

		currXY.push_back(matchedCorner);

		//����֡�����ֵ��Ϊ��������ǰ֡-��һ֡��
		ofXY[i][0] = currXY[i].x - prevXY[i].x;
		ofXY[i][1] = currXY[i].y - prevXY[i].y;
		
		//std::cout << ofXY[i][0] << ", " << ofXY[i][1] << std::endl;

		//��������Ĺ�������ָ���������λ��ʱ��˵��������������������㱻�ڵ����������������ﵽ3ʱ����ô���³�ʼ�������¼������
		//֮������3���Ǹ������׵��������������ģ����ս���XY�������ȥ4���㣨�������ֵ��������Сֵ���������ȥ8���㣬�������ﵽ3��ʱ�򣬾��п���
		//��ȥ�����ˣ������������λ�Ƶĵ㣬�϶��Ǽ������ĵ㣩
		if (abs(ofXY[i][0]) >= maxPixShift || abs(ofXY[i][1]) >= maxPixShift)
		{
			counterFailure++;
			if (counterFailure == 4)
			{
				reInit = true;
				break;
			}
		}
	}

	if (reInit)
	{
		//���³�ʼ����������һ��Ҫ����
		prevXY.clear();
		currXY.clear();
		initGoodFeatures(
			prev,
			prevXY,
			numOF,
			40*numOF,
			maxPixShift,
			blockSize);

		calcOF(
			prev,
			curr,
			prevXY,
			currXY,
			ofXY,
			numOF,
			pyrLKSize,
			sadBlockSize,
			blockSize,
			maxPixShift);
	}
}