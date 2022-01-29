#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

// 전역 변수/기호 상수
/*=================================================================*/

// 파일 명
string FILE_PATH = ".\\iris.data";

#define VIEW_WORKING				// 정의시 비교 확인
#undef VIEW_WORKING

#define SEARCH_CNT			3		// 가장 근접한 K개 확인
#define VALIDATION_PART		10		// 교차검증시 part 개수

int CLASS_CNT;						// 각 행의 마지막 정수로 분류(반드시 0부터 분류)

// 저장을 위한 데이터 클래스
/*=================================================================*/
class Data
{
public:
	vector<double> data;

	/****************************************************************
	 * Function Name : EuclidDistance
	 * Description : 유클리드 거리를 구함
	 *
	 * @param :
	 * Data other: 다른 데이터
	 *
	 * @return : double
	 *
	 * Author : Jeong MinGyu
	 * Creat Date : 2021.01.03.
	 * Last Edit :
	 ***************************************************************/
	double EuclidDistance(const Data& other) {
		int thisSize = data.size();
		int otherSize = other.data.size();

		int maxSize = otherSize > thisSize ? otherSize : thisSize;

		double sum = 0;
		for (int i = 0; i < maxSize; i++)
			sum += pow((i >= thisSize ? 0 : data[i]) - (i >= otherSize ? 0 : other.data[i]), 2);

		return sqrt(sum);
	}
};

// 함수 원형
/*=================================================================*/
void split(const string& str, const string& delim, vector<string>& parts);
void ReadFile(vector<pair<int, Data>>& data);
void PrintData(const vector<pair<int, Data>>& data);
void PrintData(const Data& data);
double CrossValidation(const vector<pair<int, Data>>& data, const int testPart, const int partCnt);
int NearestClass(vector<pair<int, Data>> data, const Data& comp);

// 메인
/*=================================================================*/
int main(int argc, char const* argv[])
{
	vector<pair<int, Data>> data;
	cout << FILE_PATH << endl << endl;
	ReadFile(data);

	double sumCorrectRate = 0;
	for (int i = 0; i < VALIDATION_PART; i++)
	{
		double correctRate = CrossValidation(data, i, VALIDATION_PART);
		cout << correctRate * 100 << "%";
		for (int i = 0; i < 15; i++) cout << " ";
		cout << endl;
		sumCorrectRate += correctRate;
	}

	cout << endl << "평균 정답률 : " << sumCorrectRate / VALIDATION_PART * 100 << "%" << endl;

	return 0;
}

// 함수 정의 
/*=================================================================*/

/****************************************************************
* Function Name : CrossValidation
* Description : 교차 검증을 위한 함수
*
* @param
* const vector<pair<int, Data>> &data : 전체 데이터
* int testPart : 테스트 할 부분
* int partCnt : N개의 파트로 나누어 하나를 검증용
*
* @return double : 정답률
*
* Author : Jeong MinGyu
* Creat Date : 2021.01.03.
* Last Edit :
***************************************************************/
double CrossValidation(const vector<pair<int, Data>>& data, int testPart, int partCnt) {
	vector<vector<Data>> partData;

	int dataSize = data.size();
	for (int i = 0; i < CLASS_CNT; i++) partData.push_back(vector<Data>());
	for (int i = 0; i < dataSize; i++) partData[data[i].first].push_back(data[i].second);

	vector<pair<int, Data>> training, validation;
	for (size_t i = 0; i < partData.size(); i++) {
		int classSize = partData[i].size();
		int partSize = classSize / partCnt;
		int start = partSize * testPart, end = partSize * testPart + partSize;

		for (int j = 0; j < classSize; j++)
		{
			if (j >= start && j < end) {
				validation.push_back(pair<int, Data>(i, partData[i][j]));
				continue;
			}
			training.push_back(pair<int, Data>(i, partData[i][j]));
		}
	}

	int validationSize = validation.size(), correct = 0;
	for (int i = 0; i < validationSize; i++)
	{
		int ans;
		if (validation[i].first == (ans = NearestClass(training, validation[i].second)))
			correct++;


#ifndef VIEW_WORKING
		cout << testPart + 1 << "/" << partCnt << " : ";
		int progress = i / (double)validationSize * 10;
		int rem = (int)(i / (double)validationSize * 100) % 10;

		cout << "[";
		for (int i = 0; i < progress; i++) cout << "=";
		cout << rem;
		for (int i = 0; i < 9 - progress; i++) cout << "-";
		cout << "]" << progress << rem << "%\r";
#else
		PrintData(validation[i].second);
		cout << " : " << validation[i].first << "  Answer: " << ans << endl;
#endif // !VIEW_WORKING
	}
	cout << testPart + 1 << "/" << partCnt << " : ";

	return correct / (double)validationSize;
}

