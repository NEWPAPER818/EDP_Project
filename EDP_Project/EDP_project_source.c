#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//function
void create_bluezone(int, int*);
void calc_bluezone(int[][7], int, int*, int*, int);
void calc_bluezone2(int [][7], int, int);
void print_bluezone(int [][7], int, int);
int in_safezone(int[][7], int);
int calc_valid_loca(int*, int*, int);
int create_rand_item(int*);
void print_rand_item(int *, int*, int);
int check_valid_item(int, int, int*, int);
void print_player_item_list(int[12]);
void print_hp(int);
int rand_meet_enemy(int);
void print_player_weapon(int [12]);
int check_valid_weapon(int[12], int);
int calc_enemy_fight(int, int, int);
int have_healing_item(int[12], int);
int print_healing_item_list(int[12]);



//main
int main() {
	//
	//srand(time(NULL));

	//
	printf("2021-2 Project\n19010925, ÀÌ¼¼Á¾ Sejong Lee, Computer Engineering\n");
	printf("--------------------------------------------------------\n");
	printf("PPPPP   U    U  BBBBB    GGGG   \n");
	printf("P    P  U    U  B    B  G       \n");
	printf("PPPPP   U    U  BBBBB   G  GGG  \n");
	printf("P       U    U  B    B  G    G  \n");
	printf("P        UUUU   BBBBB    GGGG   \n");

	//
	int hp = 100;						//0 ~ 100 ~
	int loca = 1;
	int diff = 1;						//1 ~ 10
	char item_list[13][21] = { "P1911 (Pistol)", "AKM (AR)", "M24 (SR)", "Crossbow",
									"Frying Pan", "Grenade", "C4",
									"bandage", "First Aid Kit",
									"Police Vest", "Helmet",
									"Car",
									"Punch"};
	char* p_item_list = &item_list[0];
	int player_item_list[12] = { 0 };
	//int player_item_cnt = 0;
	/*
	item list

	0 Guns
		0 P1911 (pistol)
		1 AKM (AR)
		2 M24 (SR)
		3 Crossbow

	1 Throwables
		4 Frying pan
		5 Grenade
		6 C4

	2 Healing items
		7 bandage healing hp +20
		8 First Aid kit healing hp to 100

	3 Equipment
		9 Police vest increase 20 hp
		10 helmet increase 30 hp

	4 Car
		11 Car can move 2 space
	*/

	int* have_car = &player_item_list[11];

	int bluezone[5] = { 1,0,0,0,0 };	//[phase_num, 1p_rand_num, 2p_rand_num, ...]
	int* p_bluezone = &bluezone;
	int* p_phase_num = &bluezone;
	int map[7][7] = { {0} };			//-1: blue zone / 0: will be bluezone / 1: safe zone / 2: player location

	int restart = 1;					//1: retry  0: exit

	//
	while (restart == 1) {
		//start
		printf("\nPress ENTER to start game!");
		getchar();


		//choose difficulty
		printf("\n==================================================================\n");
		printf("Choose difficulty level\n");
		printf("1: EASY ~ 10: HARD\n-> ");
		scanf("%d", &diff);

		while (diff < 1 || diff > 10) {
			printf("Wrong number! Choose difficulty level 1(EASY) to 10(HARD)\n-> ");
			scanf("%d", &diff);
		}
		printf("==================================================================\n");

		//print airplane
		{printf("\n");
		printf("                 ----------------------------------------            \n");
		printf("             /------                                                     \n");
		printf("          /        |     O   O   O   O   O   0   0   0                    \n");
		printf("        /-----------                                            \n");
		printf("       |    |                    l             l              \n");
		printf("         ---------------------------l           l----------       \n");
		printf("                                       l         l                \n"); }

		//choose where to fall
		{
			printf("\n");
			printf("---------MAP---------\n");
			printf("|  1  2  3  4  5  6 |\n");
			printf("|  7  8  9 10 11 12 |\n");
			printf("| 13 14 15 16 17 18 |\n");
			printf("| 19 20 21 22 23 24 |\n");
			printf("| 25 26 27 28 29 30 |\n");
			printf("| 31 32 33 34 35 36 |\n");
			printf("---------------------\n");
			printf("You are in the airplane! Where do you want to fall?\n");
			printf("Choose 1 ~ 36\n-> ");
			scanf("%d", &loca);

			while (loca < 1 || loca > 36) {
				printf("Wrong number! Choose the other number 1 to 36\n-> ");
				scanf("%d", &loca);
			}
		}

		//
		int last_bluezone_point[2] = { 0 };

		while (*p_phase_num <= 4) {
			//create blue zone
			create_bluezone(*p_phase_num, p_bluezone + *p_phase_num);
			calc_bluezone(map, *p_phase_num, p_bluezone, last_bluezone_point, loca);
			
			printf("\n==============================================================\n");
			printf("%d Phase / Blue zone will coming!\n", bluezone[0]);
			print_bluezone(map, loca, in_safezone(map, *p_phase_num));
			
			//move? farming? Nothing?
			{
				printf("\nWill you move or farming item? Choose one\n");
				printf("1: Move / 2: Farming item / 3: Nothing\n-> ");
				int tmp = 0;
				scanf("%d", &tmp);
				getchar();

				while (tmp < 1 || tmp > 3) {
					printf("Wrong number! Choose the correct number\n1: Move / 2: Farming item / 3: Nothing\n-> ");
					scanf("%d", &tmp);
				}

				//move
				if (tmp == 1) {	
					//if have car - can move 2 space
					//else - can move 1 space
					printf("\nYou can move %d sapce from %d\nChoose the number you want to move\n-> ", *have_car>=1 ? 2 : 1, loca);
					int loca_tmp = 0;
					scanf("%d", &loca_tmp);
					getchar();

					int p_loca = &loca;

					int check1 = 1, check2 = 1, check3 = 1;

					while (1){
						//check 1-36
						if (loca_tmp < 1 || loca_tmp > 36) {
							check1 = 0;
							printf("Wrong number! Choose the other number 1 to 36\n");
						}
						else
							check1 = 1;

						//check same num
						if (loca_tmp == loca) {
							check2 = 0;
							printf("Same location! Choose the other number\n");
						}
						else
							check2 = 1;

						//check valid num
						if (calc_valid_loca(have_car, p_loca, loca_tmp) == 0) {
							check3 = 0;
							if (check1 == 1 && check2 == 1)
								printf("Wrong number! Choose the other number. You can move %d space from %d\n",
									*have_car >= 1 ? 2 : 1, loca);
						}
						else
							check3 = 1;

						if (check1 + check2 + check3 != 3) {
							printf("-> ");
							scanf("%d", &loca_tmp);
							getchar();
						}
						else
							break;
					}
				}

				//farming item
				else if(tmp == 2){			
					printf("Press ENTER to farming item!");
					getchar();
					printf("\nYou found some item! Choose two(Numbers)!\n");

					int rand_item[5] = { -1, -1, -1, -1, -1};
					int* p_rand_item = rand_item;
					int item_cnt = create_rand_item(p_rand_item);

					print_rand_item(p_item_list, p_rand_item, item_cnt);
					printf("-> ");

					int tmp1 = 0, tmp2 = 0;
					scanf("%d %d", &tmp1, &tmp2);
					getchar();
					int check = check_valid_item(tmp1, tmp2, p_rand_item, item_cnt);

					while (check == 0) {
						printf("\nWrong numbers! Choose the valid two numbers!\n-> ");
						scanf("%d %d", &tmp1, &tmp2);
						getchar();
						check = check_valid_item(tmp1, tmp2, p_rand_item, item_cnt);
					}

				
					if (0 <= tmp2 && tmp2 <= 3) {
						int tmp_num = tmp2;
						tmp2 = tmp1;
						tmp1 = tmp_num;
					}

					//save item
					if (0 <= tmp1 && tmp1 <= 3) {
						for (int i = 0; i < 3; i++)
							player_item_list[i] = 0;
						player_item_list[tmp1]++;
						player_item_list[tmp2]++;
					}
					else {
						player_item_list[tmp1]++;
						player_item_list[tmp2]++;
					}

					//if police vest, helmet increase hp
					if (tmp1 == 9) hp += 20;
					if (tmp2 == 9) hp += 20;
					if (tmp1 == 10) hp += 30;
					if (tmp2 == 10) hp += 30;

					if (tmp1 == 9 || tmp1 == 10 || tmp2 == 9 || tmp2 == 10) {
						printf("\n");
						print_hp(hp);
					}

					printf("\n");
					print_player_item_list(player_item_list);

					
				}

				//Nothing
				else {
					printf("You choose Nothing! Press ENTER to go to next step!");
					getchar();
				}
			}

			//meet enemy
			if (rand_meet_enemy(diff) == 1) {
				printf("\n--------------------------------------------------------------\n");
				print_hp(hp);
				printf("You meet enemy! You want to fight? or run? Choose one\n1: Fight / 2: Run\n-> ");
				int tmp = 0;
				scanf("%d", &tmp);
				getchar();

				while (tmp < 1 || tmp > 2) {
					printf("Wrong number! Choose the correct number\n1: Fight / 2: Run\n-> ");
					scanf("%d", &tmp);
					getchar();
				}

				//Fight
				if (tmp == 1) {
					printf("\nWhich weapon will you use? Choose one\n");
					print_player_weapon(player_item_list);
					printf("-> ");

					int tmp_num = 0;
					scanf("%d", &tmp_num);
					getchar();

					while (check_valid_weapon(player_item_list, tmp_num) == 0) {
						printf("Wrong number! Choose the correct number of your weapon list\n-> ");
						scanf("%d", &tmp_num);
						getchar();
					}

					printf("\nYou use %s!   Fighting.....\n", item_list[tmp_num]);
					player_item_list[tmp_num]--;
					printf("Press ENTER to see the result!\n");
					getchar();

					hp = calc_enemy_fight(diff, tmp_num, hp);

					if (hp > 0) {
						printf("You survived!\n");
						print_hp(hp);
						print_player_item_list(player_item_list);
						printf("\n");
					}
					else {
						printf("You died! Game Over!\n\n");
						printf(" L      OOO    SSSS   EEEEE  \n");
						printf(" L     O   O  S       E      \n");
						printf(" L     O   O   SSS    EEEEE  \n");
						printf(" L     O   O      S   E      \n");
						printf(" LLLLL  OOO   SSSS    EEEEE  \n");
						break;
					}
				}

				//Run
				else {
					printf("\nRunning.....\n");
					printf("Press ENTER to see the result!\n");
					getchar();

					hp -= diff * 2;

					if (hp > 0) {
						printf("You survived!\n");
						print_hp(hp);
					}
					else {
						printf("You died! Game Over!\n\n");
						printf(" L      OOO    SSSS   EEEEE  \n");
						printf(" L     O   O  S       E      \n");
						printf(" L     O   O   SSS    EEEEE  \n");
						printf(" L     O   O      S   E      \n");
						printf(" LLLLL  OOO   SSSS    EEEEE  \n");
						break;
					}
				}

			}

			//healing?
			while (have_healing_item(player_item_list, hp) == 1) {
				printf("Do you want to healing?\n1: Yes / 2: No\n-> ");
				int tmp = 0;
				scanf("%d", &tmp);
				getchar();
				//printf("\n");

				while (tmp < 1 || tmp > 2) {
					printf("Wrong number! Choose the correct number\n1: Yes / 2: No\n-> ");
					scanf("%d", &tmp);
					getchar();
				}

				if (tmp == 1) {
					int num = print_healing_item_list(player_item_list);
					printf("Choose healing item that you want to use(1 or 2)\n-> ");
					int h_tmp = 0;
					scanf("%d", &h_tmp);
					getchar();

					while (num == 0 && h_tmp != 1) {
						printf("Wrong number! Choose the correct number\nYou can choose only 1\n-> ");
						scanf("%d", &h_tmp);
						getchar();
					}
					while (num == 1 && h_tmp != 2) {
						printf("Wrong number! Choose the correct number\nYou can choose only 2\n-> ");
						scanf("%d", &h_tmp);
						getchar();
					}
					while (num == 2 && (h_tmp < 1 || h_tmp > 2)) {
						printf("Wrong number! Choose the correct number\n-> ");
						scanf("%d", &h_tmp);
						getchar();
					}

					if (h_tmp == 1) {
						printf("\nYou use bandage! recover HP +20 (up to 100)\n");
						hp += 20;
						if (hp > 100)
							hp = 100;
						player_item_list[7]--;
						print_hp(hp);
					}
					else {
						printf("\nYou use First Aid Kit! recover HP to 100\n");
						hp = 100;
						player_item_list[8]--;
						print_hp(hp);
					}
				}
				else
					break;
			}


			//blue zone is moving
			calc_bluezone2(map, *p_phase_num, loca);

			//blue zone moved
			//if in blue zone - damage
			printf("\n--------------------------------------------------------------\n");
			printf("%d Phase Ended / Blue zone was moved!\n", bluezone[0]);
			print_bluezone(map, loca, in_safezone(map, *p_phase_num));

			if (in_safezone(map, *p_phase_num) != 1) {
				hp -= *p_phase_num * 2 * diff;
				if (hp < 0)
					hp = 0;
				printf("\nYou are in Blue zone! You got damaged!\n");

				if (hp > 0) {
					print_hp(hp);
				}
				else {
					printf("You died! Game Over!\n\n");
					printf(" L      OOO    SSSS   EEEEE  \n");
					printf(" L     O   O  S       E      \n");
					printf(" L     O   O   SSS    EEEEE  \n");
					printf(" L     O   O      S   E      \n");
					printf(" LLLLL  OOO   SSSS    EEEEE  \n");
					break;
				}
			}

			bluezone[0]++;	//bluezone_phase++

			//
			//getchar();
			printf("\nPress ENTER to go to next phase!");
			getchar();
		}
		
		if (bluezone[0] == 5 && hp > 0) {
			printf("\n>>>>> Last Fight! <<<<<\n");
			printf("Which weapon will you use? Choose one\n");
			print_player_weapon(player_item_list);
			printf("-> ");

			int tmp_num = 0;
			scanf("%d", &tmp_num);
			getchar();

			while (check_valid_weapon(player_item_list, tmp_num) == 0) {
				printf("Wrong number! Choose the correct number of your weapon list\n-> ");
				scanf("%d", &tmp_num);
				getchar();
			}

			printf("You use %s!   Fighting.....\n", item_list[tmp_num]);
			player_item_list[tmp_num]--;
			printf("Press ENTER to see the result!\n");
			getchar();

			hp = calc_enemy_fight(diff, tmp_num, hp);

			if (hp > 0) {
				printf("You survived! You win this game!\n\n");
				printf(" W   W   W    I    N   N    \n");
				printf(" W   W   W    I    NN  N    \n");
				printf(" W   W   W    I    N N N    \n");
				printf("  W W W W     I    N  NN    \n");
				printf("   W   W      I    N   N    \n");
				

			}
			else {
				printf("You died! Game Over!\n\n");
				printf(" L      OOO    SSSS   EEEEE  \n");
				printf(" L     O   O  S       E      \n");
				printf(" L     O   O   SSS    EEEEE  \n");
				printf(" L     O   O      S   E      \n");
				printf(" LLLLL  OOO   SSSS    EEEEE  \n");
			}
		}

		//retry?
		printf("\n==============================================================\n");
		printf("Game ended. Do you want to retry?\n1: RETRY / 0: EXIT\n-> ");
		scanf("%d", &restart);
		getchar();

		while (restart < 0 || restart > 1) {
			printf("Wrong number! Choose the correct number\n1: RETRY / 0: EXIT\n-> ");
			scanf("%d", &restart);
			getchar();
		}

		if (restart == 1) {
			//initialization
			for (int i = 1; i <= 6; i++)
				for (int j = 1; j <= 6; j++)
					map[i][j] = 0;

			*p_phase_num = 1;

			last_bluezone_point[0] = 0;
			last_bluezone_point[1] = 0;

			hp = 100;

			for (int i = 0; i < 12; i++)
				player_item_list[i] = 0;
		}
		else
			printf("EXIT...\n");
	}




	return 0;
}

