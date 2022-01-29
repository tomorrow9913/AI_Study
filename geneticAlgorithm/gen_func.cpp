/****************************************************************
 *  File Name:  gen_func
 * 
 * Description: 
 * 재산 분배 문제 해결을 위한 함수들을 모아둔 파일
 * 
 * Extern File
 * Name         Description
 * ============ ================================================
 * gen_head     자산 분배 알고리즘 학습을 위한 기호 상수 구조체 등을 선언한 파일
 * 
 * Extern Variables
 * Name         Type        Description
 * ============ =========== ====================================
 * g_person     char[]      분할 받을 사람들을 저장
 * g_ sumValue  int         전체 자산의 합
 * g_ sumGoal   int         간단한 정수비로 나타낸 목표치의 합
 * 
 * Global Variables
 * Name         Type        Description
 * ============ =========== ====================================
 * None
 ****************************************************************/

void CreateMutantGen(Gen_ genList[],int mutantPercent ,int mutantRatio);
double ComputeGenFitness(char gen[], int value[], const int goal[]);
void InitGeneric(Gen_ genList[]);
bool CmpFitness(Gen_ a, Gen_ b);
void CrossGen(Gen_ genList[]);
int Random(int min, int max);

#ifndef GEN_FUNC
    #define GEN_FUNC
    
    #include "gen_head.h"

    extern char g_person[];      // 자산을 분배 받을 사람
	extern int g_sumValue;      // 총 자산의 금액
	extern double g_sumGoal;       // 목표치 정수비의 합

    /****************************************************************
     * Function Name : Random
     * Description : 해당 구간의 수 중에서 랜덤의 수를 구함
     * @param : 하한선 상한선
     * @return random한 수
     * 
     * Author : Jeong MinGyu
     * Creat Date : 2020.01.20.
     * Last Edit : 2020.01.20.
     * 
     * Algorithm : none
     ***************************************************************/
    int Random(int min, int max) {
        return rand()%(max-min)+min;
    }

    /****************************************************************
     * Function Name : CmpFitness
     * Description : 정렬을 위한 비교 함수
     * @param : 비교할 2개의 유전자 구조체
     * @return bool 참 일때 swap
     * 
     * Author : Jeong MinGyu
     * Creat Date : 2020.01.14.
     * Last Edit : 2020.01.14.
     * 
     * Algorithm : none
     ***************************************************************/
    bool CmpFitness(Gen_ a, Gen_ b) {
        return (a.fitness < b.fitness);
    }

    /****************************************************************
     * Function Name : InitGeneric
     * Description : 초기 유전자 생성
     * @param : 유전자 정보를 저장할 배열
     * @return : void
     * 
     * Author : Jeong MinGyu
     * Creat Date : 2020.01.14.
     * Last Edit : 2020.01.14.
     * 
     * Algorithm : none
     ***************************************************************/
    void InitGeneric(Gen_ genList[]) {
        for (int i = 0; i < GEN_CNT; i++)
        {
            memset(genList[i].owner, 0, sizeof(genList[i].owner));
            for (int j = 0; j < ASSETS_CNT; j++)
            {
                genList[i].owner[j] = g_person[Random(0,POPULATE)];
            }
        }
    }

    /****************************************************************
     * Function Name : ComputeGenFitness
     * Description : 적응도 평가
     * @param : 유전자가 적힌 배열, 자산의 가치 배열, 목표값 배열
     * @return : int형의 적응도
     * 
     * Author : Jeong MinGyu
     * Creat Date : 2020.01.14.
     * Last Edit : 2020.01.17.
     * 
     * Global Variables
     * Name         Type        Description
     * ============ =========== ====================================
     * g_person     char[]      분할 받을 사람들을 저장
     * g_sumValue   int         전체 자산의 합
     * g_ sumGoal   double      간단한 정수비로 나타낸 목표치의 합
     * 
     * Algorithm : 
     * 비율을 고려한 금액(목표치)와 차이의 합
     ***************************************************************/
    double ComputeGenFitness(char gen[], int value[], const double goal[]) {
        double compareValue[POPULATE] = { 0 };
        double fitness = 0;

        // 각각의 자산이 목표치와 가까운 정도의 합으로 계산함.
        for (int i = 0; i < POPULATE; i++)
        {
            for (int j = 0; j < ASSETS_CNT; j++)
            {
                if (gen[j] == g_person[i])
                {
                    compareValue[i] += value[j];
                }
            }
            fitness += fabs(goal[i]-compareValue[i]);
        }

        return fitness;
    }

    /****************************************************************
     * Function Name : CreateMutantGen
     * Description : 돌연변이를 생성
     * @param : 유전자가 적힌 배열, 돌연변이의 확률과, 변화 비율
     * @return : void
     * 
     * Author : Jeong MinGyu
     * Creat Date : 2020.01.18.
     * Last Edit : 2020.01.18.
     * 
     * Global Variables
     * Name         Type        Description
     * ============ =========== ====================================
     * None
     * 
     * Algorithm : 
     * MUTANT_PERCENT의 확률로 엘리트를 제외한 유전자 중 랜덤으로 선택하여
     * 랜덤한 인덱스를 골라 랜덤 값으로 치환함
     ***************************************************************/
    void CreateMutantGen(Gen_ genList[],int mutantPercent ,int mutantRatio) {
        int percentage = Random(0,100);

        if (percentage < mutantPercent) {
            for (int i = 0; i < percentage; i++)
            {
                int selecteGen = Random(1, GEN_CNT-1);

                int changeCnt = Random(1,ASSETS_CNT*((double)mutantRatio/100));
                for (int i = 0; i < changeCnt; i++)
                {
                    int modifyIndex = Random(0,ASSETS_CNT);
                    genList[selecteGen].owner[modifyIndex] = g_person[Random(0,POPULATE)];
                }
            }
            return;
        }
    }

    /****************************************************************
     * Function Name : CrossGen
     * Description : 두 유전자를 선택하여 교차함
     * @param : 유전자가 적힌 배열
     * @return : void
     * 
     * Author : Jeong MinGyu
     * Creat Date : 2020.01.18.
     * Last Edit : 2020.01.18.
     * 
     * Global Variables
     * Name         Type        Description
     * ============ =========== ====================================
     * None
     * 
     * Algorithm : 
     * 룰렛 방식으로 우성 유전자와 다른 유전자를 선택
     * 보존할 엘리트를 제외하고 2개의 유전자를 교차.
     ***************************************************************/
    void CrossGen(Gen_ genList[]) {
        int boundary = Random(1, ASSETS_CNT);
        Gen_ copyGen[GEN_CNT];

        // 유전자 복사
        for (int i = 0; i < GEN_CNT; i++)
        {
            copyGen[i] = genList[i];
        }
        
        // 엘리트를 제외하고 세대 교체(단일 치환)
        for (int i = ELITE_CNT; i < GEN_CNT-1; i+=2)
        {
            int selecteGen = Random(0, GEN_CNT);

            if(i < GEN_CNT/2) {
                for (int j = 0; j < boundary; j++)
                {
                    genList[i].owner[j] = copyGen[selecteGen].owner[j];
                    genList[i+1].owner[j] = copyGen[0].owner[j];
                }
                for (int j = boundary; j < ASSETS_CNT; j++)
                {
                    genList[i].owner[j] = copyGen[0].owner[j];
                    genList[i+1].owner[j] = copyGen[selecteGen].owner[j];
                }
            }
            else if(i < (GEN_CNT/2)+(GEN_CNT/4)) {
                for (int j = 0; j < boundary; j++)
                {
                    genList[i].owner[j] = copyGen[selecteGen].owner[j];
                    genList[i+1].owner[j] = copyGen[1].owner[j];
                }
                for (int j = boundary; j < ASSETS_CNT; j++)
                {
                    genList[i].owner[j] = copyGen[1].owner[j];
                    genList[i+1].owner[j] = copyGen[selecteGen].owner[j];
                }
            }
            else if(i < (GEN_CNT/2)+(GEN_CNT/4)+(GEN_CNT/8)) {
                for (int j = 0; j < boundary; j++)
                {
                    genList[i].owner[j] = copyGen[selecteGen].owner[j];
                    genList[i+1].owner[j] = copyGen[2].owner[j];
                }
                for (int j = boundary; j < ASSETS_CNT; j++)
                {
                    genList[i].owner[j] = copyGen[2].owner[j];
                    genList[i+1].owner[j] = copyGen[selecteGen].owner[j];
                }
            }
            else {
                for (int j = 0; j < boundary; j++)
                {
                    genList[i].owner[j] = copyGen[selecteGen].owner[j];
                    genList[i+1].owner[j] = copyGen[3].owner[j];
                }
                for (int j = boundary; j < ASSETS_CNT; j++)
                {
                    genList[i].owner[j] = copyGen[3].owner[j];
                    genList[i+1].owner[j] = copyGen[selecteGen].owner[j];
                }
            }
            
        }
    }
#endif // !GEN_FUNC


