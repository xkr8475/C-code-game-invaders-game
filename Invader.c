#include <stdio.h> //printf 등의 함수를 사용하기위한 헤더파일
#include <mysql.h> //mysql 데이터베이스 연동을 위한 헤더파일
#include <windows.h> //윈도우 API를 사용하기 위한 헤더파일
#include <stdlib.h> //문자열 변환 난수생성 등을 위한 헤더파일
#include <time.h> //랜덤함수에서 값을 랜덤하게 생성하기위한 헤더파일
#include <conio.h> //콘솔 입출력 함수를 제공하는 헤더파일
#include <mmsystem.h> //playsound 함수를 사용하기위한 헤더파일
#pragma comment(lib,"winmm.lib") //playsound 함수가 포함되어있는 라이브러리
#pragma comment(lib, "libmysql.lib")  //  -- mysql db연동 
#define ESC 27 //  //ESC라는 함수명에 27이란 값의 매크로를 지정.
#define EXTENDASCII -32  //EXTENDASCII라는 함수명에 -32이란 값의 매크로를 지정
#define MAXBULLET 2 //MAXBULLET라는 함수명에 2란 값의 매크로를 지정 (플레이어가 쏠수있는 최대 총알 수 결정)
#define MAXENEMY 5 //MAXENEMY라는 함수명에 5란 값의 매크로를 지정 (적군의 함선이 나오는 최대 수를 결정)
#define MAXENEMY2 10  //위와 같은 방식  대신 함수명이 다름(난이도 2를 만들기 위함) 
#define MAXENEMY3 10
#define MAXENEMYBULLET 3 //MAXENEMYBULLET라는 함수명에 3이라는 값의 매크로를 지정 (적군이 쏘는 총알의 최대 수를 결정)
#define MAXENEMYBULLET2 10 //위와 같은 방식  대신 함수명이 다름(난이도 2를 만들기 위함)
#define MAXENEMYBULLET3 10
#define randomize() srand ( (unsigned int) time ( NULL ) ) //randomize 랜덤함수를 사용하기위해 선언된 매크로 함수.
#define random() ((rand() %4)) //randomize 와 다른 크기의 랜덤한 함수를 생성하기 위한 또다른 랜덤 매크로 함수.

//-------------------------------
//MYSQL DB연동 구분
#define DB_HOST "127.0.0.1" //DB의 값을 넘겨주기 필요한 값들을 적어준다.
#define DB_USER "root" //접속형태
#define DB_PASS "1234" //USER PASS
#define DB_NAME "test" //USER ID
#define CHOP(x) x[strlen(x) - 1] = ' ' //공백 제거
//-------------------------------
//-------------------------------
//전역함수 선언 부분 (전역변수? Invader.c안에서 공통적으로 사용할수있는 변수를 선언하는법).
int px = 30;
int bx = -1, by;
int mx = -1, my;
int i;
int y = 0;
int Score = 0;
//-------------------------------
// 함수 선언
void gotoxy(int x, int y); //gotoxy를 사용하기위한 gotoxy 함수 선언
void delay(clock_t); //시간차 출력을 주기위한 delay 함수 선언
void SetColor(int color); //API함수 SetColor를 만들어주기위한 Setcolor함수 선언.
void LoadingStage(); //로딩화면을 만들어주기위한 LoadingStage 함수 선언
void HeightLine(); //게임화면 안에서 중간에 선을 그어 플레이 화면과 조작화면을 나누기 위한 HeightLine 함수 선언.
void DrawPlayer(); //플레이어를 그려주기 위한 DrawPlayer함수
void ErasePlayer();//플레이어가 지나간 자리에 플레이어 그림을 지워주기위한 ErasePlayer함수 선언
void DrawBullet(int); //총알을 그려주기위한 DrawBullet 함수 선언.
void EraseBullet(int); //똑같이 총알을 그려주고 지나간 자리를 지워주기위한 EarseBullet함수
void PlayerMoveAction(); //플레이어가 움직이는 알고리즘을 종합한  PlayerMoveAction 함수 선언.
void DrawMissile(); //미사일 특수공격을 표현하기위한 DrawMissile 함수 선언.
void EraseMissile(); //미사일을 그려주고 지나간 자리를 지워주기위한 EraseMissile 함수선언.
void PlayerBulletAction(); //플레이어가 총알을 날리는 알고리즘을 모두 종합한 PlayerBulletAction 함수 선언.
void PlayerMissileAction(); //플레이어가 미사일을 날리는 알고리즘을 모두 종합한 PlayerMissileAction 함수 선언.

void DrawEnemy(); //적을 그려주기위한 DrawEnemy 함수 선언.
void DrawEnemy2(); // 레벨 2
void DrawEnemy3(); // 레벨 3

void EnemyMoveAction(); //적의 움직임을 알고리즘을 종합한 EnemyMoveAction  함수 선언.
void EnemyMoveAction2(); //레벨 2
void EnemyMoveAction3(); //레벨 3

void EnemyBulletAction(int); // 적이 총알을 쏘는 알고리즘을 종합한 EnemyBulletAction 함수 선언.
void EnemyBulletAction2(int);
void EnemyBulletAction3(int);

void DrawEnemyBullet(int); // 적 총알의 움직음을 구현한 DrawEnemyBullet 함수 선언.
void EraseEnemyBullet(int);  //적 총알이 지나간 자리의 총알을 지워주는 EraseEnemyBullet 함수 선언.

void DrawEnemyBullet2(int);
void EraseEnemyBullet2(int);

void DrawEnemyBullet3(int);
void EraseEnemyBullet3(int);

void MoveEnemyBullet(); // 적 총알의 움직임을 정의한 MoveEnemyBullet 함수 선언.
void MoveEnemyBullet2();
void MoveEnemyBullet3();

void CursorView(char); //마우스 커서를 숨기기 위한 CursorView 함수 선언.
void EnemyCrash(); //적군의 총알 충돌 판정기능을 위한 EnemyCrash 함수 선언.
void PlayerCrash(); //플레이어의 총알 충돌 판정 기능을 위한 PlayerCrash 함수 선언.

void EnemyCrash2(); //적군의 총알 충돌 판정기능을 위한 EnemyCrash 함수 선언.
void PlayerCrash2(); //플레이어의 총알 충돌 판정 기능을 위한 PlayerCrash 함수 선언.

void EnemyCrash3(); //적군의 총알 충돌 판정기능을 위한 EnemyCrash 함수 선언.
void PlayerCrash3(); //플레이어의 총알 충돌 판정 기능을 위한 PlayerCrash 함수 선언.
int DrawInfo(); // 움직이는 방법, 기본적인 조작키를 안내하도록 화면에 설명을 그려주는 Drawinfo 함수 선언.
//-------------------------------


//-------------------------------
//색 넣기 부분
enum
{
	//API함수를 그냥쓰기에는 복잡하여 열거형으로 각 색의 값을 가지는 상수의 이름을 명확하게 정의하기 위해 열거형으로 만듬.
	BLACK, 
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DARK_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};
//-------------------------------

//플레이어 총알 구현을 위한 구조체 만들기.
struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌
	int x, y; //x y 변수 선언.
} Bullet[MAXBULLET]; //밑 함수들에서 구조체를 사용할때 이름을 설정해줌.

//적 구현을 위한 구조체 만들기
struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌.
	int type;   //-------------------------------------
	int x, y;
	int dest;   // 각 변수명 정의 
	int nFrame;
	int nStay;  //-------------------------------------
} Enemy[MAXENEMY];//만든 구조체의 이름 만들기.

struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌.
	int type;   //-------------------------------------
	int x, y;
	int dest;   // 각 변수명 정의 
	int nFrame;
	int nStay;  //-------------------------------------
} Enemy2[MAXENEMY2];//만든 구조체의 이름 만들기.

struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌.
	int type;   //-------------------------------------
	int x, y;
	int dest;   // 각 변수명 정의 
	int nFrame;
	int nStay;  //-------------------------------------
} Enemy3[MAXENEMY3];//만든 구조체의 이름 만들기.

char *arEnemy[] = { " <<○>> ", " <<●>> ", " <<◎>> ", " <<◆>> " };  //arEnemy라는 배열안에 적군의 함선 모양을 넣어줌.
char *bsEnemy[] = { " ||==○==|| "};