//function
void create_bluezone(int phase_num, int *p_bluezone) {
	int max = 0;

	if (phase_num == 1)
		max = 9;
	else
		max = 4;

	srand(time(NULL));
	int rand_num = rand() % max  + 1;
	*p_bluezone = rand_num;
}

void calc_bluezone(int map[][7], int phase_num, int* bluezone_rand, int* last_bluezone_point, int loca) {
	//phase_num == 1; 4x4 rand: 1~9
	//phase_num == 2; 3x3		1~4
	//phase_num == 3; 2x2		1~4
	//phase_num == 4; 1x1		1~4

	//calc
	int bluezone_len = 5 - phase_num;
	int div_num = 0;

	if (phase_num == 1) div_num = 3;
	else div_num = 2;

	int ii = bluezone_rand[phase_num] / div_num + 1;
	if (bluezone_rand[phase_num] % div_num == 0)
		ii--;
	last_bluezone_point[0] += ii;

	int jj = bluezone_rand[phase_num] % div_num;
	if (jj == 0)
		jj = div_num;
	last_bluezone_point[1] += jj;

	if (phase_num != 1) {
		last_bluezone_point[0]--;
		last_bluezone_point[1]--;
	}

	//save 0 will be bluezone
	if (phase_num != 1) {
		for (int i = 1; i <= 6; i++) {
			for (int j = 1; j <= 6; j++) {
				if (map[i][j] == 1 || map[i][j] == 2)
					map[i][j] = 0;
			}
		}
	}
	
	//save 1 safe zone
	for (int i = last_bluezone_point[0]; i < last_bluezone_point[0] + bluezone_len; i++)
		for (int j = last_bluezone_point[1]; j < last_bluezone_point[1] + bluezone_len; j++)
			map[i][j] = 1;

	//save 2 player location
	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 6; j++) {
			if ((i - 1) * 6 + j == loca)
				map[i][j] = 2;
		}
	}
}

