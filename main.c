#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include<Windows.h>
#include<Windows.h>
#include<conio.h>
#include <time.h>


#define Y 19
#define X 19
#define empty 0
#define black 1
#define white 2
#define user_turn 0
#define computer_turn 1
#define depth_limit 3 //시간제한으로 인해 깊이는 3로 제한

#define DEFENSE_BLOCK_ 10 //한개의 블럭을 방어 했을시 가중치


int map[Y][X]; //바둑판 생성, 0은 비어있음, 1은 흑 2는 백
int state[Y][X]; //서치하면서 이용하는 임시 map
int user_color; //유저가 흑인지 백인지
int computer_color; //유저가 흑인지 백인지

typedef struct move_ {  
	int x; //x값
	int y; //y값

}move;

typedef struct move_and_value_ { //move와 int(value)값 , depth 3개를 동시에 return하기 위해 사용한다
	move move_;
	int value;
	int depth;
}move_and_value;

void rendering();
void create_map();
void game_run();
int game_init();
void user_input();
void computer_input();
int game_progress(int);
int isFinish();
int evaluate();//평가함수
move Heuristic_Alpha_Beta_Search();
move_and_value Max_Value(int, int, int);
move_and_value Min_Value(int, int, int); 
move_and_value returnResult(move, int, int); //2개의 합을 합치고 depth 1감소 시킨후 리턴
//int evaluateWeight(int, int); //가중치 평가
void finish(int, int);
int computerWeight(int, int);
int userWeight(int, int);



int main(void) {
	
	game_run();

	return 0;
}


//gui 이건 제작할지 미지수
void rendering() {

}

void game_run() {
	int turn; 
	int result; //게임이 어떻게 끝났는지,

	create_map();// 맵 생성
	turn = game_init(); //게임 시작, 누가 먼저시작인지에 따라 턴이 정해진다
	result = game_progress(turn); //init에서 정해진 turn을 바탕으로 게임 진행
	finish(result, turn);
 
};

void create_map() {
	//map생성
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			map[i][j] = empty; 
		}
	}

	//state 생성
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			state[i][j] = empty; 
		}
	}

}

//게임 시작, 돌 색깔 변경, 누구 턴인지 정하고, 필요에 따라유저의 인풋을 받는다
int game_init() {
	int user;
	int turn;
	srand(time(NULL)); // 매번 다른 시드값 생성

	printf("* 제한시간은 %d초 입니다.\n", 15);
	printf("* 흑이 먼저 시작합니다.\n");
	printf("* 입력은 정수와 공백만 가능합니다 ex)좌표 입력하려면 %d %d 이런 형식으로 입력\n", 8, 8);
	printf("* 흑을 선택하려면 %d 백을 선택하려면 %d를 입력하세요\n", 1, 2);
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	

	do {
		scanf("%d", &user);
		if (user == black) {
			printf("흑 선택!\n");
			user_color = black;
			computer_color = white;
			turn = user_turn;
		}
		else if (user == white) {
			printf("백 선택!\n");
			user_color = white;
			computer_color = black;
			turn = computer_turn;
		}
		else { //입력식 벗어남
			printf("%d과 %d만 입력하세요\n", 1, 2);
		}
	} while ((user != 1) && (user != 2));


	//원활한 게임 진행을 취해 처음 1,2수는 다른 알고리즘으로 둔다
	if (turn == computer_turn) { //컴퓨터가 첫 수를 두는 경우 중앙에 랜덤으로 둠
	
		int randomX = 0;
		int randomY = 0;

		randomX = rand() % 5 + 5;
		randomY = rand() % 5 + 5;

		//결과
		printf("컴퓨터의 수 (%d , %d)", randomX, randomY);

		map[randomY][randomX] = computer_color;
		state[randomY][randomX] = computer_color;
		turn = user_turn;
	}
	else { //유저가 첫수를 두는 경우, 컴퓨터의 두번째수는 첫수를 기준으로 랜덤으로 둠
		user_input(); //첫번째 입력을 받는다
		
		int x, y;
		int plus_x = 0, plus_y = 0;

		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				if (map[i][j] != empty) {
					y = i;
					x = j;
					break;
				}
			}
		}

		//최대 2칸까지 떨어지도록 한다,
		while (x + plus_x > 18 || x + plus_x < 0 || y + plus_y > 18 || y + plus_y < 0 || (plus_x == 0 && plus_y == 0)) { 
			plus_x = (rand() % 5) - 2;
			plus_y = (rand() % 5) - 2;
		}

		map[y + plus_y][x + plus_x] = computer_color;
		state[y + plus_y][x + plus_x] = computer_color;

		//결과
		printf("컴퓨터의 수 (%d , %d)", x + plus_x, y + plus_y);





	}

	return turn; //턴 리턴

}

