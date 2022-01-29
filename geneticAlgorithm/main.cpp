/****************************************************************
 * File Name:  main.c
 * 
 * Description: 분할 불가능할 재산을 일정 비율로 분배
 * 
 * Extern File
 * Name         Description
 * ============ ================================================
 * gen_func.cpp 함수들을 모아둔 파일
 * gen_head.h   구조체 및 기호 상수 적힌 일일
 *  
 * Extern Variables
 * Name         Type        Description
 * ============ =========== ====================================
 * g_person     char[]      분할 받을 사람들을 저장
 * g_ sumValue  int         전체 자산의 합
 * g_ sumGoal   int         간단한 정수비로 나타낸 목표치의 합
 * 
 * 
 * Global Variables
 * Name         Type        Description
 * ============ =========== ====================================
 * None
 ****************************************************************/

#include "gen_head.h"
#include "gen_func.cpp"

int main(int argc, char *argv[]) {
    extern char g_person[];      // 자산을 분배 받을 사람
	extern int g_sumValue;      // 총 자산의 금액
	extern double g_sumGoal;       // 목표치 정수비의 합

    srand(time(NULL));

    for (int i = 1; i < POPULATE; i++)
    {
        g_person[i] = g_person[i-1]+1;
    }

    Gen_ gen[GEN_CNT];
    double goal[POPULATE] = { 0 };
    int value[ASSETS_CNT] = { 0 };

    // 자산 가치 입력
    for (int i = 0; i < ASSETS_CNT; i++)
    {
        scanf("%d", &value[i]);
        g_sumValue += value[i];
    }

    // 목표치 입력
    for (int i = 0; i < POPULATE; i++)
    {
        scanf("%lf", &goal[i]);
        g_sumGoal += goal[i];
    }

    // 비율을 목표 금액으로 설정
    for (int i = 0; i < POPULATE; i++)
    {
        goal[i] = (goal[i]/g_sumGoal)*g_sumValue;
    }
    
    // 초기 유전자 생성(랜덤)
    InitGeneric(gen);

    // 적응도 평가
    for (int i = 0; i < GEN_CNT; i++)
    {
        gen[i].fitness = ComputeGenFitness(gen[i].owner, value, goal);
    }
    std::sort(gen, gen+GEN_CNT, CmpFitness);

    int generation = 0;
    int flag = 0;

    while ((generation++ < GEN_LIMIT) || flag)
    {
        if (kbhit()) {
          char ch=getch();
    
          if (ch == 'q') break;
          if (ch == 's') flag = 0;
          if (ch == 'r') flag = 1;
        }

        /* 세대 교체 */
        //선택, 교차
        CrossGen(gen);
        
        // 돌연변이
        CreateMutantGen(gen, MUTANT_PERCENT, CHANGE_RATIO);
        
        // 적응도 평가
        for (int i = 0; i < GEN_CNT; i++)
        {
            gen[i].fitness = ComputeGenFitness(gen[i].owner, value, goal);
        }
        std::sort(gen, gen+GEN_CNT, CmpFitness);

        // 가장 좋은 유전자 출력
        printf("GEN:%04d Gene:%s Adaptation:%lf\n", generation, gen[0].owner, gen[0].fitness);
    }

    // 최종 분배한 자산
    double asset[POPULATE] = {0};
    for (int i = 0; i < POPULATE; i++)
    {
        for (int j = 0; j < ASSETS_CNT; j++)
        {
            if(gen[0].owner[j] == g_person[i]) {
                asset[i] += value[j];
            }
        }
    }
    
    // 결과 출력
    printf("\nGoal: ");
    for (int i = 0; i < POPULATE; i++)
    {
        printf("%.0lf ", goal[i]);
    }
    
    printf("\nResult: ");
    for (int i = 0; i < POPULATE; i++) {
        printf("%.0lf ", asset[i]);
    }
    printf("Adaptation: %.2lf \nGene: %s ", gen[0].fitness, gen[0].owner);

    return 0;
}