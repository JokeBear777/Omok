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
#define depth_limit 3 //�ð��������� ���� ���̴� 3�� ����

#define DEFENSE_BLOCK_ 10 //�Ѱ��� ���� ��� ������ ����ġ


int map[Y][X]; //�ٵ��� ����, 0�� �������, 1�� �� 2�� ��
int state[Y][X]; //��ġ�ϸ鼭 �̿��ϴ� �ӽ� map
int user_color; //������ ������ ������
int computer_color; //������ ������ ������

typedef struct move_ {  
	int x; //x��
	int y; //y��

}move;

typedef struct move_and_value_ { //move�� int(value)�� , depth 3���� ���ÿ� return�ϱ� ���� ����Ѵ�
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
int evaluate();//���Լ�
move Heuristic_Alpha_Beta_Search();
move_and_value Max_Value(int, int, int);
move_and_value Min_Value(int, int, int); 
move_and_value returnResult(move, int, int); //2���� ���� ��ġ�� depth 1���� ��Ų�� ����
//int evaluateWeight(int, int); //����ġ ��
void finish(int, int);
int computerWeight(int, int);
int userWeight(int, int);



int main(void) {
	
	game_run();

	return 0;
}


//gui �̰� �������� ������
void rendering() {

}

void game_run() {
	int turn; 
	int result; //������ ��� ��������,

	create_map();// �� ����
	turn = game_init(); //���� ����, ���� �������������� ���� ���� ��������
	result = game_progress(turn); //init���� ������ turn�� �������� ���� ����
	finish(result, turn);
 
};

void create_map() {
	//map����
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			map[i][j] = empty; 
		}
	}

	//state ����
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			state[i][j] = empty; 
		}
	}

}

//���� ����, �� ���� ����, ���� ������ ���ϰ�, �ʿ信 ���������� ��ǲ�� �޴´�
int game_init() {
	int user;
	int turn;
	srand(time(NULL)); // �Ź� �ٸ� �õ尪 ����

	printf("* ���ѽð��� %d�� �Դϴ�.\n", 15);
	printf("* ���� ���� �����մϴ�.\n");
	printf("* �Է��� ������ ���鸸 �����մϴ� ex)��ǥ �Է��Ϸ��� %d %d �̷� �������� �Է�\n", 8, 8);
	printf("* ���� �����Ϸ��� %d ���� �����Ϸ��� %d�� �Է��ϼ���\n", 1, 2);
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	

	do {
		scanf("%d", &user);
		if (user == black) {
			printf("�� ����!\n");
			user_color = black;
			computer_color = white;
			turn = user_turn;
		}
		else if (user == white) {
			printf("�� ����!\n");
			user_color = white;
			computer_color = black;
			turn = computer_turn;
		}
		else { //�Է½� ���
			printf("%d�� %d�� �Է��ϼ���\n", 1, 2);
		}
	} while ((user != 1) && (user != 2));


	//��Ȱ�� ���� ������ ���� ó�� 1,2���� �ٸ� �˰������� �д�
	if (turn == computer_turn) { //��ǻ�Ͱ� ù ���� �δ� ��� �߾ӿ� �������� ��
	
		int randomX = 0;
		int randomY = 0;

		randomX = rand() % 5 + 5;
		randomY = rand() % 5 + 5;

		//���
		printf("��ǻ���� �� (%d , %d)", randomX, randomY);

		map[randomY][randomX] = computer_color;
		state[randomY][randomX] = computer_color;
		turn = user_turn;
	}
	else { //������ ù���� �δ� ���, ��ǻ���� �ι�°���� ù���� �������� �������� ��
		user_input(); //ù��° �Է��� �޴´�
		
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

		//�ִ� 2ĭ���� ���������� �Ѵ�,
		while (x + plus_x > 18 || x + plus_x < 0 || y + plus_y > 18 || y + plus_y < 0 || (plus_x == 0 && plus_y == 0)) { 
			plus_x = (rand() % 5) - 2;
			plus_y = (rand() % 5) - 2;
		}

		map[y + plus_y][x + plus_x] = computer_color;
		state[y + plus_y][x + plus_x] = computer_color;

		//���
		printf("��ǻ���� �� (%d , %d)", x + plus_x, y + plus_y);





	}

	return turn; //�� ����

}

