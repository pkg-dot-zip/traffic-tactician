/**
 * This file contains all the code that calculates the points for the pose estimation. No other file but poseChecker.cpp should
 * interact with this code EVER.
 *
 * This code is inspired by https://learnopencv.com/multi-person-pose-estimation-in-opencv-using-openpose/ and uses a model trained by
 * Carnegie Mellon’s School of Computer Science.
 * The code is HEAVILY modified to be able to run on CPUs instead of the GPU (for portability reasons). This means that although the results are less accurate,
 * we are able to process more images than with the default values & code.
 *
 * In het Nederlands:
 * AFBLIJVEN!
 */


#include <iostream>
#include <random>
#include <set>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/dnn.hpp>

#include "easylogging++.h"
#include "keyPoint.h"
#include "poseChecker.h"
#include "settingsFromJson.h"

////////////////////////////////
std::ostream& operator <<(std::ostream& os, const KeyPoint& kp)
{
	os << "Id:" << kp.id << ", Point:" << kp.point << ", Prob:" << kp.probability << std::endl;
	return os;
}

////////////////////////////////
struct ValidPair
{
	ValidPair(const int aId, const int bId, const float score)
	{
		this->aId = aId;
		this->bId = bId;
		this->score = score;
	}

	int aId;
	int bId;
	float score;
};

std::ostream& operator <<(std::ostream& os, const ValidPair& vp)
{
	os << "A:" << vp.aId << ", B:" << vp.bId << ", score:" << vp.score << std::endl;
	return os;
}

////////////////////////////////

template <class T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	bool first = true;
	for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii, first = false)
	{
		if (!first) os << ",";
		os << " " << *ii;
	}
	os << "]";
	return os;
}

template <class T>
std::ostream& operator <<(std::ostream& os, const std::set<T>& v)
{
	os << "[";
	bool first = true;
	for (typename std::set<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii, first = false)
	{
		if (!first) os << ",";
		os << " " << *ii;
	}
	os << "]";
	return os;
}

////////////////////////////////

constexpr int nPoints = 18;

const std::string keypointsMapping[] = {
	"Nose", "Neck",
	"R-Sho", "R-Elb", "R-Wr",
	"L-Sho", "L-Elb", "L-Wr",
	"R-Hip", "R-Knee", "R-Ank",
	"L-Hip", "L-Knee", "L-Ank",
	"R-Eye", "L-Eye", "R-Ear", "L-Ear"
};

const std::vector<std::pair<int, int>> mapIdx = {
	{31, 32}, {39, 40}, {33, 34}, {35, 36}, {41, 42}, {43, 44},
	{19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30},
	{47, 48}, {49, 50}, {53, 54}, {51, 52}, {55, 56}, {37, 38},
	{45, 46}
};

const std::vector<std::pair<int, int>> posePairs = {
	{1, 2}, {1, 5}, {2, 3}, {3, 4}, {5, 6}, {6, 7},
	{1, 8}, {8, 9}, {9, 10}, {1, 11}, {11, 12}, {12, 13},
	{1, 0}, {0, 14}, {14, 16}, {0, 15}, {15, 17}, {2, 17},
	{5, 16}
};

static void getKeyPoints(cv::Mat& probMap, const double threshold, std::vector<KeyPoint>& keyPoints)
{
	const auto ksize = cv::Size(settings.blurKSize, settings.blurKSize);
	constexpr int sigmaX = settings.blurSigmaX;
	constexpr int sigmaY = settings.blurSigmaY;

	cv::Mat smoothProbMap;
	cv::GaussianBlur(probMap, smoothProbMap, ksize, sigmaX, sigmaY);

	cv::Mat maskedProbMap;
	cv::threshold(smoothProbMap, maskedProbMap, threshold, 255, cv::THRESH_BINARY);

	maskedProbMap.convertTo(maskedProbMap, CV_8U, 1);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(maskedProbMap, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i)
	{
		cv::Mat blobMask = cv::Mat::zeros(smoothProbMap.rows, smoothProbMap.cols, smoothProbMap.type());

		cv::fillConvexPoly(blobMask, contours[i], cv::Scalar(1));

		double maxVal;
		cv::Point maxLoc;

		cv::minMaxLoc(smoothProbMap.mul(blobMask), 0, &maxVal, 0, &maxLoc);

		keyPoints.push_back(KeyPoint(maxLoc, probMap.at<float>(maxLoc.y, maxLoc.x)));
	}
}