//사용자로부터 위치를 입력받고 map변경
void user_input() {
	int x, y;

	printf("돌 놓을 좌표 입력 (X축 좌표 Y축 좌표)\n"); 
	
	//입력받은 돌 유효성 검사
	do {
		scanf("%d %d", &x, &y);

		
		if (x < 0 || x > 20 || y < 0 || y > 20) {
			printf("좌표값이 범위를 초과했습니다\n");
		}

		if (map[y][x] != empty) {
			printf("이미 착수된 자리입니다\n");
		}


	} while (x < 0 || x > 20 || y < 0 || y > 20 || map[y][x] != 0); //하나라도 통과못하면 다시 입력받는다

	//유효성 통과시 map변경
	map[y][x] = user_color;
	state[y][x] = user_color;
}

//컴퓨터로부터 위치를 입력받고 맵 변경

void computer_input() {
	move result;
	result = Heuristic_Alpha_Beta_Search();
	
	//알고리즘 결과에 따라 출력후 색깔변경
	printf("컴퓨터의 수 (%d , %d)", result.x, result.y);
	map[result.y][result.x] = computer_color;
	state[result.y][result.x] = computer_color;
}



//게임 진행
int game_progress(int turn) {
	time_t start, finish;

	while (1) { //finish할때까지 게임 반복
		start = time(NULL);
		double duration;

		//턴 진행
		if (turn == computer_turn) { //컴퓨터 턴이면 휴리스틱 알파베타 알고리즘 실행
			computer_input();
			//turn = user_turn;
		}
		else if (turn == user_turn) { //유저 턴이면 입력을 받는다
			user_input();
			//turn = computer_turn;
		}

		finish = time(NULL);
		duration = (double)(finish - start);


		//finish조건 
		if (duration > 15) { //시간 초과되면 실격패
			if (turn == computer_turn) {
				return 10;
			}
			else {
				return 11;
			}

			
		}

		if (isFinish()) { //오목이 완성되면 종료
			if (turn == computer_turn) {
				return 20;
			}
			else {
				return 21;
			}
		}

	
		//finish하지 하지 않았다면 턴 스위칭
		if (turn == computer_turn) { 

			turn = user_turn;
		}
		else if (turn == user_turn) { 
			
			turn = computer_turn;
		}

	}


}

//////////////////////////////////////////
int isFinish() {
	//세로 판별
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int count = 0;
			int color;
			if (map[i][j] != empty && i < 15 ) {
				color = map[i][j]; 
				for (int k = 0; k < 5; k++) {
					if (map[i + k][j] == color) {
						count++;
					}
					else {
						break;
					}
				}

				if (count >= 5) {
					return 1;
				}

			}
		}
	}
	//가로 판별
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int count = 0;
			int color;
			if (map[i][j] != empty && i < 15) {
				color = map[i][j];
				for (int k = 0; k < 5; k++) {
					if (map[i][j + k] == color) {
						count++;
					}
					else {
						break;
					}
				}

				if (count >= 5) {
					return 1;
				}

			}
		}
	}

	//오른쪽 대각선 판별
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int count = 0;
			int color;
			if (map[i][j] != empty && i < 15) {
				color = map[i][j];
				for (int k = 0; k < 5; k++) {
					if (map[i+k][j + k] == color) {
						count++;
					}
					else {
						break;
					}
				}

				if (count >= 5) {
					return 1;
				}

			}
		}
	}

	//왼쪽 대각선 판별
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int count = 0;
			int color;
			if (map[i][j] != empty && i < 15 && j > 3 ) {
				color = map[i][j];
				for (int k = 0; k < 5; k++) {
					if (map[i - k][j - k] == color) {
						count++;
					}
					else {
						break;
					}
				}

			}

			if (count >= 5) {
				return 1;
			}
		}
	}

	//통과못했으면 
	return 0;
	//비정상적인 종료, 아직 미구현
	
}