void calc_bluezone2(int map[][7], int phase_num, int loca) {
	//
	int safe_num = in_safezone(map, phase_num);

	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 6; j++) {
			if (map[i][j] == 2) {
				if (safe_num == 1)
					map[i][j] = 1;
				else if (safe_num == 0)
					map[i][j] = 0;
				else
					map[i][j] = -1;
			}	
		}
	}

	//0 -> -1
	for (int i = 1; i <= 6; i++)
		for (int j = 1; j <= 6; j++)
			if (map[i][j] == 0)
				map[i][j] = -1;

	//save 2 player location
	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 6; j++) {
			if ((i - 1) * 6 + j == loca)
				map[i][j] = 2;
		}
	}

}

void print_bluezone(int map[][7], int loca, int is_in_safezone) {
	//print bluezone
	
	printf("---------MAP---------\n");
	for (int i = 1; i <= 6; i++) {
		printf("|");
		for (int j = 1; j <= 6; j++) {
			int num = (i - 1) * 6 + j;

			if (map[i][j] == 2)
				printf("  X");

			else if (map[i][j] == 1)
				printf(" %2d", num);

			else if (map[i][j] == 0)
				printf("  o");

			else
				printf("  .");
		
		}
		printf(" |");

		if (i == 3)
			printf(" Number: Safe zone\n");
		else if (i == 4)
			printf("      o: Will be blue zone\n");
		
		else if(i == 5)
			printf("      .: Blue zone\n");
		
		else if (i == 6) {
			printf("      X: Your location(%d, ", loca);

			if (is_in_safezone == 0) printf("In will be blue zone)\n");
			else if (is_in_safezone == -1) printf("In blue zone)\n");
			else printf("In Safe zone)\n");
		}
		else
			printf("\n");

	}
	printf("---------------------\n");
	

	/*
	 1  2  3  4  5  6 
	 7  8  9 10 11 12
	13 14 15 16 17 18 
	19 20 21 22 23 24
	25 26 27 28 29 30
	31 32 33 34 35 36 

	o o o o x x
	o o o o x x
	o o o o x x
	o o o o x x
	x x x x x x
	x x x x x x

	1  1 2 3 4 5 6
	2  1 2 3 4 5 6
	3  1 2 3 4 5 6
	4  1 2 3 4 5 6
	5  1 2 3 4 5 6
	6  1 2 3 4 5 6

	*/

}