//적군 총알 구현을 위한 구조체 만들기
struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌.
	int x, y;
	int nFrame; // 각 변수명 정의.
	int nStay;
}EnemyBullet[MAXENEMYBULLET]; //적군 총알 구조체의 이름 정의

struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌.
	int x, y;
	int nFrame; // 각 변수명 정의.
	int nStay;
}EnemyBullet2[MAXENEMYBULLET2]; //적군 총알 구조체의 이름 정의

struct
{
	BOOL exist; //exist의 값이 참인지 거짓인지 판단해줌.
	int x, y;
	int nFrame; // 각 변수명 정의.
	int nStay;
}EnemyBullet3[MAXENEMYBULLET3]; //적군 총알 구조체의 이름 정의
//-------------------------------
//메인함수 부분
int main()
{
	//-------------------------------
	//MYSQL DB 연동 
	//MYSQL 선언 해주는 부분
	MYSQL* connection = NULL, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW  sql_row;
	//데이터를 넘겨받을때 필요한 값이 저장될 변수 선언
	int query_stat = 0;
	char ans = 0;
	char id[20];
	char passwd[20];
	char query[255];
	while (1) { //무한 반복.
		system("cls");//화면 비워주기
		mysql_init(&conn); //초기화

		//한글을 사용하기위해 셋팅해주는 부분
		mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
		mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");
		//connection 에 값을 넣을때 들어가는 양식 HOST이름, USER이름, PASS, NAME, 포트번호, 소캣, 플레그 값
		connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);

		if (connection == NULL) //만약 connection에 값이 제대로 넘어오지 않았다면 오류문자 출력
		{
			fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn)); //어느 부분에서 오류가 났는지 보여줄수있도록 출력
			return 1; //return 1은 정상 종료를 하고 다음 문장을 실행하라는 뜻.
		}
		//connection과 마찬가지로 query_stata 값에 로그인 하기위해 값을 저장해줌.
		query_stat = mysql_query(connection, "select * from login");
		//만약 query_stat 에도 값이 넘어 오지 않았다면 오류문자 출력
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return 1;
		}
		//들어온값이 sql_result로 넘겨준다
		sql_result = mysql_store_result(connection);
		//아이디와 패스워드를 입력받는다. 이때 값이 들어가거나 넘어올때의 형태는 배열로 넘어오게된다.
		printf("%-8s\n", "아이디");
		//입력받는 데이터의 크기만큼 입력받아야하기 때문에 while문을 돌아 아이디, 비밀번호를 입력받아야한다면 총 두번, 점수까지 받는다면 3번 돌게된다.
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			//각각 아이디, 비밀번호 값을 받아준다, 배열로서 넘어오기때문에 점수를 추가하고싶다면 sql_row[2]를 생성해줘야한다.
			printf("%s\n", sql_row[0]);
		}
		//비워주는(초기화)해주는 역활.
		mysql_free_result(sql_result);

		//회원가입
		printf("회원가입을 원하십니까(Y/N) : ");
		scanf("%c", &ans);
		scanf("%*c");
		//위에서 받은값이 Y라면 회원가입 문장 실행
		if (ans == 'y' || ans == 'Y') {
			printf("아이디 : ");
			fgets(id, 20, stdin); //scanf와 비슷하지만 fgets은 공백을 혀용한다 , stdin 키보드로 입력받겠다는 뜻
			CHOP(id); //공백을 제거해주는 문장

			printf("패스워드 : ");
			fgets(passwd, 20, stdin); //똑같이 fgets로 공백을 허용해서 받고 stdin으로 키보드로 입력을 받는다
			CHOP(passwd); //공백을 제거해주는 문장

			//sprintf 는 printf등과는 다르게 변수(버퍼)에 문자열을 출력한다.
			//여기서 버퍼는 배열로 이루어져있기때문에 배열 형싱으로 데이터를 넘겨받기위함에서는 sprintf를 쓴다.
			sprintf(query, "insert into login values" "('%s', '%s')", id, passwd);
			
			query_stat = mysql_query(connection, query); //connection 의 값과 query 값이 query_stat에 저장된다.
			if (query_stat != 0)  //만약 query_stat 이 정상적으로 값이 넘어오지않았다면 오류 메세지를 출력해준다.
			{
				fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
				return 1;
			}
		}
		else { //위에서 회원가입 여부를 물었을때 Y이외의 답이 돌아왔을때 ( 회원가입을 하지않을때) 실행된다.
			printf("아이디 : ");
			fgets(id, 20, stdin); //위와 똑같이 fgets, stdin을 사용하여 받는다
			CHOP(id); //공백제거

			printf("패스워드 : ");
			fgets(passwd, 20, stdin);
			CHOP(passwd);
			//똑같은 배열형식으로 받아오기 때문에 sprintf를 사용하여 받아온다
			sprintf(query, "select * from login where id =  '%s' and passwd = '%s'", id, passwd);
			//데이터값을 넘겨줌
			query_stat = mysql_query(connection, query);
			if (query_stat != 0) //만약 값이 제대로 넘어가지않았다면 오류 메세지 출력
			{
				fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
				return 1;
			}
			sql_result = mysql_store_result(connection);

			if ((sql_row = mysql_fetch_row(sql_result)) != NULL) //값이 정상적으로 넘어왔을때(값이 NULL이 아닐때) 실행
			{
				printf("로그인 성공 \n"); //로그인 성공 메세지 출력
				system("pause");
				break;
			}
			else {
				printf("로그인 실패\n"); //값이 제대로 넘어오지 않았다면 (NULL이라면) 로그인 실패 메세지 출력
				system("pause");
			}
		}

		mysql_close(connection); //mysql 구문 종료
	}

	//-------------------------------
	//실제로 게임이 구현되는 부분.
	system("cls"); // 화면 지워주기.
	CursorView(0); // 마우스 커서 안보이게 하기
	PlaySound(TEXT("abc.wav"), NULL, SND_ASYNC | SND_LOOP);
	LoadingStage();// 처음 게임 실행하는 화면을 구현해둔 LoadingStage 함수 호출.
	system("mode con: lines=40"); // 콘솔창의 크기를 설정
	system("cls"); // 화면 지워주기.
    randomize(); 
    int test = 1;
	int test1 = 1;

	while (1) //계속 실행될수 있도록 무한 반복
	{
		HeightLine(); //화면 중앙에 선을 그려주는 인터페이스를 구현한 HeightLine 함수 호출
		DrawInfo(); //화면을 나누고 오른쪽에 플레이 방법을 그려주는 DrawInfo 함수 호출
		
		int level = Score;

		if (Score == 0 || Score <= 50) {
			PlayerMoveAction(); //플레이어의 움직임을 구현한 PlayerMoveAction 함수를 호출함으로서 게임이 시작되면 플레이어가 움직일수있게 해줌.
			EnemyCrash();
			PlayerBulletAction(); //플레이어가 발사하는 총알의 움직임과 키입력을 판단하는 PlayerBulletAction 함수 호출
			EnemyCrash();
			PlayerMissileAction(); //플레이어가 발사하는 미사일의 움직임과 키입력을 판단하는 PlayerBulletAction 함수 호출.
			EnemyCrash();
			DrawEnemy(); //적군 함선을 그려주는 DrawEnemy 함수 호출
			EnemyCrash();
			EnemyMoveAction(); //적군 함선이 움직이고 총알을 발사하는등에  행동을 가진 EnemyMoveAction 함수 호출.
			EnemyCrash();
			MoveEnemyBullet(); //적군의 총알이 움직이는 함수

			EnemyCrash(); //적군의 충돌판정을 체크해주는 EnemyCrash 함수 호출
			PlayerCrash(); //플레이어의 충돌판정을 체크해주는 PlayerCrash 함수 호출
			delay(10);
		}
		else if (Score == 60 || Score <= 100) {
			if(test == 1){
				system("cls");
				SetColor(SKYBLUE);
				gotoxy(50, 8); printf("Level 2 START - SPEED UP !!!"); delay(500);
				delay(1000);
				getch();
				system("cls");
				test = 0;
			}
			PlayerMoveAction(); //플레이어의 움직임을 구현한 PlayerMoveAction 함수를 호출함으로서 게임이 시작되면 플레이어가 움직일수있게 해줌.
			EnemyCrash2();
			PlayerBulletAction(); //플레이어가 발사하는 총알의 움직임과 키입력을 판단하는 PlayerBulletAction 함수 호출
			EnemyCrash2();
			PlayerMissileAction(); //플레이어가 발사하는 미사일의 움직임과 키입력을 판단하는 PlayerBulletAction 함수 호출.
			EnemyCrash2();
			DrawEnemy2(); //적군 함선을 그려주는 DrawEnemy 함수 호출
			EnemyCrash2();
			EnemyMoveAction2(); //적군 함선이 움직이고 총알을 발사하는등에  행동을 가진 EnemyMoveAction 함수 호출.
			EnemyCrash2();
			MoveEnemyBullet2(); //적군의 총알이 움직이는 함수

			EnemyCrash2(); //적군의 충돌판정을 체크해주는 EnemyCrash 함수 호출tTt
			PlayerCrash2(); //플레이어의 충돌판정을 체크해주는 PlayerCrash 함수 호출
			delay(10);
		}
		else if (Score == 110 || Score <= 1000) {
			if (test1 == 1) {
				system("cls");
				SetColor(RED);
				gotoxy(57, 5); printf("WARNING");
				gotoxy(50, 8); printf("Level 3 START - HELL STAGE !!!"); delay(500);
				delay(1000);
				getch();
				system("cls");
				test1 = 0;
			}
			PlayerMoveAction(); //플레이어의 움직임을 구현한 PlayerMoveAction 함수를 호출함으로서 게임이 시작되면 플레이어가 움직일수있게 해줌.
			EnemyCrash3();
			PlayerBulletAction(); //플레이어가 발사하는 총알의 움직임과 키입력을 판단하는 PlayerBulletAction 함수 호출
			EnemyCrash3();
			PlayerMissileAction(); //플레이어가 발사하는 미사일의 움직임과 키입력을 판단하는 PlayerBulletAction 함수 호출.
			EnemyCrash3();
			DrawEnemy3(); //적군 함선을 그려주는 DrawEnemy 함수 호출
			EnemyCrash3();
			EnemyMoveAction3(); //적군 함선이 움직이고 총알을 발사하는등에  행동을 가진 EnemyMoveAction 함수 호출.
			EnemyCrash3();
			MoveEnemyBullet3(); //적군의 총알이 움직이는 함수

			EnemyCrash3(); //적군의 충돌판정을 체크해주는 EnemyCrash 함수 호출
			PlayerCrash3(); //플레이어의 충돌판정을 체크해주는 PlayerCrash 함수 호출
			delay(10);
		}
	}
	_getch(); //게임이 종료되고 키입력을 받으면 완전히 종료가 된다.
	return 0;
}
//-------------------------------