void finish(int result, int turn) {
	
	if (result == 10) { //실격패+컴퓨터 턴
		printf("컴퓨터가 시간을 초과하여 유저 승리!\n");
	}
	else if (result == 11) { //실격패 + 유저턴
		printf("유저가 시간을 초과하여 컴퓨터 승리!\n");
	}
	else if (result == 20) { //정상적인 게임 종료 + 컴퓨터턴
		printf("\n********************************* 컴퓨터 승리! **************************************\n");
	}
	else if (result = 21) { //정상적인 게임 종료 + 유저턴
		printf("\n********************************* 유저 승리! **************************************\n");
	}
}


move Heuristic_Alpha_Beta_Search() {
	move result;
	move_and_value temp;
	int depth = 0;  // 수정: 시작 깊이를 0으로 설정

	temp = Max_Value(INT_MIN, INT_MAX, depth); // 수정: INT_MIN과 INT_MAX를 사용하여 초기 알파 및 베타 값을 설정
	result = temp.move_;

	//test
	//printf("이용된 가중치 : %d\n", temp.value);

	return result;
}

move_and_value Max_Value(int alpha, int beta, int depth) {
	depth++;

	if (depth >= depth_limit) {
		int value = evaluate();
		move temp = { -1, -1 };
		return returnResult(temp, value, depth);
	}

	int value = INT_MIN;  // 수정: INT_MIN 사용
	move moving = { -1, -1 };

	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			if (state[i][j] == empty) {
				state[i][j] = computer_color;
				move_and_value result = Min_Value(alpha, beta, depth);
				state[i][j] = empty;

				if (result.value > value) {
					value = result.value;
					//test
					//printf("%d가 선택됨\n", value);

					moving.x = j;
					moving.y = i;

					alpha = max(alpha, value);
					if (value >= beta) {
						return returnResult(moving, value, depth);
					}
				}
			}
		}
	}

	return returnResult(moving, value, depth);
}

move_and_value Min_Value(int alpha, int beta, int depth) {
	depth++;

	if (depth >= depth_limit) {
		int value = evaluate();
		move temp = { -1, -1 };
		return returnResult(temp, value, depth);
	}

	int value = INT_MAX;  // 수정: INT_MAX 사용
	move moving = { -1, -1 };

	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			if (state[i][j] == empty) {
				state[i][j] = user_color;  //user_color 사용
				move_and_value result = Max_Value(alpha, beta, depth);
				state[i][j] = empty;

				if (result.value < value) {
					value = result.value;
					moving.x = j;
					moving.y = i;

					beta = min(beta, value);
					if (value <= alpha) {
						return returnResult(moving, value, depth);
					}
				}
			}
		}
	}

	return returnResult(moving, value, depth);
}