//����ڷκ��� ��ġ�� �Է¹ް� map����
void user_input() {
	int x, y;

	printf("�� ���� ��ǥ �Է� (X�� ��ǥ Y�� ��ǥ)\n"); 
	
	//�Է¹��� �� ��ȿ�� �˻�
	do {
		scanf("%d %d", &x, &y);

		
		if (x < 0 || x > 20 || y < 0 || y > 20) {
			printf("��ǥ���� ������ �ʰ��߽��ϴ�\n");
		}

		if (map[y][x] != empty) {
			printf("�̹� ������ �ڸ��Դϴ�\n");
		}


	} while (x < 0 || x > 20 || y < 0 || y > 20 || map[y][x] != 0); //�ϳ��� ������ϸ� �ٽ� �Է¹޴´�

	//��ȿ�� ����� map����
	map[y][x] = user_color;
	state[y][x] = user_color;
}

//��ǻ�ͷκ��� ��ġ�� �Է¹ް� �� ����

void computer_input() {
	move result;
	result = Heuristic_Alpha_Beta_Search();
	
	//�˰��� ����� ���� ����� ���򺯰�
	printf("��ǻ���� �� (%d , %d)", result.x, result.y);
	map[result.y][result.x] = computer_color;
	state[result.y][result.x] = computer_color;
}