//-------------------------------
//함수구현 부분
void gotoxy(int x, int y)
{
	//gotoxy를 사용하기위한 기본적인 구조
	//안에서의 표현은 자유이지만 void gotoxy와 , SetConsoleCusorPosition 은 필수적으로 gotoxy가 구성되기위한 함수들이다.

	COORD pos = { x,y }; //x값과 y값을 맴버로 가지는 구조체 타입 ( x 와 y 의 값을 저장하는 역활을 한다)

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
	//표준콘솔출력의 핸들 ... Window.h 헤더파일 안쪽에 
	//SetConsoleCusorPosition 함수의 정의가 구현되어있다..

}
//dealy라는 이름의 함수 정의
void delay(clock_t n) //colock_t는 long타입을 다른이름으로 typedef해둔것. ( long = colock_t) 결과적으로 n은 long타입이 된다.
{
	//delay 기능을 사용하기위한 기본적인 함수 구현.
	//무식하게 for문을 돌려서 dealy를 주는것보다 time.h 의 헤더파일에 있는 기능을 활용하여 dealy를 사용하면 훨씬 효율적으로 사용할수있다.
	clock_t start = clock(); 
	//while 대신 for도 가능.
	while (clock() - start < n);

}
//SetColor라는 색을 사용하기위한 함수 구현
void SetColor(int color)
{
	//system("color")의 함수를 사용하여 색을 지정할수있지만 system함수를 사용하는것은 호출시간 지연등의 문제로 비효율적이다.
	//그래서 window.h의 헤더파일을 사용하면 사용할수있는  SetConsoleTextAttribute 를 사용하는것이 일반적.
	//또한 system() 과 setcolor()의 차이점은. system은 화면 전체의 색깔을 변경하는것이고 setcolor는 다음에 나올 텍스트 부터 색깔을 변환 시켜준다.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//처음 인코딩 시켰을때 플레이어에게 보여주는 게임 시작 화면. LoadingStage라는 함수를 구현, 정의 한다.
void LoadingStage()
{
	int i; //반복문을 사용하기위한 i변수 선언

	for(i = 33; i < 88; i += 2) 
	{
		SetColor(SKYBLUE); //인터페이스의 색을 설정해준다
		gotoxy(i, 4); printf("ㅡ"); // i의 좌표 x는 기본값 20에 자신에게 2를 계속더해주면서 x값만 변화하며  "ㅡ" 가 찍히도록함 (윗줄의 가로를 구현) 
		delay(30); //찍히는 속도 설정
	}

	gotoxy(88, 4); printf("ㄱ"); //세로로 넘어가는 부분을 매끄럽게 하기위해 "ㄱ" 을 찍어줌
	for (i = 5; i < 12; i++) // "l" 세로 인터페이스를 찍기위한 반복문.
	{
		SetColor(VIOLET); //인터페이스의 색을 설정해준다
		gotoxy(88, i); printf("┃"); // x좌표의 최대인 50은 고정으로 두고 거기서부터 i의 좌표값이 5부터 하나씩 증가하면서 " ㅣ " 를 찍어준다
		delay(50); //찍히는 속도 설정.
	}
	gotoxy(88, 12); printf("┛"); //가로로 넘어가는 부분을 매끄럽게 하기위해 "┛" 을 찍어줌
	for (i = 86; i > 31; i -= 2) //x 값을 다시 왼쪽으로 찍기위한 반복문
	{
		SetColor(SKYBLUE); //인터페이스의 색을 설정해줌.
		gotoxy(i, 12); printf("ㅡ"); //y의값은 10으로 고정 거기서 x값 48부터 -2씩 해줌으로서 왼쪽으로 x값이 20이 될때까지 찍는다.
		delay(30);//찍히는 속도 설정.
	}

	gotoxy(32, 12); printf("ㄴ"); //인터페이스의 색을 설정해준다.
	for (i = 11; i > 4; i--) //마지막 세로줄을 찍어주기위한 반복문.
	{
		SetColor(RED); //인터페이스의 색을 설정해줌.
		gotoxy(31, i); printf("┃"); //x값은 20으로 고정 그 좌표에서 위로올라가며 찍어야 하기때문에 총 5번 . 9 ~ 4 까지 올라가면서 세로로 찍힘.
	    delay(50);//찍히는 속도 설정.
	}

	gotoxy(31, 4);  printf("┏"); //마지막 인터페이스의 가로로 넘어가는 부분을 자연스럽게 하기위해 "┏"을 찍어줌
	delay(-50); 

	gotoxy(42, 8); printf("I"); delay(500);//-------------------------------
	gotoxy(48, 8); printf("N"); delay(500);
	gotoxy(54, 8); printf("V"); delay(500);
	gotoxy(60, 8); printf("A"); delay(500);//찍혀진 네모 인터페이스 안에 I N V A D E R라는 글자를 딜레이를 주면서 출력.
	gotoxy(66, 8); printf("D"); delay(500);
	gotoxy(72, 8); printf("E"); delay(500);
	gotoxy(78, 8); printf("R"); delay(500);//-------------------------------

	gotoxy(49, 19); printf("PLEASE PRESS ANY KEY"); //마지막으로 밑에 "아무 키나 누르면 게임 시작"란 문자를 출력.

	_getch(); //사용자의 키를 입력받으면 넘어간다.
}
//게임시작후 중앙에 세로줄을 그려주기 위한 HeightLine 함수 정의
void HeightLine()
{
	int i; //반복문을 사용하기위한 i

	for (i = 0; i < 40; i++) //콘솔창의 맨위 부터 맨아래 (콘솔창의 세로길이를 40으로 설정했음으로 맨 아래는 40) 까지 반복문을 돈다.
	{
		SetColor(SKYBLUE); //세로줄의 색 지정
		gotoxy(55, i); printf("|"); // "ㅣ"를 x좌표값 (콘솔창의 중간) 의 y값 0 (콘솔창의 맨위) 부터 아래까지 찍어준다.
	}
}
//플레이어 함선을 그려주는 DrawPlayer 함수 정의
void DrawPlayer()
{
	SetColor(GREEN); //플레이어의 함선은 초록색으로 바꿔줌
	gotoxy(px, 39); printf("<<★>>");
	//처음 플레이어의 함선이 찍힐 위치에 플레이어 함선을 그려준다. (px의 값은 전역변수의 px값으로서 30으로 초기화 됨. px = player X)
	//여기서 py는 인베이더 게임에서는 플레이어의 움직임이 위아래는 없기때문에 구현하지않았다.
}
//플레이어 함선이 움직인자리를 지워주는 ErasePlayer 함수 정의.
void ErasePlayer()
{
	gotoxy(px, 39); printf("      "); //플레이어의 x값 px값을 " " 공백으로 해줌으로서 플레이어가 움직인 자리에 플레이어 함선이 남아있지않게 해준다.
}
//총알을 그려주는 DrawBullet 함수 정의.
void DrawBullet(int i)
{
	int count = 1;
	SetColor(GREEN); //플레이어의 총알이라 색도 같은 초록색으로 설정.
	gotoxy(Bullet[i].x,Bullet[i].y); printf("▲"); //총알을 gotoxy로 위에 정의해둔 Bullet라는 구조체를 가져와 움직일수있도록 해줍니다.
}
//총알을 지워주는 EraseBullet 함수 정의.
void EraseBullet(int i)
{
	//총알이 지니간 모양이 남으면 안되기때문에 지나간 자리를 그대로 따라서 지워주는 함수를 구현합니다.
	gotoxy(Bullet[i].x, Bullet[i].y); printf("  ");
}
//플레이어의 움직임을 총합한 PlayerMoveAction 이라는 함수 정의
void PlayerMoveAction()
{	
	//다중으로 키입력을 받을수 있게해주는 GetAsyncKeyState()함수를 사용 px > 1은 플레이어 캐릭터가 지정된 구간 밖으로 나가지못하게 해준다.
	if (GetAsyncKeyState(VK_LEFT) < 0 && px > 1)
	{
		ErasePlayer(); //플레이어가 지나간 흔적을 지워주는 함수
		px--; //왼쪽으로 가기위해 왼쪽으로 이동
		DrawPlayer(); //플레이어를 그려줌.
	}
	//다중입력 함수 사용에 RIGHT 오른쪽으로 갈때 px가 47 이상 움직일수있는 구간 밖으로 나가는 것을 막아준다.
	else if (GetAsyncKeyState(VK_RIGHT) < 0 && px < 47)
	{
		ErasePlayer(); //플레이어가 지나간 흔적을 지워주는 함수
		px++; //오른쪽으로 가기위해 오른쪽으로 이동
		DrawPlayer(); //플레이를 그려줌.
	}

	if (_kbhit()) //키보드가 눌렸는지 체크해주는 함수 _kbhit() 눌리면 0이외의 값을 리턴.
	{
		int ch = _getch(); //ch에 눌린값을 대입한다.
		switch (ch) //키보드가 눌려서 값이 들어오면 switch문 실행
		{
		case ' ': //입력받은 값이 스페이스바 라면 실행.
			for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) { ; } //총알이 3번 만큼 발사될수 있게 해준다.
			if (i != MAXBULLET) //최대 총알의 값만큼 발사할수있게 해야하기 떄문에 i가 MAXBULLET의 값과 같다면 0 다른다면  1 을 출력해준다.
			{
				Bullet[i].x = px + 3;  //플레이어의 가운데에서 쏘기 위해 +3 <<*>> 일경우 플레이어의 x + 3 을해줘야 * 부분에서 총이나간다.
				Bullet[i].y = 38; //총알의 y값이 38로 위로 쏠수있게 해준다.
				Bullet[i].exist = TRUE;
			}
			break;
		case 'D': //입력받은값이 D라면 미사일을 발사해야함으로 밑에 if문을 실행.
			if (mx == -1) //미사일의 x값이 -1라면
			{
				mx = px + 3;  //플레이어의 가운대의 값을 미사일 x의 값에 저장
				my = 38; //미사일의 y값을 38로 설정하여 미사일이 위로 올라가게 해줌.
			}
			break;
		case 'd': //위와 똑같은 매커니즘.
			if (mx == -1)
			{
				mx = px + 3; //플레이어의 가운대의 값을 미사일 X의 값에 저장.
				my = 38; //미사일의 Y값을 38로 설정하여 미사일이 위로 올라가게 해줌.
			}
			break;
		case ESC:
			exit(0); //정상종료의 뜻.
		}
	}
}
//플레이어가 쏘는 투사체(총알)의 전체적인 움직임을 설정해주는 함수 PlayerBulletAction 함수 정의
void PlayerBulletAction()
	{
		int i; //반복문을 위한 변수 i를 선언

		for (i = 0; i < MAXBULLET; i++) //MAXBULLET만큼 반복문이 돈다. 
		{
			if (Bullet[i].exist == TRUE)
			{
				//EraseBullet() 함수 에 처음 1의 값이 들어간다. 1이 들어가게 되면 
				//EraseBullet함수 안에 Bullet[i].x 값과 y값이 매크로 쪽으로 넘어가게 된다.
				EraseBullet(i); 
				if (Bullet[i].y == 0) // 존재하는 총알마다 y좌표를 조사하여 0이면 총알을 제거 하도록 한다.
				{
					Bullet[i].exist = FALSE; //총알을 제거함.
				}
				else //만약 존재하는 총알이 0이 아니라면 총알이 계속 올라가도록 만들어주는 조건문이다.
				{
					Bullet[i].y--; //총알의 y값이 --되며 올라간다.
					DrawBullet(i); //총알을 그려주는 함수 실행
					
				}
			}

		}
		
	}