int in_safezone(int map[][7], int phase_num) {
	int safezone_size = (5 - phase_num) * (5 - phase_num);
	int safezone_cnt = 0;
	int wbbzone_size = (5 - phase_num) * 2 + 1;	//will be blue zone_cnt
	int wbbzone_cnt = 0;

	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 6; j++) {
			if (map[i][j] == 1)
				safezone_cnt++;
			else if (map[i][j] == 0)
				wbbzone_cnt++;
		}
	}

	if (safezone_cnt == safezone_size) {
		if (wbbzone_cnt != wbbzone_size && wbbzone_cnt >= 1)
			return 0;					//in will be blue zone
		else
			return -1;					//in blue zone
	}
	else
		return 1;						//in safezone
}

int calc_valid_loca(int *p_have_car, int * p_loca, int loca_tmp) {
	//if valid location num - return 1, *p_loca = loca_tmp
	//else return 0

	int num = *p_loca - loca_tmp;

	if (num == 1 || num == -1 || num == 6 || num == -6) {
		*p_loca = loca_tmp;
		return 1;
	}
	else if (num == 2 || num == -2 || num == 12 || num == -12 || num == 7 || num == 5 || num == -7 || num == -5) {
		if (*p_have_car >= 1) {
			*p_loca = loca_tmp;
			(*p_have_car)--;
			return 1;
		}
		else {
			return 0;
		}
	}
	else
		return 0;

	/*
	 1  2  3  4  5  6
	 7  8  9 10 11 12
	13 14 15 16 17 18
	19 20 21 22 23 24
	25 26 27 28 29 30
	31 32 33 34 35 36
	*/
}