//���� ����
int game_progress(int turn) {
	time_t start, finish;

	while (1) { //finish�Ҷ����� ���� �ݺ�
		start = time(NULL);
		double duration;

		//�� ����
		if (turn == computer_turn) { //��ǻ�� ���̸� �޸���ƽ ���ĺ�Ÿ �˰��� ����
			computer_input();
			//turn = user_turn;
		}
		else if (turn == user_turn) { //���� ���̸� �Է��� �޴´�
			user_input();
			//turn = computer_turn;
		}

		finish = time(NULL);
		duration = (double)(finish - start);


		//finish���� 
		if (duration > 15) { //�ð� �ʰ��Ǹ� �ǰ���
			if (turn == computer_turn) {
				return 10;
			}
			else {
				return 11;
			}

			
		}

		if (isFinish()) { //������ �ϼ��Ǹ� ����
			if (turn == computer_turn) {
				return 20;
			}
			else {
				return 21;
			}
		}

	
		//finish���� ���� �ʾҴٸ� �� ����Ī
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
	//���� �Ǻ�
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
	//���� �Ǻ�
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

	//������ �밢�� �Ǻ�
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

	//���� �밢�� �Ǻ�
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

	//����������� 
	return 0;
	//���������� ����, ���� �̱���
	
}

void finish(int result, int turn) {
	
	if (result == 10) { //�ǰ���+��ǻ�� ��
		printf("��ǻ�Ͱ� �ð��� �ʰ��Ͽ� ���� �¸�!\n");
	}
	else if (result == 11) { //�ǰ��� + ������
		printf("������ �ð��� �ʰ��Ͽ� ��ǻ�� �¸�!\n");
	}
	else if (result == 20) { //�������� ���� ���� + ��ǻ����
		printf("\n********************************* ��ǻ�� �¸�! **************************************\n");
	}
	else if (result = 21) { //�������� ���� ���� + ������
		printf("\n********************************* ���� �¸�! **************************************\n");
	}
}


move Heuristic_Alpha_Beta_Search() {
	move result;
	move_and_value temp;
	int depth = 0;  // ����: ���� ���̸� 0���� ����

	temp = Max_Value(INT_MIN, INT_MAX, depth); // ����: INT_MIN�� INT_MAX�� ����Ͽ� �ʱ� ���� �� ��Ÿ ���� ����
	result = temp.move_;

	//test
	//printf("�̿�� ����ġ : %d\n", temp.value);

	return result;
}

move_and_value Max_Value(int alpha, int beta, int depth) {
	depth++;

	if (depth >= depth_limit) {
		int value = evaluate();
		move temp = { -1, -1 };
		return returnResult(temp, value, depth);
	}

	int value = INT_MIN;  // ����: INT_MIN ���
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
					//printf("%d�� ���õ�\n", value);

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

	int value = INT_MAX;  // ����: INT_MAX ���
	move moving = { -1, -1 };

	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			if (state[i][j] == empty) {
				state[i][j] = user_color;  //user_color ���
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

//�����ϴ��Լ�
move_and_value returnResult(move moving, int value, int depth) {
	move_and_value result;
	result.move_ = moving;
	result.value = value;
	result.depth = depth--;//���Ͻ� depth 1 ����


	return result;
}
//���Լ�
int evaluate() {

	int value = 0;

	// ���� ���� computer_color ����ġ ��
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X ; j++) {
			int computer_count = 0; // ��ǻ�� ��
		
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 


			if (state[i][j] == computer_color) { //��ǻ���� ����ġ �ľ�
				
				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == computer_color) computer_count++;
					else if (state[i][j + k] == user_color || (j + k == X - 1)) { //������ ����������(���Ǵ� ������) �ϴ� last_state����
						last_state++;
						
						if (j + k != X - 1){ //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //������ �������� �׳� ����
						
						j += k;  //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
						break;
					}
				}

				if (j - 1 == 0 || state[i][j-1] == user_color) { //��������� �����ִٸ�
					last_state++;
				}
			

				//�̰��� �������� ����ġ �ľ�
				value += computerWeight(computer_count, last_state);
			}
			
		

		}
	}

	// ���� ���� computer_color ����ġ ��
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X ; j++) {
			int computer_count = 0; // ��ǻ�� ��
		
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 


			if (state[i][j] == computer_color) { //��ǻ���� ����ġ �ľ�
				
				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == computer_color) computer_count++;
					else if (state[i][j + k] == user_color || (j + k == X - 1)) { //������ ����������(���Ǵ� ������) �ϴ� last_state����
						last_state++;
						
						if (j + k != X - 1){ //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //������ �������� �׳� ����
						
						j += k;  //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
						break;
					}
				}

				if (j - 1 == 0 || state[i][j-1] == user_color) { //��������� �����ִٸ�
					last_state++;
				}
			

				//�̰��� �������� ����ġ �ľ�
				value += computerWeight(computer_count, last_state);
			}
			
		

		}
	}

	// ���� ���� computer_color ����ġ ��
	for (int j = 0; j < X; j++) {
		for (int i = 0; i < Y; i++) {
			int computer_count = 0; // ��ǻ�� ��
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 

			if (state[i][j] == computer_color) { //��ǻ���� ����ġ �ľ�
				for (int k = 0; (k < 5) && (i + k < Y); k++) {
					if (state[i + k][j] == computer_color) computer_count++;
					else if (state[i + k][j] == user_color || (i + k == Y - 1)) { //������ ����������(���Ǵ� ������) �ϴ� last_state����
						last_state++;
						if (i + k != Y - 1) { //������ ������ �ƴ϶�� [i, i + k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							i += k;
						}
						break;
					}
					else if (state[i + k][j] == empty) { //������ �������� �׳� ����
						i += k;
						break;
					}
				}

				if (i - 1 == 0 || state[i - 1][j] == user_color) { //��������� �����ִٸ�
					last_state++;
				}

				//�̰��� �������� ����ġ �ľ�
				value += computerWeight(computer_count, last_state);
			}
		}
	}

	// �밢�� ���� computer_color ����ġ �� (���� ������ ������ �Ʒ� ����)
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int computer_count = 0; // ��ǻ�� ��
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 

			if (state[i][j] == computer_color) { //��ǻ���� ����ġ �ľ�
				for (int k = 0; (k < 5) && (i + k < Y) && (j + k < X); k++) {
					if (state[i + k][j + k] == computer_color) computer_count++;
					else if (state[i + k][j + k] == user_color || (i + k == Y - 1) || (j + k == X - 1)) { //������ ����������(���Ǵ� ������) �ϴ� last_state����
						last_state++;
						if ((i + k != Y - 1) && (j + k != X - 1)) { //������ ������ �ƴ϶�� [i, i + k], [j, j + k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							i += k;
							j += k;
						}
						break;
					}
					else if (state[i + k][j + k] == empty) { //������ �������� �׳� ����
						i += k;
						j += k;
						break;
					}
				}

				if ((i - 1 == 0 || j - 1 == 0) || state[i - 1][j - 1] == user_color) { //��������� �����ִٸ�
					last_state++;
				}

				//�̰��� �������� ����ġ �ľ�
				value += computerWeight(computer_count, last_state);
			}
		}
	}

	// �밢�� ���� computer_color ����ġ �� (���� �Ʒ����� ������ �� ����)
	for (int i = Y - 1; i >= 0; i--) {
		for (int j = 0; j < X; j++) {
			int computer_count = 0; // ��ǻ�� ��
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 

			if (state[i][j] == computer_color) { //��ǻ���� ����ġ �ľ�
				for (int k = 0; (k < 5) && (i - k >= 0) && (j + k < X); k++) {
					if (state[i - k][j + k] == computer_color) computer_count++;
					else if (state[i - k][j + k] == user_color || (i - k == 0) || (j + k == X - 1)) { //������ ����������(���Ǵ� ������) �ϴ� last_state����
						last_state++;
						if ((i - k != 0) && (j + k != X - 1)) { //������ ������ �ƴ϶�� [i, i - k], [j, j + k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							i -= k;
							j += k;
						}
						break;
					}
					else if (state[i - k][j + k] == empty) { //������ �������� �׳� ����
						i -= k;
						j += k;
						break;
					}
				}

				if ((i + 1 == Y - 1 || j - 1 == 0) || state[i + 1][j - 1] == user_color) { //��������� �����ִٸ�
					last_state++;
				}

				//�̰��� �������� ����ġ �ľ�
				value += computerWeight(computer_count, last_state);
			}
		}
	}
	

	//���� ����ġ ���� ����
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // ���� ��

			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 


			if (state[i][j] == user_color) { //������ ����ġ �ľ�

				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == user_color) user_count++;
					else if (state[i][j + k] == computer_color || (j + k == X - 1)) { //������ ����������(���Ǵ� ������) �ϴ� last_state����
						last_state++;

						if (j + k != X - 1) { //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //������ �������� �׳� ����

						j += k;  //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
						break;
					}
				}

				if (j - 1 == 0 || state[i][j - 1] == computer_color) { //��������� �����ִٸ�
					last_state++;
				}


				//�̰��� �������� ����ġ �ľ�
				value -= computerWeight(user_count, last_state);
			}



		}
	}

	// ���� ���� user_color ����ġ ��
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // ���� ��

			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 


			if (state[i][j] == user_color) { //������ ����ġ �ľ�

				for (int k = 0; (k < 5) && (j + k < X); k++) {

					if (state[i][j + k] == user_color) user_count++;
					else if (state[i][j + k] == computer_color || (j + k == X - 1)) { //������ ����������(���Ǵ� ��ǻ�͵�) �ϴ� last_state����
						last_state++;

						if (j + k != X - 1) { //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							j += k;
						}
						break;
					}
					else if (state[i][j + k] == empty) { //������ �������� �׳� ����

						j += k;  //������ ������ �ƴ϶�� [j, j+ k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
						break;
					}
				}

				if (j - 1 == 0 || state[i][j - 1] == computer_color) { //��������� �����ִٸ�
					last_state++;
				}


				//�̰��� �������� ����ġ �ľ�
				value -= computerWeight(user_count, last_state);
			}



		}
	}

	// ���� ���� userr_color ����ġ ��
	for (int j = 0; j < X; j++) {
		for (int i = 0; i < Y; i++) {
			int user_count = 0; // ���� ��
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 

			if (state[i][j] == user_color) { //������ ����ġ �ľ�
				for (int k = 0; (k < 5) && (i + k < Y); k++) {
					if (state[i + k][j] == user_color) user_count++;
					else if (state[i + k][j] == computer_color || (i + k == Y - 1)) { //������ ����������(���Ǵ� ��ǻ�͵�) �ϴ� last_state����
						last_state++;
						if (i + k != Y - 1) { //������ ������ �ƴ϶�� [i, i + k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							i += k;
						}
						break;
					}
					else if (state[i + k][j] == empty) { //������ �������� �׳� ����
						i += k;
						break;
					}
				}

				if (i - 1 == 0 || state[i - 1][j] == computer_color) { //��������� �����ִٸ�
					last_state++;
				}

				//�̰��� �������� ����ġ �ľ�
				value -= computerWeight(user_count, last_state);
			}
		}
	}

	// �밢�� ���� user_color ����ġ �� (���� ������ ������ �Ʒ� ����)
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // ��ǻ�� ��
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 

			if (state[i][j] == user_color) { //������ ����ġ �ľ�
				for (int k = 0; (k < 5) && (i + k < Y) && (j + k < X); k++) {
					if (state[i + k][j + k] == user_color) user_count++;
					else if (state[i + k][j + k] == computer_color || (i + k == Y - 1) || (j + k == X - 1)) { //������ ����������(���Ǵ� ��ǻ�͵�) �ϴ� last_state����
						last_state++;
						if ((i + k != Y - 1) && (j + k != X - 1)) { //������ ������ �ƴ϶�� [i, i + k], [j, j + k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							i += k;
							j += k;
						}
						break;
					}
					else if (state[i + k][j + k] == empty) { //������ �������� �׳� ����
						i += k;
						j += k;
						break;
					}
				}

				if ((i - 1 == 0 || j - 1 == 0) || state[i - 1][j - 1] == computer_color) { //��������� �����ִٸ�
					last_state++;
				}

				//�̰��� �������� ����ġ �ľ�
				value -= computerWeight(user_count, last_state);
			}
		}
	}

	// �밢�� ���� user_color ����ġ �� (���� �Ʒ����� ������ �� ����)
	for (int i = Y - 1; i >= 0; i--) {
		for (int j = 0; j < X; j++) {
			int user_count = 0; // ���� ��
			int last_state = 0; //�շ��ִ��� 0 , �ϳ��� �����ִ��� 1 , ���ʴ� �����ִ���(���� �ɸ��͵� ����) 2 

			if (state[i][j] == user_color) { //������ ����ġ �ľ�
				for (int k = 0; (k < 5) && (i - k >= 0) && (j + k < X); k++) {
					if (state[i - k][j + k] == user_color) user_count++;
					else if (state[i - k][j + k] == computer_color || (i - k == 0) || (j + k == X - 1)) { //������ ����������(���Ǵ� ��ǻ�͵�) �ϴ� last_state����
						last_state++;
						if ((i - k != 0) && (j + k != X - 1)) { //������ ������ �ƴ϶�� [i, i - k], [j, j + k]�� �ѹ� Ž�������Ƿ� �н��Ѵ�
							i -= k;
							j += k;
						}
						break;
					}
					else if (state[i - k][j + k] == empty) { //������ �������� �׳� ����
						i -= k;
						j += k;
						break;
					}
				}

				if ((i + 1 == Y - 1 || j - 1 == 0) || state[i + 1][j - 1] == computer_color) { //��������� �����ִٸ�
					last_state++;
				}

				//�̰��� �������� ����ġ �ľ�
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
		printf("���� ī��Ʈ : %d )\n", user_count);
	}

	int value = 0;

	// ������ ��� ���� ������ ������ ���� �ʴ´�
	if (last_state == 2) {
		value = 0;
	}
	// ���ʸ� ���� ���� ���, ������ ���������� ���� ���� ���ɼ��� �����Ƿ� ���
	else if (last_state == 1) {
		if (user_count == 4) {
			value += 10000;  // ������ �¸��� ���ɼ��� ū ��� ���� ����ġ
		}
		else if (user_count == 3) {
			value += 1000;  // ������ ���� �� ���� ������ ���� ��� �� �ʿ�
		}
		else if (user_count == 2) {
			value += 10;  // ���ɼ��� ������ ������ �� ���� ���
		}
		else if (user_count == 1) {
			value += 1;
		}
	}
	// ���� ��� ���� ���� ���� ���, ������ �¸��� ���ɼ��� �� �����Ƿ� �� ���� ����ġ�� ��
	else if (last_state == 0) {
		if (user_count == 4) {
			value += 110000;  // ������ ���� ���� �¸��� �� �ִ� �ſ� ������ ��Ȳ
		}
		else if (user_count == 3) {
			value += 1000;  // ������ �¸��� ���ɼ��� �ִ� ��Ȳ
		}
		else if (user_count == 2) {
			value += 100;  // ������ ������ �����ϰ� ������ �� �ִ� ��Ȳ
		}
		else if (user_count == 1) {
			value += 10;
		}
	}

	return value;
}