//미사일을 그려주는 함수 DrawMissile() 정의
void DrawMissile()
	{
		SetColor(YELLOW); //미사일 색 정의
		gotoxy(mx, my - 2); printf("▲▲"); // 크기를 표현하기위해서 미사일의 y 값 보다 1만큼, 2만큼 위에서 보이도록 했다
		gotoxy(mx, my - 1); printf("■■"); // 미사일의 몸통부분 
		gotoxy(mx, my);     printf("■■"); // 미사일의 꼬리부분
	}
//미사일을 지워주는 함수 EraseMissile 정의
void EraseMissile()
	{
		gotoxy(mx, my - 2); printf("    "); // 똑같은 크기만큼 지워줘야 하기때문에 그려줄때와 마찬가지로 1만큼 2만큼 위까지 다 지워주도록 한다.
		gotoxy(mx, my - 1); printf("    "); 
		gotoxy(mx, my);     printf("    ");
	}
// 미사일이 날아가는 알고리즘을 정의한 PlayerMissileAction() 함수
// 미사일은 연사로 쓸수없게 하기위해서 배열로 설정하지 않았다.
void PlayerMissileAction()
	{
		if (mx != -1)  //만약 미사일의 x값이 -1과 같지 않다면 실행 -> 미사일이 발사 되었을때.
		{
			EraseMissile(); //미사일을 지워주는 함수 호출
			if (my - 2 == 0) //만약 미사일의 y값의 -2 가 0 즉 꼭대기 까지 올라갔다면 실행.
			{
				mx = -1; //미사일의 x값을 다시 -1로 설정. -> 다시 발사할수 있게 해주기 위해서
			}
			else
			{
				my--; //위의 경우가 아닌경우(발사된후 꼭대기 까지 올라가지 않았을경우) 미사일의 y값을 줄여 올라갈수있도록 해줍니다.
				DrawMissile(); //미사일 y의 값이 올라가서 미사일 그리기를 반복
			}
		}
	}