static void populateRandomColorPalette(std::vector<cv::Scalar>& colors, const int nColors)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis1(64, 200);
	std::uniform_int_distribution<> dis2(100, 255);
	std::uniform_int_distribution<> dis3(100, 255);

	for (int i = 0; i < nColors; ++i)
	{
		colors.push_back(cv::Scalar(dis1(gen), dis2(gen), dis3(gen)));
	}
}

// TODO: Manually put in colors.
static void populateColorPalette(std::vector<cv::Scalar>& colors, const int nColors)
{
	for (int i = 0; i < nColors; ++i)
	{
		colors.push_back(cv::Scalar(0, 0, 0));
	}
}

static void splitNetOutputBlobToParts(cv::Mat& netOutputBlob, const cv::Size& targetSize,
                                      std::vector<cv::Mat>& netOutputParts)
{
	const int nParts = netOutputBlob.size[1];
	const int height = netOutputBlob.size[2];
	const int width = netOutputBlob.size[3];

	for (int i = 0; i < nParts; ++i)
	{
		cv::Mat part(height, width, CV_32F, netOutputBlob.ptr(0, i));

		cv::Mat resizedPart;

		cv::resize(part, resizedPart, targetSize);

		netOutputParts.push_back(resizedPart);
	}
}

static void populateInterpPoints(const cv::Point& a, const cv::Point& b, int numPoints,
                                 std::vector<cv::Point>& interpCoords)
{
	const float xStep = static_cast<float>(b.x - a.x) / static_cast<float>(numPoints - 1);
	const float yStep = static_cast<float>(b.y - a.y) / static_cast<float>(numPoints - 1);

	interpCoords.push_back(a);

	for (int i = 1; i < numPoints - 1; ++i)
	{
		interpCoords.push_back(cv::Point(a.x + xStep * i, a.y + yStep * i));
	}

	interpCoords.push_back(b);
}


void getValidPairs(const std::vector<cv::Mat>& netOutputParts,
                   const std::vector<std::vector<KeyPoint>>& detectedKeypoints,
                   std::vector<std::vector<ValidPair>>& validPairs,
                   std::set<int>& invalidPairs)
{
	constexpr int nInterpSamples = settings.nInterpSamples;
	constexpr float pafScoreTh = settings.pafScoreTh;
	constexpr float confTh = settings.confTh;

	for (int k = 0; k < mapIdx.size(); ++k)
	{
		// A->B constitute a limb
		cv::Mat pafA = netOutputParts[mapIdx[k].first];
		cv::Mat pafB = netOutputParts[mapIdx[k].second];

		// Find the keypoints for the first and second limb
		const std::vector<KeyPoint>& candA = detectedKeypoints[posePairs[k].first];
		const std::vector<KeyPoint>& candB = detectedKeypoints[posePairs[k].second];

		const int nA = candA.size();
		const int nB = candB.size();

		/*
		  # If keypoints for the joint-pair is detected
		  # check every joint in candA with every joint in candB
		  # Calculate the distance vector between the two joints
		  # Find the PAF values at a set of interpolated points between the joints
		  # Use the above formula to compute a score to mark the connection valid
		 */

		if (nA != 0 && nB != 0)
		{
			std::vector<ValidPair> localValidPairs;

			for (int i = 0; i < nA; ++i)
			{
				int maxJ = -1;
				float maxScore = -1;
				bool found = false;

				for (int j = 0; j < nB; ++j)
				{
					std::pair<float, float> distance(candB[j].point.x - candA[i].point.x,
					                                 candB[j].point.y - candA[i].point.y);

					const float norm = std::sqrt(distance.first * distance.first + distance.second * distance.second);

					if (!norm) continue;

					distance.first /= norm;
					distance.second /= norm;

					//Find p(u)
					std::vector<cv::Point> interpCoords;
					populateInterpPoints(candA[i].point, candB[j].point, nInterpSamples, interpCoords);
					//Find L(p(u))
					std::vector<std::pair<float, float>> pafInterp;
					for (int l = 0; l < interpCoords.size(); ++l)
					{
						pafInterp.push_back(
							std::pair(
								pafA.at<float>(interpCoords[l].y, interpCoords[l].x),
								pafB.at<float>(interpCoords[l].y, interpCoords[l].x)
							));
					}

					std::vector<float> pafScores;
					float sumOfPafScores = 0;
					int numOverTh = 0;
					for (int l = 0; l < pafInterp.size(); ++l)
					{
						float score = pafInterp[l].first * distance.first + pafInterp[l].second * distance.second;
						sumOfPafScores += score;
						if (score > pafScoreTh) ++numOverTh;

						pafScores.push_back(score);
					}

					const float avgPafScore = sumOfPafScores / static_cast<float>(pafInterp.size());

					if (static_cast<float>(numOverTh) / static_cast<float>(nInterpSamples) > confTh)
					{
						if (avgPafScore > maxScore)
						{
							maxJ = j;
							maxScore = avgPafScore;
							found = true;
						}
					}
				} /* j */

				if (found)
				{
					localValidPairs.push_back(ValidPair(candA[i].id, candB[maxJ].id, maxScore));
				}
			} /* i */

			validPairs.push_back(localValidPairs);
		}
		else
		{
			invalidPairs.insert(k);
			validPairs.push_back(std::vector<ValidPair>());
		}
	} /* k */
}