/****************************************************************
* Function Name : NearestClass
* Description : 가장 근접한 점을 구함
*
* @param
* vector<pair<int, Data>> data : 전체 데이터
* const Data &comp : 비교할 한 점
*
* @return int : 분류
*
* Author : Jeong MinGyu
* Creat Date : 2021.01.03.
* Last Edit :
***************************************************************/
int NearestClass(vector<pair<int, Data>> data, const Data& comp) {
	int result = 0;

	struct Comp {
		Data param;

		Comp(Data param) { this->param = param; }
		bool operator()(pair<int, Data> i, pair<int, Data> j) {
			return param.EuclidDistance(i.second) < param.EuclidDistance(j.second);
		}
	};

	sort(data.begin(), data.end(), Comp(comp));

	int* check = new int[CLASS_CNT]();
	for (int i = 1; i <= SEARCH_CNT; i++)
		check[data[i].first]++;

	for (int i = 0; i < CLASS_CNT; i++)
		if (check[i] > check[result]) result = i; 

	delete[] check;
	return result;
}

/****************************************************************
* Function Name : PrintData
* Description : 출력
*
* @param
* const vector<pair<int, Data>> &data : 출력할 데이터
*
* @return int : void
*
* Author : Jeong MinGyu
* Creat Date : 2021.01.03.
* Last Edit :
***************************************************************/
void PrintData(const vector<pair<int, Data>>& data) {
	for (auto& i : data) {
		cout << "[" << i.first << ", (";
		for (auto& j : i.second.data) cout << j << " ";
		cout << ")] " << endl;
	}
}

/****************************************************************
* Function Name : PrintData
* Description : 출력
*
* @param
* const Data &data : 출력할 데이터
*
* @return int : void
*
* Author : Jeong MinGyu
* Creat Date : 2021.01.03.
* Last Edit :
***************************************************************/
void PrintData(const Data& data) {
	cout << "(";
	for (auto& i : data.data) cout << i << " ";
	cout << ")";
}

/****************************************************************
* Function Name : split
* Description : 특정기호를 기준으로 자름
*
* @param
* const string& str : 입력 문자열
* const string& : 자르는데 쓰이는 기준 문자열
* vector<string>& parts : 자른 문자들을 저장할 벡터
*
* @return int : void
*
* Author : Jeong MinGyu
* Creat Date : 2021.01.03.
* Last Edit :
***************************************************************/
void split(const string& str, const string& delim, vector<string>& parts) {
	size_t start, end = 0;
	while (end < str.size()) {
		start = end;
		while (start < str.size() && (delim.find(str[start]) != string::npos)) {
			start++;  // skip initial whitespace
		}
		end = start;
		while (end < str.size() && (delim.find(str[end]) == string::npos)) {
			end++; // skip to end of word
		}
		if (end - start != 0) {  // just ignore zero-length strings.
			parts.push_back(string(str, start, end - start));
		}
	}
}

/****************************************************************
* Function Name : ReadFile
* Description : 파일을 읽어서 입력값 저장
*
* @param
* const string &filePath : 읽을 파일의 경로
* vector<pair<int, Data>> &data : 값을 저장할 벡터
*
* @return int : void
*
* Author : Jeong MinGyu
* Creat Date : 2021.01.03.
* Last Edit :
***************************************************************/
void ReadFile(vector<pair<int, Data>>& data) {
	ifstream openFile(FILE_PATH.data());

	if (openFile.is_open()) {
		set<int> s;
		string line;

		while (getline(openFile, line)) {
			Data tmp;
			int kind;
			vector<string> info;
			split(line, " ", info);

			for (auto& i : info) tmp.data.push_back(stod(i));

			kind = (int)tmp.data.back();
			s.insert(kind);
			tmp.data.erase(tmp.data.end() - 1);

			data.push_back(pair<int, Data>(kind, tmp));
		}

		CLASS_CNT = s.size();
		openFile.close();
		return;
	}
	
	cout << "file open failed" << endl;
}