/*===============================================================*/
//적을 그려주는 DrawEnemy 함수를 정의
void DrawEnemy()
	{
		BOOL Found; // Found 값의 참 거짓을 판단.
		int i, j; 

		
			int dom = rand() % 89; //dom이라는 변수에 0 ~ 88의 랜덤한값 을 생성후 저장한다. 
			int movespeed = rand() % 4 + 2; //movespeed란 변수에 랜던한값 0~4 이지만 거기서 +2를 더해 값을 임의로 크게 저장
		

		if (dom == 1) //dom변수가 1이  됐을때 실행한다 . 즉 89/1 확률로 적이 생성 될수있도록 한다.
		{
			for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) { ; } //적군의 총 함선수를 제한 

			if (i != MAXENEMY) //i의 값이 MAXENEMY 와 같지않다면 실행 
			{
				if (random(10) == 0) //확률적으로 적이 왼쪽에서 나올지 오른쪽에서 나올지를 결정해준다.
				{
					Enemy[i].x = 5; //적군의 함선이 나올 위치
					Enemy[i].dest = 1; //dest 1이라면 왼쪽에서 오른쪽으로 이동
				}
				else
				{

					Enemy[i].x = 45; //적군의 함선이 나올 위치
					Enemy[i].dest = -1; //dest -1이라면 오른쪽에서 왼쪽으로 이동
				}
			
				while (1)
				{
					Enemy[i].y = random(10) + 1;
					for (Found = FALSE, j = 0; j < MAXENEMY; j++)
					{
						if (Enemy[i].exist == TRUE && Enemy[j].y == Enemy[i].y)
						{
							Found = TRUE;
							break;
						}
					}

					if (Found == FALSE)
						break;
				}
				Enemy[i].nFrame = Enemy[i].nStay = movespeed;  //적군이 오른쪽에서 왼쪽 또는 왼쪽에서 오른쪽으로 가는 속도 랜덤으로 결정
				Enemy[i].type = random(sizeof(arEnemy) / sizeof(arEnemy[0])); //적군의 함선 4개중 랜덤하게 소환하기위해 랜덤으로 가져옴.
				Enemy[i].exist = TRUE; //적군이 화면에 있는지 없는지를 판단.
			}
		}
	}
//적군의 움직임을 정의한 EnemyMoveAction() 함수
void EnemyMoveAction()
	{
		int i;

		for (i = 0; i < MAXENEMY; i++) 
		{
			if (Enemy[i].exist == FALSE) //적이 없다면 다시 반복문을 실행.
				continue;

			if (--Enemy[i].nStay == 0)
			{
				if (Enemy[i].type == -1) //Enemy[i].type = -1이라면 죽었다는것을 의미.  하지만 메모리 상에는 계속 남아있으므로 지워줘야한다.
				{
					
					gotoxy(Enemy[i].x - 3, Enemy[i].y);		printf("      ");
					gotoxy(Enemy[i].x - 3, Enemy[i].y - 1); printf("      ");
					gotoxy(Enemy[i].x - 3, Enemy[i].y - 2); printf("      ");
					Enemy[i].exist = FALSE;
					continue;
				}
				
				Enemy[i].nStay = Enemy[i].nFrame;

				if (Enemy[i].x > 47 || Enemy[i].x < 4) //적군이 47, 또는 4 -> 오른쪽, 왼쪽 에 생성되어 있을경우.
				{
					//적군을 지워준다
					Enemy[i].exist = FALSE;
					gotoxy(Enemy[i].x - 3, Enemy[i].y); 
					printf("          ");
				}
				else
				{
					Enemy[i].x += Enemy[i].dest;
					SetColor(RED); //색을 빨간색으로 지정
					gotoxy(Enemy[i].x - 3, Enemy[i].y);  //적 함선이 이동하는것을 구현.
					printf(arEnemy[Enemy[i].type]); //적함선을  화면에 찍어준다.

					EnemyBulletAction(i);// 후에 적 함선쪽에서 총알이 날아가는것을 구현
				}
			}
		}
	}

/*===============================================================*/
//적군의 총알 움직임의 알고리즘을 정의한 EnemyBulletAction() 함수
void EnemyBulletAction(int i)
	{
		int j;

		if (random(20) == 0)
		{
			for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) { ; } //MAXENEMYBULLET 만큼 실행이된다.
			if (j != MAXENEMYBULLET) //j의값과 MAXENEMYBULLET이 같다면 실행 X 다르다면 실행. -> 최대수를 제한하기위해.
			{
				EnemyBullet[j].x = Enemy[i].x + 4; //적군의 총알의 X좌표
				EnemyBullet[j].y = Enemy[i].y + 3; //적군의 총알의 Y좌표
				EnemyBullet[j].nFrame = EnemyBullet[j].nStay = Enemy[i].nFrame * 6; //적군 총알의 내려오는 속도 조정.
				EnemyBullet[j].exist = TRUE; //적이 화면 상에 있다는것을 확인.
			}
		}
	}
//적 총알을 그려주는 함수 DrawEnemyBullet() 정의 플레이어 총알과 같은 알고리즘이다.
void DrawEnemyBullet(int i)
	{
		SetColor(RED);// 색은 빨간색으로 지정
		gotoxy(EnemyBullet[i].x, EnemyBullet[i].y); //총알의 좌표값이 위에서 아래로가도록 만들어준다.
		printf("*"); 
	}
//적 총알을 지워주는 함수
void EraseEnemyBullet(int i)
	{
		gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
		printf(" ");
	}
/*===============================================================*/
//실제 적의 총알이 움직이는 알고리즘을 정의한 MoveEnemyBullet() 함수
void MoveEnemyBullet()
	{
		int i;

			for (i = 0; i < MAXENEMYBULLET; i++) { //적군의 MAXENEMYBULLET만큼의 총알이 나오도록 한다.
				if (EnemyBullet[i].exist == FALSE)  //화면에 총알이없다면 다시 찍어줄수있도록 함.
					continue;

				if (--EnemyBullet[i].nStay == 0)
				{
					EnemyBullet[i].nStay = EnemyBullet[i].nFrame;
					EraseEnemyBullet(i); //적군의 총알을 따라가면서 지워줌

					if (EnemyBullet[i].y > 38) //적군의 총알이 화면밖으로 사라졌을시.
					{
						EnemyBullet[i].exist = FALSE; //총알이 화면안에 없다는것을 표현
					}
					else //그것이아니라 적군의 총알이 화면에 아직 있을경우
					{
						if (random(9)) //총알이 플레이어에게 다가오도록 설정해줌.
						{
							if (EnemyBullet[i].x < px) //플레이어의 위치와 총알의 위치를 비교해서 맞춰줌
								EnemyBullet[i].x++;  //플레이어 x보다 작다면 x를 증가 시켜줌
							else
								EnemyBullet[i].x--; //그것이 아니라면 총알의 x값을 감소 시킴으로서 플레이어의 함선을 따라가도록 만들었다.
						}
						EnemyBullet[i].y++; //총알이 계속 아래로 내려가도록 y값 증가
						DrawEnemyBullet(i); //총알을 그려준다.
					}

				}
			}

	}
/*===============================================================*/