int create_rand_item(int *p_rand_item) {
	/*
	item list

	0 Guns
		0 P1911 (pistol)
		1 AKM (AR)
		2 M24 (SR)
		3 Crossbow

	1 Throwables
		0 Frying pan						0
		1 Grenade							1
		2 C4								2

	2 Healing items
		0 bandage healing hp +20			3
		1 First Aid kit healing hp to 100	4

	3 Equipment
		0 Police vest increase hp			5
		1 helmet increase hp				6

	4 Car
		0 Car can move 2 space				7
	*/

	//item num random 3 - 5
	srand(time(NULL));
	int item_num = rand() % 3 + 3;

	//rand gun 0 - 3
	srand(time(NULL));
	int gun_num = rand() % 4;
	*p_rand_item = gun_num;
	
	//rand item 4 - 11
	for (int i = 1; i < item_num; i++) {
		srand(time(NULL)+i);
		int tmp = rand() % 8 + 4;
		*(p_rand_item + i) = tmp;
	}


	return item_num;
}

void print_rand_item(int *p_item_list, int *p_rand_item, int item_cnt) {
	/*"P1911 (Pistol)", "AKM (AR)", "M24 (SR)", "Crossbow",
									"Frying Pan", "Grenade", "C4",
									"bandage", "First Aid Kit",
									"Police Vest", "Helmet",
									"Car" };*/
	char item_list[12][21] = { "P1911 (Pistol)", "AKM (AR)", "M24 (SR)", "Crossbow",
									"Frying Pan", "Grenade", "C4",
									"bandage", "First Aid Kit",
									"Police Vest", "Helmet",
									"Car" };

	for (int i = 0; i < item_cnt; i++) {
		printf("%d. %s", *(p_rand_item+i), item_list[*(p_rand_item + i)]);
		if (i != item_cnt - 1)
			printf(" / ");
	}
	printf("\n");

}