//리턴하는함수
move_and_value returnResult(move moving, int value, int depth) {
	move_and_value result;
	result.move_ = moving;
	result.value = value;
	result.depth = depth--;//리턴시 depth 1 감소


	return result;
}
//평가함수
int evaluate() {

	int value = 0;

	// 수평 방향 computer_color 가중치 평가
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X ; j++) {
			int computer_count = 0; // 컴퓨터 수
		
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 


			if (state[i][j] == computer_color) { //컴퓨터의 가중치 파악
				
				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == computer_color) computer_count++;
					else if (state[i][j + k] == user_color || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 유저돌) 일단 last_state증가
						last_state++;
						
						if (j + k != X - 1){ //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄
						
						j += k;  //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
						break;
					}
				}

				if (j - 1 == 0 || state[i][j-1] == user_color) { //이전블록이 막혀있다면
					last_state++;
				}
			

				//이것을 바탕으로 가중치 파악
				value += computerWeight(computer_count, last_state);
			}
			
		

		}
	}

	// 수평 방향 computer_color 가중치 평가
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X ; j++) {
			int computer_count = 0; // 컴퓨터 수
		
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 


			if (state[i][j] == computer_color) { //컴퓨터의 가중치 파악
				
				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == computer_color) computer_count++;
					else if (state[i][j + k] == user_color || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 유저돌) 일단 last_state증가
						last_state++;
						
						if (j + k != X - 1){ //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄
						
						j += k;  //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
						break;
					}
				}

				if (j - 1 == 0 || state[i][j-1] == user_color) { //이전블록이 막혀있다면
					last_state++;
				}
			

				//이것을 바탕으로 가중치 파악
				value += computerWeight(computer_count, last_state);
			}
			
		

		}
	}

	// 수직 방향 computer_color 가중치 평가
	for (int j = 0; j < X; j++) {
		for (int i = 0; i < Y; i++) {
			int computer_count = 0; // 컴퓨터 수
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 

			if (state[i][j] == computer_color) { //컴퓨터의 가중치 파악
				for (int k = 0; (k < 5) && (i + k < Y); k++) {
					if (state[i + k][j] == computer_color) computer_count++;
					else if (state[i + k][j] == user_color || (i + k == Y - 1)) { //한쪽이 막혀있으면(벽또는 유저돌) 일단 last_state증가
						last_state++;
						if (i + k != Y - 1) { //벽으로 막힌게 아니라면 [i, i + k]는 한번 탐색했으므로 패스한다
							i += k;
						}
						break;
					}
					else if (state[i + k][j] == empty) { //한쪽이 비여있으면 그냥 끝냄
						i += k;
						break;
					}
				}

				if (i - 1 == 0 || state[i - 1][j] == user_color) { //이전블록이 막혀있다면
					last_state++;
				}

				//이것을 바탕으로 가중치 파악
				value += computerWeight(computer_count, last_state);
			}
		}
	}

	// 대각선 방향 computer_color 가중치 평가 (왼쪽 위에서 오른쪽 아래 방향)
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int computer_count = 0; // 컴퓨터 수
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 

			if (state[i][j] == computer_color) { //컴퓨터의 가중치 파악
				for (int k = 0; (k < 5) && (i + k < Y) && (j + k < X); k++) {
					if (state[i + k][j + k] == computer_color) computer_count++;
					else if (state[i + k][j + k] == user_color || (i + k == Y - 1) || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 유저돌) 일단 last_state증가
						last_state++;
						if ((i + k != Y - 1) && (j + k != X - 1)) { //벽으로 막힌게 아니라면 [i, i + k], [j, j + k]는 한번 탐색했으므로 패스한다
							i += k;
							j += k;
						}
						break;
					}
					else if (state[i + k][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄
						i += k;
						j += k;
						break;
					}
				}

				if ((i - 1 == 0 || j - 1 == 0) || state[i - 1][j - 1] == user_color) { //이전블록이 막혀있다면
					last_state++;
				}

				//이것을 바탕으로 가중치 파악
				value += computerWeight(computer_count, last_state);
			}
		}
	}

	// 대각선 방향 computer_color 가중치 평가 (왼쪽 아래에서 오른쪽 위 방향)
	for (int i = Y - 1; i >= 0; i--) {
		for (int j = 0; j < X; j++) {
			int computer_count = 0; // 컴퓨터 수
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 

			if (state[i][j] == computer_color) { //컴퓨터의 가중치 파악
				for (int k = 0; (k < 5) && (i - k >= 0) && (j + k < X); k++) {
					if (state[i - k][j + k] == computer_color) computer_count++;
					else if (state[i - k][j + k] == user_color || (i - k == 0) || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 유저돌) 일단 last_state증가
						last_state++;
						if ((i - k != 0) && (j + k != X - 1)) { //벽으로 막힌게 아니라면 [i, i - k], [j, j + k]는 한번 탐색했으므로 패스한다
							i -= k;
							j += k;
						}
						break;
					}
					else if (state[i - k][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄
						i -= k;
						j += k;
						break;
					}
				}

				if ((i + 1 == Y - 1 || j - 1 == 0) || state[i + 1][j - 1] == user_color) { //이전블록이 막혀있다면
					last_state++;
				}

				//이것을 바탕으로 가중치 파악
				value += computerWeight(computer_count, last_state);
			}
		}
	}
	

	//유저 가중치 수정 버전
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // 유저 수

			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 


			if (state[i][j] == user_color) { //유저의 가중치 파악

				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == user_color) user_count++;
					else if (state[i][j + k] == computer_color || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 유저돌) 일단 last_state증가
						last_state++;

						if (j + k != X - 1) { //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄

						j += k;  //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
						break;
					}
				}

				if (j - 1 == 0 || state[i][j - 1] == computer_color) { //이전블록이 막혀있다면
					last_state++;
				}


				//이것을 바탕으로 가중치 파악
				value -= computerWeight(user_count, last_state);
			}



		}
	}

	// 수평 방향 user_color 가중치 평가
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // 유저 수

			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 


			if (state[i][j] == user_color) { //유저의 가중치 파악

				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == user_color) user_count++;
					else if (state[i][j + k] == computer_color || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 컴퓨터돌) 일단 last_state증가
						last_state++;

						if (j + k != X - 1) { //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄

						j += k;  //벽으로 막힌게 아니라면 [j, j+ k]는 한번 탐색했으므로 패스한다
						break;
					}
				}

				if (j - 1 == 0 || state[i][j - 1] == computer_color) { //이전블록이 막혀있다면
					last_state++;
				}


				//이것을 바탕으로 가중치 파악
				value -= computerWeight(user_count, last_state);
			}



		}
	}

	// 수직 방향 userr_color 가중치 평가
	for (int j = 0; j < X; j++) {
		for (int i = 0; i < Y; i++) {
			int user_count = 0; // 유저 수
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 

			if (state[i][j] == user_color) { //유저의 가중치 파악
				for (int k = 0; (k < 5) && (i + k < Y); k++) {
					if (state[i + k][j] == user_color) user_count++;
					else if (state[i + k][j] == computer_color || (i + k == Y - 1)) { //한쪽이 막혀있으면(벽또는 컴퓨터돌) 일단 last_state증가
						last_state++;
						if (i + k != Y - 1) { //벽으로 막힌게 아니라면 [i, i + k]는 한번 탐색했으므로 패스한다
							i += k;
						}
						break;
					}
					else if (state[i + k][j] == empty) { //한쪽이 비여있으면 그냥 끝냄
						i += k;
						break;
					}
				}

				if (i - 1 == 0 || state[i - 1][j] == computer_color) { //이전블록이 막혀있다면
					last_state++;
				}

				//이것을 바탕으로 가중치 파악
				value -= computerWeight(user_count, last_state);
			}
		}
	}

	// 대각선 방향 user_color 가중치 평가 (왼쪽 위에서 오른쪽 아래 방향)
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // 컴퓨터 수
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 

			if (state[i][j] == user_color) { //유저의 가중치 파악
				for (int k = 0; (k < 5) && (i + k < Y) && (j + k < X); k++) {
					if (state[i + k][j + k] == user_color) user_count++;
					else if (state[i + k][j + k] == computer_color || (i + k == Y - 1) || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 컴퓨터돌) 일단 last_state증가
						last_state++;
						if ((i + k != Y - 1) && (j + k != X - 1)) { //벽으로 막힌게 아니라면 [i, i + k], [j, j + k]는 한번 탐색했으므로 패스한다
							i += k;
							j += k;
						}
						break;
					}
					else if (state[i + k][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄
						i += k;
						j += k;
						break;
					}
				}

				if ((i - 1 == 0 || j - 1 == 0) || state[i - 1][j - 1] == computer_color) { //이전블록이 막혀있다면
					last_state++;
				}

				//이것을 바탕으로 가중치 파악
				value -= computerWeight(user_count, last_state);
			}
		}
	}

	// 대각선 방향 user_color 가중치 평가 (왼쪽 아래에서 오른쪽 위 방향)
	for (int i = Y - 1; i >= 0; i--) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // 유저 수
			int last_state = 0; //뚫려있는지 0 , 하나만 막혀있는지 1 , 양쪽다 막혀있는지(벽에 걸린것도 포함) 2 

			if (state[i][j] == user_color) { //유저의 가중치 파악
				for (int k = 0; (k < 5) && (i - k >= 0) && (j + k < X); k++) {
					if (state[i - k][j + k] == user_color) user_count++;
					else if (state[i - k][j + k] == computer_color || (i - k == 0) || (j + k == X - 1)) { //한쪽이 막혀있으면(벽또는 컴퓨터돌) 일단 last_state증가
						last_state++;
						if ((i - k != 0) && (j + k != X - 1)) { //벽으로 막힌게 아니라면 [i, i - k], [j, j + k]는 한번 탐색했으므로 패스한다
							i -= k;
							j += k;
						}
						break;
					}
					else if (state[i - k][j + k] == empty) { //한쪽이 비여있으면 그냥 끝냄
						i -= k;
						j += k;
						break;
					}
				}

				if ((i + 1 == Y - 1 || j - 1 == 0) || state[i + 1][j - 1] == computer_color) { //이전블록이 막혀있다면
					last_state++;
				}

				//이것을 바탕으로 가중치 파악
				value -= computerWeight(user_count, last_state);
			}
		}
	}


	

	//test
	//printf("%d/", value);


	return value;

}