void DrawEnemy2()
{
	BOOL Found; // Found 값의 참 거짓을 판단.
	int i, j;


	int dom = rand() % 89; //dom이라는 변수에 0 ~ 88의 랜덤한값 을 생성후 저장한다. 
	int movespeed = rand() % 1 + 2; //movespeed란 변수에 랜던한값 0~4 이지만 거기서 +2를 더해 값을 임의로 크게 저장


	if (dom == 1) //dom변수가 1이  됐을때 실행한다 . 즉 89/1 확률로 적이 생성 될수있도록 한다.
	{
		for (i = 0; i < MAXENEMY2 && Enemy2[i].exist == TRUE; i++) { ; } //적군의 총 함선수를 제한 

		if (i != MAXENEMY2) //i의 값이 MAXENEMY 와 같지않다면 실행 
		{
			if (random(10) == 0) //확률적으로 적이 왼쪽에서 나올지 오른쪽에서 나올지를 결정해준다.
			{
				Enemy2[i].x = 5; //적군의 함선이 나올 위치
				Enemy2[i].dest = 1; //dest 1이라면 왼쪽에서 오른쪽으로 이동
			}
			else
			{

				Enemy2[i].x = 45; //적군의 함선이 나올 위치
				Enemy2[i].dest = -1; //dest -1이라면 오른쪽에서 왼쪽으로 이동
			}

			while (1)
			{
				Enemy2[i].y = random(10) + 1;
				for (Found = FALSE, j = 0; j < MAXENEMY2; j++)
				{
					if (Enemy2[i].exist == TRUE && Enemy2[j].y == Enemy2[i].y)
					{
						Found = TRUE;
						break;
					}
				}

				if (Found == FALSE)
					break;
			}
			Enemy2[i].nFrame = Enemy2[i].nStay = movespeed;  //적군이 오른쪽에서 왼쪽 또는 왼쪽에서 오른쪽으로 가는 속도 랜덤으로 결정
			Enemy2[i].type = random(sizeof(arEnemy) / sizeof(arEnemy2[0])); //적군의 함선 4개중 랜덤하게 소환하기위해 랜덤으로 가져옴.
			Enemy2[i].exist = TRUE; //적군이 화면에 있는지 없는지를 판단.
		}
	}
}
//적군의 움직임을 정의한 EnemyMoveAction() 함수
void EnemyMoveAction2()
{
	int i;

	for (i = 0; i < MAXENEMY2; i++)
	{
		if (Enemy2[i].exist == FALSE) //적이 없다면 다시 반복문을 실행.
			continue;

		if (--Enemy2[i].nStay == 0)
		{
			if (Enemy2[i].type == -1) //Enemy[i].type = -1이라면 죽었다는것을 의미.  하지만 메모리 상에는 계속 남아있으므로 지워줘야한다.
			{

				gotoxy(Enemy2[i].x - 3, Enemy2[i].y);	  printf("      ");
				gotoxy(Enemy2[i].x - 3, Enemy2[i].y - 1); printf("      ");
				gotoxy(Enemy2[i].x - 3, Enemy2[i].y - 2); printf("      ");
				Enemy2[i].exist = FALSE;
				continue;
			}

			Enemy2[i].nStay = Enemy2[i].nFrame;

			if (Enemy2[i].x > 47 || Enemy2[i].x < 4) //적군이 47, 또는 4 -> 오른쪽, 왼쪽 에 생성되어 있을경우.
			{
				//적군을 지워준다
				Enemy2[i].exist = FALSE;
				gotoxy(Enemy2[i].x - 3, Enemy2[i].y);
				printf("          ");
			}
			else
			{
				Enemy2[i].x += Enemy2[i].dest;
				SetColor(RED); //색을 빨간색으로 지정
				gotoxy(Enemy2[i].x - 3, Enemy2[i].y);  //적 함선이 이동하는것을 구현.
				printf(arEnemy[Enemy2[i].type]); //적함선을  화면에 찍어준다.

				EnemyBulletAction2(i);// 후에 적 함선쪽에서 총알이 날아가는것을 구현
			}
		}
	}
}

/*===============================================================*/
void EnemyBulletAction2(int i)
{
	int j;

	if (random(20) == 0)
	{
		for (j = 0; j < MAXENEMYBULLET && EnemyBullet2[j].exist == TRUE; j++) { ; } //MAXENEMYBULLET 만큼 실행이된다.
		if (j != MAXENEMYBULLET) //j의값과 MAXENEMYBULLET이 같다면 실행 X 다르다면 실행. -> 최대수를 제한하기위해.
		{
			EnemyBullet2[j].x = Enemy[i].x + 4; //적군의 총알의 X좌표
			EnemyBullet2[j].y = Enemy[i].y + 3; //적군의 총알의 Y좌표
			EnemyBullet2[j].nFrame = EnemyBullet2[j].nStay = Enemy2[i].nFrame * 6; //적군 총알의 내려오는 속도 조정.
			EnemyBullet2[j].exist = TRUE; //적이 화면 상에 있다는것을 확인.
		}
	}
}
//적 총알을 그려주는 함수 DrawEnemyBullet() 정의 플레이어 총알과 같은 알고리즘이다.
void DrawEnemyBullet2(int i)
{
	SetColor(RED);// 색은 빨간색으로 지정
	gotoxy(EnemyBullet2[i].x, EnemyBullet2[i].y); //총알의 좌표값이 위에서 아래로가도록 만들어준다.
	printf("*");
}
//적 총알을 지워주는 함수
void EraseEnemyBullet2(int i)
{
	gotoxy(EnemyBullet2[i].x, EnemyBullet2[i].y);
	printf(" ");
}
/*===============================================================*/
//실제 적의 총알이 움직이는 알고리즘을 정의한 MoveEnemyBullet() 함수
void MoveEnemyBullet2()
{
	int i;

	for (i = 0; i < MAXENEMYBULLET2; i++) { //적군의 MAXENEMYBULLET만큼의 총알이 나오도록 한다.
		if (EnemyBullet2[i].exist == FALSE)  //화면에 총알이없다면 다시 찍어줄수있도록 함.
			continue;

		if (--EnemyBullet2[i].nStay == 0)
		{
			EnemyBullet2[i].nStay = EnemyBullet2[i].nFrame;
			EraseEnemyBullet2(i); //적군의 총알을 따라가면서 지워줌

			if (EnemyBullet2[i].y > 38) //적군의 총알이 화면밖으로 사라졌을시.
			{
				EnemyBullet2[i].exist = FALSE; //총알이 화면안에 없다는것을 표현
			}
			else //그것이아니라 적군의 총알이 화면에 아직 있을경우
			{
				if (random(9)) //총알이 플레이어에게 다가오도록 설정해줌.
				{
					if (EnemyBullet2[i].x < px) //플레이어의 위치와 총알의 위치를 비교해서 맞춰줌
						EnemyBullet2[i].x++;  //플레이어 x보다 작다면 x를 증가 시켜줌
					else
						EnemyBullet2[i].x--; //그것이 아니라면 총알의 x값을 감소 시킴으로서 플레이어의 함선을 따라가도록 만들었다.
				}
				EnemyBullet2[i].y++; //총알이 계속 아래로 내려가도록 y값 증가
				DrawEnemyBullet2(i); //총알을 그려준다.
			}

		}
	}

}