int check_valid_item(int tmp1, int tmp2, int* p_rand_item, int item_cnt) {

	int tmp_rand_item[5] = { -1, -1, -1, -1, -1 };
	for (int i = 0; i < 5; i++) {
		tmp_rand_item[i] = *(p_rand_item + i);
	}

	int check1 = 0, check2 = 0;
	//1 5 5 6
	//1 5
	for (int *p = tmp_rand_item; p < tmp_rand_item + item_cnt; p++) {
		if (*p == tmp1) {
			check1++;
			*p = -1;
			tmp1 = -1;
		}
		else if (*p == tmp2) {
			check2++;
			*p = -1;
			tmp2 = -1;
		}
	}

	if (check1 + check2 == 2)
		return 1;
	else
		return 0;
	
}

void print_player_item_list(int player_item_list[12]) {
	char item_list[12][21] = { "P1911 (Pistol)", "AKM (AR)", "M24 (SR)", "Crossbow",
									"Frying Pan", "Grenade", "C4",
									"bandage", "First Aid Kit",
									"Police Vest", "Helmet",
									"Car" };
	int flg = 0;

	printf("=====Your item list=====\n");
	for (int i = 0; i < 12; i++) {
		if (i != 9 && i != 10 && player_item_list[i] != 0) {
			printf("%d %s", player_item_list[i], item_list[i]);
			printf(" / ");
			flg = 1;
		}
	}
	if (flg == 0)
		printf("None");
	printf("\n");
	
}