int computerWeight(int computer_count, int last_state) {
	//test
	//printf("%d ", computer_count);
	int value = 0;

	if (last_state == 2) {
		value = 0;
	}
	else if (last_state == 1) {
		if (computer_count == 5) {
			value += 200000;
		}
		else if (computer_count == 4) {
			value += 1000;
		}
		else if (computer_count == 3) {
			value += 100;
		}
		else if (computer_count == 2) {
			value += 10;
		}
		else if (computer_count == 1) {
			value += 1;
		}

	}
	else if (last_state == 0) {
		if (computer_count == 5) {
			value += 20000;
		}
		else if (computer_count == 4) {
			value += 10000;
		}
		else if (computer_count == 3) {
			value += 1000;
		}
		else if (computer_count == 2) {
			value += 100;
		}
		else if (computer_count == 1) {
			value += 10;
		}

	}

	return value;
}

int userWeight(int user_count, int last_state) {
	//test
	if (user_count >= 3) {
		printf("유저 카운트 : %d )\n", user_count);
	}

	int value = 0;

	// 양쪽이 모두 막혀 있으면 점수를 주지 않는다
	if (last_state == 2) {
		value = 0;
	}
	// 한쪽만 막혀 있을 경우, 유저가 연속적으로 돌을 놓을 가능성이 있으므로 방어
	else if (last_state == 1) {
		if (user_count == 4) {
			value += 10000;  // 유저가 승리할 가능성이 큰 경우 높은 가중치
		}
		else if (user_count == 3) {
			value += 1000;  // 유저가 돌을 더 놓을 공간이 있을 경우 방어가 필요
		}
		else if (user_count == 2) {
			value += 10;  // 가능성이 낮지만 무시할 수 없는 경우
		}
		else if (user_count == 1) {
			value += 1;
		}
	}
	// 양쪽 모두 막혀 있지 않을 경우, 유저가 승리할 가능성이 더 높으므로 더 높은 가중치를 줌
	else if (last_state == 0) {
		if (user_count == 4) {
			value += 110000;  // 유저가 다음 수에 승리할 수 있는 매우 위험한 상황
		}
		else if (user_count == 3) {
			value += 1000;  // 유저가 승리할 가능성이 있는 상황
		}
		else if (user_count == 2) {
			value += 100;  // 유저가 게임을 유리하게 진행할 수 있는 상황
		}
		else if (user_count == 1) {
			value += 10;
		}
	}

	return value;
}