void DrawEnemy3()
{
	BOOL Found; // Found 값의 참 거짓을 판단.
	int i, j;


	int dom = rand() % 89; //dom이라는 변수에 0 ~ 88의 랜덤한값 을 생성후 저장한다. 
	int movespeed = rand() % 1 + 2; //movespeed란 변수에 랜던한값 0~4 이지만 거기서 +2를 더해 값을 임의로 크게 저장


	if (dom == 1) //dom변수가 1이  됐을때 실행한다 . 즉 89/1 확률로 적이 생성 될수있도록 한다.
	{
		for (i = 0; i < MAXENEMY3 && Enemy3[i].exist == TRUE; i++) { ; } //적군의 총 함선수를 제한 

		if (i != MAXENEMY3) //i의 값이 MAXENEMY 와 같지않다면 실행 
		{
			if (random(10) == 0) //확률적으로 적이 왼쪽에서 나올지 오른쪽에서 나올지를 결정해준다.
			{
				Enemy3[i].x = 5; //적군의 함선이 나올 위치
				Enemy3[i].dest = 1; //dest 1이라면 왼쪽에서 오른쪽으로 이동
			}
			else
			{

				Enemy3[i].x = 45; //적군의 함선이 나올 위치
				Enemy3[i].dest = -1; //dest -1이라면 오른쪽에서 왼쪽으로 이동
			}

			while (1)
			{
				Enemy3[i].y = random(10) + 1;
				for (Found = FALSE, j = 0; j < MAXENEMY3; j++)
				{
					if (Enemy3[i].exist == TRUE && Enemy3[j].y == Enemy3[i].y)
					{
						Found = TRUE;
						break;
					}
				}

				if (Found == FALSE)
					break;
			}
			Enemy3[i].nFrame = Enemy3[i].nStay = movespeed;  //적군이 오른쪽에서 왼쪽 또는 왼쪽에서 오른쪽으로 가는 속도 랜덤으로 결정
			Enemy3[i].type = random(sizeof(arEnemy) / sizeof(arEnemy[0])); //적군의 함선 4개중 랜덤하게 소환하기위해 랜덤으로 가져옴.
			Enemy3[i].exist = TRUE; //적군이 화면에 있는지 없는지를 판단.
		}
	}
}
//적군의 움직임을 정의한 EnemyMoveAction() 함수
void EnemyMoveAction3()
{
	int i;

	for (i = 0; i < MAXENEMY3; i++)
	{
		if (Enemy3[i].exist == FALSE) //적이 없다면 다시 반복문을 실행.
			continue;

		if (--Enemy3[i].nStay == 0)
		{
			if (Enemy3[i].type == -1) //Enemy[i].type = -1이라면 죽었다는것을 의미.  하지만 메모리 상에는 계속 남아있으므로 지워줘야한다.
			{

				gotoxy(Enemy3[i].x - 3, Enemy3[i].y);		printf("      ");
				gotoxy(Enemy3[i].x - 3, Enemy3[i].y - 1); printf("      ");
				gotoxy(Enemy3[i].x - 3, Enemy3[i].y - 2); printf("      ");
				Enemy3[i].exist = FALSE;
				continue;
			}

			Enemy3[i].nStay = Enemy3[i].nFrame;

			if (Enemy3[i].x > 47 || Enemy3[i].x < 4) //적군이 47, 또는 4 -> 오른쪽, 왼쪽 에 생성되어 있을경우.
			{
				//적군을 지워준다
				Enemy3[i].exist = FALSE;
				gotoxy(Enemy3[i].x - 3, Enemy3[i].y);
				printf("          ");
			}
			else
			{
				Enemy3[i].x += Enemy3[i].dest;
				SetColor(RED); //색을 빨간색으로 지정
				gotoxy(Enemy3[i].x - 3, Enemy3[i].y);  //적 함선이 이동하는것을 구현.
				printf(arEnemy[Enemy3[i].type]); //적함선을  화면에 찍어준다.

				EnemyBulletAction3(i);// 후에 적 함선쪽에서 총알이 날아가는것을 구현
			}
		}
	}
}

/*===============================================================*/
//적군의 총알 움직임의 알고리즘을 정의한 EnemyBulletAction() 함수
void EnemyBulletAction3(int i)
{
	int j;

	if (random(20) == 0)
	{
		for (j = 0; j < MAXENEMYBULLET3 && EnemyBullet3[j].exist == TRUE; j++) { ; } //MAXENEMYBULLET 만큼 실행이된다.
		if (j != MAXENEMYBULLET3) //j의값과 MAXENEMYBULLET이 같다면 실행 X 다르다면 실행. -> 최대수를 제한하기위해.
		{
			EnemyBullet3[j].x = Enemy3[i].x + 4; //적군의 총알의 X좌표
			EnemyBullet3[j].y = Enemy3[i].y + 3; //적군의 총알의 Y좌표
			EnemyBullet3[j].nFrame = EnemyBullet3[j].nStay = Enemy3[i].nFrame * 6; //적군 총알의 내려오는 속도 조정.
			EnemyBullet3[j].exist = TRUE; //적이 화면 상에 있다는것을 확인.
		}
	}
}
//적 총알을 그려주는 함수 DrawEnemyBullet() 정의 플레이어 총알과 같은 알고리즘이다.
void DrawEnemyBullet3(int i)
{
	SetColor(RED);// 색은 빨간색으로 지정
	gotoxy(EnemyBullet3[i].x, EnemyBullet3[i].y); //총알의 좌표값이 위에서 아래로가도록 만들어준다.
	printf("*");
}
//적 총알을 지워주는 함수
void EraseEnemyBullet3(int i)
{
	gotoxy(EnemyBullet3[i].x, EnemyBullet3[i].y);
	printf(" ");
}
/*===============================================================*/
//실제 적의 총알이 움직이는 알고리즘을 정의한 MoveEnemyBullet() 함수
void MoveEnemyBullet3()
{
	int i;

	for (i = 0; i < MAXENEMYBULLET3; i++) { //적군의 MAXENEMYBULLET만큼의 총알이 나오도록 한다.
		if (EnemyBullet3[i].exist == FALSE)  //화면에 총알이없다면 다시 찍어줄수있도록 함.
			continue;

		if (--EnemyBullet3[i].nStay == 0)
		{
			EnemyBullet3[i].nStay = EnemyBullet3[i].nFrame;
			EraseEnemyBullet3(i); //적군의 총알을 따라가면서 지워줌

			if (EnemyBullet3[i].y > 38) //적군의 총알이 화면밖으로 사라졌을시.
			{
				EnemyBullet3[i].exist = FALSE; //총알이 화면안에 없다는것을 표현
			}
			else //그것이아니라 적군의 총알이 화면에 아직 있을경우
			{
				if (random(9)) //총알이 플레이어에게 다가오도록 설정해줌.
				{
					if (EnemyBullet3[i].x < px) //플레이어의 위치와 총알의 위치를 비교해서 맞춰줌
						EnemyBullet3[i].x++;  //플레이어 x보다 작다면 x를 증가 시켜줌
					else
						EnemyBullet3[i].x--; //그것이 아니라면 총알의 x값을 감소 시킴으로서 플레이어의 함선을 따라가도록 만들었다.
				}
				EnemyBullet3[i].y++; //총알이 계속 아래로 내려가도록 y값 증가
				DrawEnemyBullet3(i); //총알을 그려준다.
			}

		}
	}

}
/*===============================================================*/






/*===============================================================*/
void CursorView(char show)//커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
// 적군의 충돌을 판정해주는 함수 EnemyCrash() 정의
void EnemyCrash()
{
	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1) //적 함선이 파괴 되거나 화면에 없을경우 반복문 다시 실행.
			continue;

		if (Enemy[i].y == Bullet[i].y && abs(Enemy[i].x - Bullet[i].x) <= 7) //y는 0~40까지 존재하지만 x는 음수의 값도있기 때문에 abs = 절대값을 사용, x와 y의 값이 일치하는지를 판단.
		{
			gotoxy(Bullet[i].x, Bullet[i].y); printf("         "); //바로 적군의 기체를 지운다.
			Bullet[i].exist = FALSE; //적군 총알을 화면에서 지운다
			Enemy[i].type = -1; //적군 함선이 맞았다는 의미.

			SetColor(GREEN); //+10이라는 글자는 초록색으로 표현한다
			gotoxy(Enemy[i].x - 3, Enemy[i].y); //적군의 기체가 죽은 자리에 생성되도록 설정
			printf("+10점!"); //+10점 출력
			Score += 10; //+10출력후 Score 라는 지역변수에 10을 더해준다.
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}
		//미사일에 맞았을 경우에도 처리를 해주어야 하기때문에 위와 동일하지만 플레이어의 Bullet이 아닌 mx로 대체된다.
		if (Enemy[i].y == my && abs(Enemy[i].x - mx) <= 7) 
		{
			gotoxy(mx, my); printf("       ");
			mx = -1;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x - 3, Enemy[i].y);
			printf("+10점!!!");
			Score += 10;
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}
	}
}
//플레이어의 충돌을 판정해주는 알고리즘 PlayerCrash() 함수 정의.
void PlayerCrash() {
	int i;

	for (i = 0; i < MAXENEMYBULLET; i++) //MAXENEMYBULLET만큼 반복
	{
		if (EnemyBullet[i].exist == FALSE) //적 총알이 화면에 없을때 계속실행. 
			continue;
		//적군의 총알이 플레이어가 있는 y좌표에 내려오고, x좌표 값 까지 일치할때 실행한다.
		if (EnemyBullet[i].y == 39 && abs(EnemyBullet[i].x - px) <= 5) 
		{
			SetColor(RED); //빨간색으로 충돌 색 설정
			gotoxy(px - 3, 37); printf("@bang@"); //플레이어의 x좌표값, 즉 플레이어가 있는곳에서 printf해준다.
			gotoxy(px - 3, 38); printf("@@@@@@");
			gotoxy(px - 3, 39); printf("@bang@");
			delay(2000); //플레이어의 함선이 터진후 조금 대기
			system("cls"); //화면을 지워준다
			gotoxy(35, 12); printf("게임오버"); //그 후 게임오버 라는 문장출력.
			gotoxy(35, 15); printf("Your Score! = %d\n", Score);
			delay(1000); // 또다시 잠시 대기
			exit(0); //후 프로그램을 종료한다.
		}
	}
}