void getPersonwiseKeypoints(const std::vector<std::vector<ValidPair>>& validPairs,
                            const std::set<int>& invalidPairs,
                            std::vector<std::vector<int>>& personwiseKeypoints)
{
	for (int k = 0; k < mapIdx.size(); ++k)
	{
		if (invalidPairs.find(k) != invalidPairs.end()) continue;

		const std::vector<ValidPair>& localValidPairs(validPairs[k]);

		const int indexA(posePairs[k].first);
		const int indexB(posePairs[k].second);

		for (int i = 0; i < localValidPairs.size(); ++i)
		{
			bool found = false;
			int personIdx = -1;

			for (int j = 0; !found && j < personwiseKeypoints.size(); ++j)
			{
				if (indexA < personwiseKeypoints[j].size() &&
					personwiseKeypoints[j][indexA] == localValidPairs[i].aId)
				{
					personIdx = j;
					found = true;
				}
			} /* j */

			if (found)
			{
				personwiseKeypoints[personIdx].at(indexB) = localValidPairs[i].bId;
			}
			else if (k < 17)
			{
				std::vector<int> lpkp(std::vector<int>(18, -1));

				lpkp.at(indexA) = localValidPairs[i].aId;
				lpkp.at(indexB) = localValidPairs[i].bId;

				personwiseKeypoints.push_back(lpkp);
			}
		} /* i */
	} /* k */
}

constexpr int spatialSizeFactor = settings.spatialSizeFactor;
// Typically larger, we however don't need good accuracy at all and will probably not go over 200.