void print_hp(int hp) {
	printf("Your HP: %d - HP bar: ", hp);

	int hp_num = hp / 10;
	if (hp_num > 10) hp_num = 10;

	int x_num = 10 - hp_num;

	int armed_num = hp / 10 - 10;
	if (armed_num < 0) armed_num = 0;

	for (int i = 0; i < hp_num; i++) {
		if (i == 5) printf(" ");
		printf("0");
	}
	for (int i = 0; i < x_num; i++) printf("X");
	for (int i = 0; i < armed_num; i++) printf(")");
	printf("\n");

}

int rand_meet_enemy(int diff) {
	srand(time(NULL));

	//1-15
	int rand_num = rand() % 15 + 1;

	if (rand_num <= diff) return 1;
	else return 0;
}

void print_player_weapon(int player_item_list[12]) {
	char item_list[12][21] = { "P1911 (Pistol)", "AKM (AR)", "M24 (SR)", "Crossbow",
									"Frying Pan", "Grenade", "C4",
									"bandage", "First Aid Kit",
									"Police Vest", "Helmet",
									"Car" };

	printf("===Your weapon list===\n");
	for (int i = 0; i < 7; i++) {
		if (player_item_list[i] != 0) {
			printf("%d. %s", i, item_list[i]);
			printf(" / ");
		}
	}
	printf("12. Punch");
	printf("\n");

}

int check_valid_weapon(int player_item_list[12], int tmp_num) {
	int re = 0;

	if (tmp_num == 12)
		re = 1;
	else if (tmp_num < 0 || tmp_num > 12)
		re = 0;
	else {
		if (player_item_list[tmp_num] >= 1)
			re = 1;
		else
			re = 0;
	}

	return re;
}

int calc_enemy_fight(int diff, int tmp_num, int hp) {
	int weapon_damage = 0;	//5 - 9

	if (tmp_num == 12)
		weapon_damage = 5;
	else if (4 <= tmp_num && tmp_num <= 6)
		weapon_damage = tmp_num + 2;
	else
		weapon_damage = tmp_num + 6;

	int attack_damage = 120 - (11 - diff) * weapon_damage;
	
	//diff 1 ~ 10
	//120 - 90 ~ 5
	//30 ~ 115
	//hp == 100
	//70 ~ -15

	hp -= attack_damage;
	if (hp < 0) hp = 0;

	return hp;
}

int have_healing_item(int player_item_list[12], int hp) {
	if ((player_item_list[7] >= 1 || player_item_list[8] >= 1) && hp < 100)
		return 1;
	else
		return 0;
}

int print_healing_item_list(int player_item_list[12]) {
	char item_list[12][21] = { "P1911 (Pistol)", "AKM (AR)", "M24 (SR)", "Crossbow",
									"Frying Pan", "Grenade", "C4",
									"bandage", "First Aid Kit",
									"Police Vest", "Helmet",
									"Car" };

	printf("=====Your healing item list=====\n");
	printf("1: %d %s / ", player_item_list[7], item_list[7]);
	printf("2: %d %s", player_item_list[8], item_list[8]);
	printf("\n");

	if (player_item_list[7] > 0 && player_item_list[8] > 0)
		return 2;
	else if (player_item_list[7] > 0 && player_item_list[8] == 0)
		return 0;
	else if (player_item_list[7] == 0 && player_item_list[8] > 0)
		return 1;
}
