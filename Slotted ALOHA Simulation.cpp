#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

//초보 개발자 육기훈


double Input_Random_Number()							//랜덤한 숫자를 출력하는 함수.
{
	double Random = 0.0;

	srand(GetTickCount());								//랜덤 숫자 부여는 초를 씨앗값으로 가지기 떄문에, 같은 숫자가 출력된다. 이를 방지하기 위해서 밀리세크단위로 시간을 반환하는 GetTickCount를 이용한다.

	Random = (double)(rand() % 1000) / 1000.0;			//Random에 0 ~ 1사이의 랜덤한 숫자 대입. ex) 0, 0.002, 0.179등... 1000으로 나눌 시 int값으로 나누는 것이기 때문에, 0이 반환된다.

	return Random;
}


void Input_Array(int *User, double P)
{
	double Check[100];									//랜덤한 숫자를 집어넣는 배열.

	Check[0] = Input_Random_Number();

	for (int i = 1; i < 100; i++)						//랜덤한 숫자를 집어 넣는다.		
	{
		Check[i] = Input_Random_Number();

		while (Check[i] == Check[i-1])
			Check[i] = Input_Random_Number();
	}

	for (int j = 0; j < 100; j++)
	{
		if (Check[j] < P)								//G보다 작거나 같으면 =  G만큼의 확률일 떄 = 확률이 발생 했을 때, 1을 대입.
			User[j] = 1;
		else											//반대로 G보다 크면, 즉 확률이 발생하지 않았을 때, 0을 대입.
			User[j] = 0;
	}
}

void main()
{
	int User[100] = {0};								//Slotted ALOHA를 사용하는 유저들이 데이터를 보내는지, 안 보내는지 확인하는 배열.
	double P = 0;										//G값을 대입시킨다.
	FILE *fp;

	int All_Sum = 0;									//User에 들어가는 모든 성공의 합. 확률에서 분모 역활을 해준다.
	int Success_Sum = 0;								//User에서 보내기에 성공한 경우의 합. 확률에서 분자 역활을 해준다.
	int Now_Sum = 0;									//User에서 각 상황에서 성공 했는지, 실패했는지 확인해주는 역활을 해준다.
	fopen_s(&fp, "test.txt", "w");

	double probability = 0;								//All_Sum / Success_Sum 값을 넣어준다. 즉 확률을 나타낸다.


	for (int i = 0; i < 20; i++)						//각 상황을 G값을 변경하면서 20번 수행한다.
	{
		printf("P값을 입력해주세요 :");					//P값을 입력받는다.
		scanf_s("%lf", &P);
		printf("\n\n");									//칸 맞추기를 위한 센스있는 \n

		for (int j = 0; j < 1000; j++)					//각 상황을 1000번 수행.
		{
			Input_Array(User, P);

			for (int k = 0; k < 100; k++)				//배열 100개중에서 몇개가 전송을 시도하였는지 확인한다.
				if (User[k] == 1)
					Now_Sum++;
			
			if (Now_Sum == 1)							//Now_Sum이 1일 경우, 즉, 1개만 보낸경우 = 전송에 성공한 경우이므로 Success_Sum에 1개 추가해준다.
				Success_Sum++;

			All_Sum += Now_Sum;							//전송에 성공했던 실패했던 All_Sum에 Now_Sum값을 더해준다.
			Now_Sum = 0;								//Now_Sum이 계산 오류가 없도록 0으로 다시 초기화 시켜준다.
		}

		probability = (double)Success_Sum / (double)All_Sum;

		printf("All_Sum의 값 = %d\n", All_Sum);
		printf("Success_Sum의 값 = %d\n", Success_Sum);
		printf("확률 : %lf\n\n", probability);

		fprintf_s(fp,"%d %d %.3lf\n",All_Sum,Success_Sum, probability);

		All_Sum = 0;
		Success_Sum = 0;
	}

	fflush(fp);
	fclose(fp);
}