void EnemyCrash2()
{
	int i;

	for (i = 0; i < MAXENEMY2; i++) {
		if (Enemy2[i].exist == FALSE || Enemy2[i].type == -1) //적 함선이 파괴 되거나 화면에 없을경우 반복문 다시 실행.
			continue;

		if (Enemy2[i].y == Bullet[i].y && abs(Enemy2[i].x - Bullet[i].x) <= 7) //y는 0~40까지 존재하지만 x는 음수의 값도있기 때문에 abs = 절대값을 사용, x와 y의 값이 일치하는지를 판단.
		{
			gotoxy(Bullet[i].x, Bullet[i].y); printf("         "); //바로 적군의 기체를 지운다.
			Bullet[i].exist = FALSE; //적군 총알을 화면에서 지운다
			Enemy2[i].type = -1; //적군 함선이 맞았다는 의미.

			SetColor(GREEN); //+10이라는 글자는 초록색으로 표현한다
			gotoxy(Enemy2[i].x - 3, Enemy2[i].y); //적군의 기체가 죽은 자리에 생성되도록 설정
			printf("+10점!"); //+10점 출력
			Score += 10; //+10출력후 Score 라는 지역변수에 10을 더해준다.
			Enemy2[i].nFrame = Enemy2[i].nStay = 50;
			break;
		}
		//미사일에 맞았을 경우에도 처리를 해주어야 하기때문에 위와 동일하지만 플레이어의 Bullet이 아닌 mx로 대체된다.
		if (Enemy[i].y == my && abs(Enemy[i].x - mx) <= 7)
		{
			gotoxy(mx, my); printf("       ");
			mx = -1;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x - 3, Enemy[i].y);
			printf("+10점!!!");
			Score += 10;
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}
	}
}
//플레이어의 충돌을 판정해주는 알고리즘 PlayerCrash() 함수 정의.
void PlayerCrash2() {
	int i;

	for (i = 0; i < MAXENEMYBULLET2; i++) //MAXENEMYBULLET만큼 반복
	{
		if (EnemyBullet2[i].exist == FALSE) //적 총알이 화면에 없을때 계속실행. 
			continue;
		//적군의 총알이 플레이어가 있는 y좌표에 내려오고, x좌표 값 까지 일치할때 실행한다.
		if (EnemyBullet2[i].y == 39 && abs(EnemyBullet2[i].x - px) <= 5)
		{
			SetColor(RED); //빨간색으로 충돌 색 설정
			gotoxy(px - 3, 37); printf("@bang@"); //플레이어의 x좌표값, 즉 플레이어가 있는곳에서 printf해준다.
			gotoxy(px - 3, 38); printf("@@@@@@");
			gotoxy(px - 3, 39); printf("@bang@");
			delay(2000); //플레이어의 함선이 터진후 조금 대기
			system("cls"); //화면을 지워준다
			gotoxy(35, 12); printf("Game Over\n"); //그 후 게임오버 라는 문장출력.
			gotoxy(35, 15); printf("Your Score! = %d\n", Score);
			delay(1000); // 또다시 잠시 대기
			exit(0); //후 프로그램을 종료한다.
		}
	}
}

void EnemyCrash3()
{
	int i;

	for (i = 0; i < MAXENEMY3; i++) {
		if (Enemy3[i].exist == FALSE || Enemy3[i].type == -1) //적 함선이 파괴 되거나 화면에 없을경우 반복문 다시 실행.
			continue;

		if (Enemy3[i].y == Bullet[i].y && abs(Enemy3[i].x - Bullet[i].x) <= 7) //y는 0~40까지 존재하지만 x는 음수의 값도있기 때문에 abs = 절대값을 사용, x와 y의 값이 일치하는지를 판단.
		{
			gotoxy(Bullet[i].x, Bullet[i].y); printf("         "); //바로 적군의 기체를 지운다.
			Bullet[i].exist = FALSE; //적군 총알을 화면에서 지운다
			Enemy3[i].type = -1; //적군 함선이 맞았다는 의미.

			SetColor(GREEN); //+10이라는 글자는 초록색으로 표현한다
			gotoxy(Enemy3[i].x - 3, Enemy3[i].y); //적군의 기체가 죽은 자리에 생성되도록 설정
			printf("+10점!"); //+10점 출력
			Score += 10; //+10출력후 Score 라는 지역변수에 10을 더해준다.
			Enemy3[i].nFrame = Enemy3[i].nStay = 50;
			break;
		}
		//미사일에 맞았을 경우에도 처리를 해주어야 하기때문에 위와 동일하지만 플레이어의 Bullet이 아닌 mx로 대체된다.
		if (Enemy[i].y == my && abs(Enemy[i].x - mx) <= 7)
		{
			gotoxy(mx, my); printf("       ");
			mx = -1;
			Enemy[i].type = -1;

			SetColor(GREEN);
			gotoxy(Enemy[i].x - 3, Enemy[i].y);
			printf("+10점!!!");
			Score += 10;
			Enemy[i].nFrame = Enemy[i].nStay = 50;
			break;
		}
	}
}
//플레이어의 충돌을 판정해주는 알고리즘 PlayerCrash() 함수 정의.
void PlayerCrash3() {
	int i;

	for (i = 0; i < MAXENEMYBULLET3; i++) //MAXENEMYBULLET만큼 반복
	{
		if (EnemyBullet3[i].exist == FALSE) //적 총알이 화면에 없을때 계속실행. 
			continue;
		//적군의 총알이 플레이어가 있는 y좌표에 내려오고, x좌표 값 까지 일치할때 실행한다.
		if (EnemyBullet3[i].y == 39 && abs(EnemyBullet3[i].x - px) <= 5)
		{
			SetColor(RED); //빨간색으로 충돌 색 설정
			gotoxy(px - 3, 37); printf("@bang@"); //플레이어의 x좌표값, 즉 플레이어가 있는곳에서 printf해준다.
			gotoxy(px - 3, 38); printf("@@@@@@");
			gotoxy(px - 3, 39); printf("@bang@");
			delay(2000); //플레이어의 함선이 터진후 조금 대기
			system("cls"); //화면을 지워준다
			gotoxy(35, 12); printf("Game Over\n"); //그 후 게임오버 라는 문장출력.
			gotoxy(35, 15); printf("Your Score! = %d\n", Score);
			delay(1000); // 또다시 잠시 대기
			exit(0); //후 프로그램을 종료한다.
		}
	}
}

//INVADER게임 플레이중 화면 오른쪽에 기본 인터페이스와 설명을 그려주는 DrawInfo() 함수
int DrawInfo() {
	//원하는 좌표에 적고싶은 문장을 printf로 출력.■
	SetColor(WHITE);
	//gotoxy(58, 4); printf(" I N V A D E R ");
	SetColor(SKYBLUE);
	gotoxy(58, 3); printf("=====================================================");
	gotoxy(58, 4); printf(" ■  ■    ■  ■  ■    ■    ■■    ■■■  ■■  ");
	gotoxy(58, 5); printf(" ■  ■■  ■  ■  ■  ■  ■  ■  ■  ■      ■  ■");
	gotoxy(58, 6); printf(" ■  ■  ■■  ■  ■  ■■■  ■  ■  ■      ■■  ");
	gotoxy(58, 7); printf(" ■  ■    ■    ■    ■  ■  ■■    ■■■  ■  ■");
	SetColor(VIOLET);
	gotoxy(58, 8); printf("=====================================================");
	SetColor(WHITE);
	gotoxy(62, 20); printf("점수 : %d", Score);

	gotoxy(60, 30); printf("LEFT <- -> RIGHT");
	gotoxy(60, 35); printf("총  : 스페이스바");
	gotoxy(60, 37); printf("미사일  : D");

	return Score;

}
																																																										