// TODO: Look into usage if UMAT & MATEXPR to see if we can optimize this further. -> Benchmark.
void getCalculatedPose(std::map<std::string, std::vector<KeyPoint>>& keyPointsToUseInCalculation, cv::Mat& input,
                       cv::Mat& outputFrame, cv::dnn::Net& inputNet)
{
	if (!keyPointsToUseInCalculation.empty()) throw std::exception("Map to save points in is not empty.");

	// For reference:
	// 1. https://pyimagesearch.com/2017/11/06/deep-learning-opencvs-blobfromimage-works/
	// 2. https://docs.opencv.org/3.4/d6/d0f/group__dnn.html#ga33d1b39b53a891e98a654fdeabba22eb
	constexpr double scaleFactor = 1.0 / 255.0;
	const int spatialSizeWidth = spatialSizeFactor * input.cols / input.rows;
	// TODO: We know webcam res. Hardcode this & put in setings.json so we can constexpr everywhere. NOT JUST HERE. WE USE .cols AND .rows EVERYWHERE!
	constexpr int spatialSizeHeight = spatialSizeFactor;
	const auto mean = cv::Scalar(0, 0, 0);

	cv::Mat inputBlob = cv::dnn::blobFromImage(input, scaleFactor,
	                                           cv::Size(spatialSizeWidth, spatialSizeHeight),
	                                           mean, false, false);

	inputNet.setInput(inputBlob);

	cv::Mat netOutputBlob = inputNet.forward();

	std::vector<cv::Mat> netOutputParts;
	splitNetOutputBlobToParts(netOutputBlob, cv::Size(input.cols, input.rows), netOutputParts);


	int keyPointId = 0;
	std::vector<std::vector<KeyPoint>> detectedKeypoints;
	std::vector<KeyPoint> keyPointsList;

	for (int i = 0; i < nPoints; ++i)
	{
		std::vector<KeyPoint> keyPoints;

		getKeyPoints(netOutputParts[i], settings.confidenceMapThreshold, keyPoints);

		// LOG(INFO) << "Keypoints - " << keypointsMapping[i] << " : " << keyPoints << std::endl;

		keyPointsToUseInCalculation.insert(std::make_pair(keypointsMapping[i], keyPoints));

		for (int j = 0; j < keyPoints.size(); ++j, ++keyPointId)
		{
			keyPoints[j].id = keyPointId;
		}

		detectedKeypoints.push_back(keyPoints);
		keyPointsList.insert(keyPointsList.end(), keyPoints.begin(), keyPoints.end());
	}

	checkPoseForAll(keyPointsToUseInCalculation);

	std::vector<cv::Scalar> colors;

	if (settings.useColorsForPose)
	{
		populateRandomColorPalette(colors, nPoints);
	}
	else
	{
		populateColorPalette(colors, nPoints);
	}

	outputFrame = input.clone();

	for (int i = 0; i < nPoints; ++i)
	{
		for (int j = 0; j < detectedKeypoints[i].size(); ++j)
		{
			cv::circle(outputFrame, detectedKeypoints[i][j].point, 5, colors[i], -1, cv::LINE_AA);
		}
	}

	std::vector<std::vector<ValidPair>> validPairs;
	std::set<int> invalidPairs;
	getValidPairs(netOutputParts, detectedKeypoints, validPairs, invalidPairs);

	std::vector<std::vector<int>> personwiseKeypoints;
	getPersonwiseKeypoints(validPairs, invalidPairs, personwiseKeypoints);

	for (int i = 0; i < nPoints - 1; ++i)
	{
		for (int n = 0; n < personwiseKeypoints.size(); ++n)
		{
			const std::pair<int, int>& posePair = posePairs[i];
			const int indexA = personwiseKeypoints[n][posePair.first];
			const int indexB = personwiseKeypoints[n][posePair.second];

			if (indexA == -1 || indexB == -1) continue;

			const KeyPoint& kpA = keyPointsList[indexA];
			const KeyPoint& kpB = keyPointsList[indexB];

			cv::line(outputFrame, kpA.point, kpB.point, colors[i], 3, cv::LINE_AA);
		}
	}
}

// Methods under here are allowed to be called by other files.
std::map<std::string, std::vector<KeyPoint>> poseEstimationKeyPoints;

constexpr double upscaleFactor = settings.upscaleFactor;
constexpr double downscaleFactor = settings.downscaleFactor;

std::map<std::string, std::vector<KeyPoint>>& getPoseEstimationKeyPointsMap(cv::Mat& input,
                                                                            cv::Mat& outputFrame,
                                                                            cv::dnn::Net& inputNet)
{
	// First we downscale the image.
	cv::resize(input, input, cv::Size(), downscaleFactor, downscaleFactor, cv::INTER_AREA);
	// INTER_AREA is better than the default (INTER_LINEAR) for camera views, according to a Stackoverflow user. TODO: CHECK IF THIS IS TRUE.
	LOG(INFO) << "AFTER DOWNSCALING - Width: " << input.rows << " | Height: " << input.cols << std::endl;

	// Then we retrieve the estimationpoints.
	const int64 timeStart = cv::getTickCount();
	getCalculatedPose(poseEstimationKeyPoints, input, outputFrame, inputNet);
	const int64 timeEnd = cv::getTickCount();

	const double time = (timeEnd - timeStart) / cv::getTickFrequency();

	LOG(INFO) << "Time it took to retrieve the poseEstimationKeyPoints: " << time << std::endl;

	// Then we upscale and flip.  We flip the mat here so that our cam view looks more natural; it confuses the user to see his left arm on the right side of his screen.
	cv::resize(outputFrame, outputFrame, cv::Size(), upscaleFactor, upscaleFactor);
	cv::flip(outputFrame, outputFrame, 1);

	return poseEstimationKeyPoints;
}


// Clears the keypoints map.
void clearPoseEstimationKeyPointsMap()
{
	poseEstimationKeyPoints.clear();
}
