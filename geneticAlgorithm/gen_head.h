/****************************************************************
 *  File Name: gen_head.h
 * 
 * Description: 
 * 자산 분배 알고리즘 학습을 위한 기호 상수 구조체 등을 선언한 파일
 * 
 * Global Variables
 * 
 * Name         Type    Description
 * ============ ======= =======================================
 * g_preson		char[]	자산을 분배 받을 사람
 * g_sumValue	int		자산의 총 금액
 * g_sumGoal	int		목표치 정수비의 합
 ****************************************************************/
#ifndef GEN_HEAD
	#define GEN_HEAD

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdint.h>
	#include <math.h>
	#include <time.h>
	#include <conio.h>
	#include <algorithm>

	#define ASSETS_CNT  200			// 자산의 갯수
	#define GEN_CNT     3000		// 유전자의 갯수
	#define POPULATE    15			// 자산을 나눌 사람의 수

	#define GEN_LIMIT           500		// 진화를 멈출 세대의 수
	#define MUTANT_PERCENT      30		// 돌연변이가 생길 확률(%)
	#define CHANGE_RATIO        4		// 돌연변이가 생길 때 변화가 생기는 비율(%)
	#define ELITE_CNT  			2		// 엘리트의 수

	typedef struct Gen {
		char owner[ASSETS_CNT+1];
		double fitness;		
	}Gen_;

	char g_person[POPULATE] = {'a'};  	// 자산을 분배 받을 사람
	int g_sumValue;     				// 총 자산의 금액
	double g_sumGoal;     				// 목표치의 합
#endif // !GEN_HEAD