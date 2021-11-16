// speech-project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm>
//#include <unistd.h>
#include <sstream>
#include <windows.h>

#define CONVERGE_ITERATIONS 200
#define M 32 //Number of obsevation symbols per state
#define N 5 //Number of states
#define P 12
#define LIMIT 5000
#define CB_SIZE 32
#define PI 3.142857142857
#define FRAME_SIZE 320
#define FRAMES 100

const int WIN_SIZE  = (FRAME_SIZE * FRAMES);

int T; //Time sequence length
const int MAX_T = 150; // max time sequence length
using namespace std;

//Global variables
long double dcShift, nFactor, mx, silenceEnergy;
long double const threshold = 1e-30;   //Min threshold to be assigned to zero values in matrix B.
long int sSize = 0, sampSize = 0, enSize = 0;
long double max_pobs_model = 0;
int test_ans = 0, fake = 0;

//Globally defined arrays
int O[MAX_T+1];	//Observation sequence
int Q[MAX_T+1];	//state sequence.
long double pstar = 0, prev_p_star = -1;
int Psi[MAX_T+1][N+1]; 
long double Alpha[MAX_T+1][N+1];
long double Beta[MAX_T+1][N+1];
long double Gamma[MAX_T+1][N+1];
long double Delta[MAX_T+1][N+1];
long double Xi[MAX_T+1][N+1][N+1];

//codebook
long double codeBook[CB_SIZE][P];

//arrays for samples
long int sample[1000000];
long double steadySamp[WIN_SIZE];
long double energy[100000];
long double Ai[P+1], Ri[P+1], Ci[P+1];

//tokhura weights
double tokhuraWeights[]={1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

//Model parameters A, B and Pi
long double A[N+1][N+1] = {0};
long double B[N+1][M+1] = {0};
long double Pi[N+1] = {0};

long double Abar[N+1][N+1] = {0};
long double Bbar[N+1][M+1] = {0};
long double Pibar[N+1] = {0};

long double a_average[N+1][N+1] = {0};
long double b_average[N+1][M+1] = {0};
long double pi_average[N+1] = {0};

//files
char* A_file = "a_i_j.txt";
char* B_file = "b_i_j.txt";
char* PI_file = "pi.txt";

int cnt = 1, train = 0;
long double P_O_given_Model = 0;
ofstream uni, dump;
FILE *common_dump;
int environment_known = 0, is_live_testing = 0;
char keywords[][100] = {"calendar", "close", "down", "exit", "faculties", "github", "history", "jobs", "open", "outlook", "payments", "portal", "quora", "search", "up", "video"};


bool browser_open = false;
#define SCROLL_DOWN_VAL -700
#define SCROLL_UP_VAL 700
#define CONFIRM_BOX_YES 6
#define CONFIRM_BOX_NO 7
#define CONFIRM_BOX_CANCEL 2

static const WORD TAB_SCANCODE = 0x09;
static const WORD LEFT_ALT_SCANCODE = 0x12;
static const WORD LCTRL_SCANCODE = 0x1d;
static const WORD H_SCANCODE = 0x23;
static const WORD B_SCANCODE = 0x30;
static const WORD J_SCANCODE = 0x24;
static const WORD K_SCANCODE = 0x25;
static const WORD O_SCANCODE = 0x18;
static const WORD P_SCANCODE = 0x19;
static const WORD NUMPAD_5 = 0x4c;
static const WORD LSHIFT_SCANCODE = 0x2a;
static const DWORD keypress_delay_ms = 500;
static const WORD W_SCANCODE = 0x57;

bool next_prob_word[16];

void pressKey(bool down_or_up, WORD scanCode, bool extended = false){
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = scanCode;
	
	if(!down_or_up) input.ki.dwFlags |= KEYEVENTF_KEYUP;
	if(extended) input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;

	SendInput(1, &input, sizeof(input));
}

void up_key(WORD scanCode, bool extended = false){
	pressKey(false, scanCode, extended);
}

void down_key(WORD scanCode, bool extended = false){
	pressKey(true, scanCode, extended);
}

void alt_tab(int tab_count = 1){
	down_key(LEFT_ALT_SCANCODE);

	Sleep(keypress_delay_ms);
	for (int i = 0; i<tab_count; ++i) {
		down_key(TAB_SCANCODE);
		Sleep(keypress_delay_ms);
		up_key(TAB_SCANCODE);
		Sleep(keypress_delay_ms);
	}
	up_key(LEFT_ALT_SCANCODE);
}

void ctrl_h(){
	down_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	down_key(H_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(H_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

}

void ctrl_j(){
	down_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	down_key(J_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(J_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

}

void ctrl_k(){
	down_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	down_key(K_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(K_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

}

void ctrl_w(){
	down_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	down_key(W_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(W_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

}

void alt_p(){
	down_key(LEFT_ALT_SCANCODE);
	Sleep(keypress_delay_ms);
	down_key(P_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(P_SCANCODE);
	Sleep(keypress_delay_ms);
	up_key(LEFT_ALT_SCANCODE);
	Sleep(keypress_delay_ms);
}

void ctrl_shift_o(){
	down_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

	down_key(LSHIFT_SCANCODE);
	Sleep(keypress_delay_ms);

	down_key(O_SCANCODE);
	Sleep(keypress_delay_ms);

	up_key(O_SCANCODE);
	Sleep(keypress_delay_ms);

	up_key(LSHIFT_SCANCODE);
	Sleep(keypress_delay_ms);

	up_key(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
}

void show_menu(){
	printf("OPEN - to open the browser\nHISTORY - to see the history\nClose - to close the current tab\n");
	printf("UP - to scroll up\nDOWN - to scroll down\nCALENDAR - to see institute calendar\nQUORA - to see the quora page\n");
	printf("GITHUB - to see the GitHub\nJOBS - to see the jobs page\nPORTAL - to see the placement portal of IITG\n");
	printf("SEARCH - to search on google\nPayments - to open the fees payment page of IITG\nVIDEO - to search the video on youtube\n");
	printf("FACULTIES - to see the faculty page\nOUTLOOK - to open the outlook\nEXIT - to exit the browser\n");
	printf("Speak what you want to do - \n");
}

void close_browser(){
	system("taskkill /f /im firefox.exe");
}

void close_tab(){
	ctrl_w();
}

void open_faculty_page(){
	system("start https://iitg.ac.in/iitg_faculty_all");
}

void open_outlook(){
	system("start https://outlook.office.com/mail/");
}

void search_video(){
	char video_search[] = "https://www.youtube.com/results?search_query=";
	char input[100];
	printf("Enter the video name - ");
	scanf("%s", input);
	strcat(video_search, input);
	char command[100] = "start ";
	strcat(command, video_search);
	system(command);
}

void open_payments(){
	// if(browser_open)
		system("start https://online.iitg.ac.in/epay/");
	// else {
	// 	open_browser();
	// 	system("start https://online.iitg.ac.in/epay/");
	// }
}

void search_google(){
	//query replace space with +
	char  search_query[] = "https://www.google.com/search?q=";
	char qinput[100];
	printf("Enter the search query - ");
	scanf("%s", qinput);
	strcat(search_query, qinput);
	char command[100] = "start ";
	strcat(command, search_query);
	system(command);
}

void open_quora(){
	system("start https://www.quora.com/");
}

void open_portal(){
	system("start https://swc.iitg.ac.in/placement-portal/");
}

void open_jobs(){
	printf("Opening jobs page\n");
}

void open_github(){
	system("start https://github.com");
}

void go_to_browser(){
	Sleep(keypress_delay_ms);
	alt_tab(2);
}

void open_calendar(){
	system("start https://iitg.ac.in/acad/academic_calender.php");
}

void open_history(){
	ctrl_h();
}

void scrolling_down(){
	go_to_browser();
	Sleep(500);
	mouse_event(MOUSEEVENTF_WHEEL, 0, 0, SCROLL_DOWN_VAL, 0);
}

void scrolling_up(){
	go_to_browser();
	Sleep(500);
	mouse_event(MOUSEEVENTF_WHEEL, 0, 0, SCROLL_UP_VAL, 0);
}

void open_browser(){
	system("start firefox.exe");
}

int is_substring(char *a, char *b){
	int m = sizeof(a)/sizeof(a[0]);
	int n = sizeof(b)/sizeof(b[0]);

	for(int i=0; i<=n-m; i++){
		int j;
		for(j=0; j<m; j++){
			if(b[i+j] != a[j])
				break;
		}
		if(j == m)
			return 1;
	}
	return -1;
}

char window_title[100];
HWND hwnd;

//perform the operation according to detected operation
void perform(int operation){
	int t0, t1, t2, t3, t6, t14;
	int i;

	if(system(NULL)){
		printf("Performing operation\n");
	}else {
		printf("Error in system call\n");
		exit(1);
	}

	switch (operation){
		case 0:
			{
				open_calendar();
				for(int i = 0; i<16; i++){
					if(i == 3 || i == 14 || i==0){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 1:
			{
				for(int i=5; i>0; i--){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); 
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					printf("\n%s", window_title);

					if (is_substring("Firefox", window_title) != -1){
						break;
					}
				}
				close_tab();
				for(int i = 0; i<16; i++){
					next_prob_word[i] = 1;
				}
				break;
			}
		
		case 2:
			{
				for(int i=5; i>0; i--){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); 
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					printf("\n%s", window_title);

					if (is_substring("Firefox", window_title) != -1){
						break;
					}
				}
				scrolling_down();
				for(int i = 0; i<16; i++){
					if(i == 8){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 3:
			{
				t3 = MessageBox(NULL, TEXT("Do you want to close the browser?"), TEXT("msg"), MB_YESNOCANCEL);
				if(t3 == CONFIRM_BOX_YES){
					close_browser();
					for(int i = 0; i<16; i++){
						if(i == 3 || i == 14){
							next_prob_word[i] = 0;
						}
						else {
							next_prob_word[i] = 1;
						}
					}
				}
				break;
			}
		
		case 4:
			{
				open_faculty_page();
				for(int i = 0; i<16; i++){
					if(i == 8 || i == 14 || i==4){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 5:
			{
				open_github();
				for(int i = 0; i<16; i++){
					if(i == 8 || i == 14 || i==5){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}

				break;
			}
		
		case 6:
			{
				for(int i=5; i>0; i--){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); 
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					printf("\n%s", window_title);

					if (is_substring("Firefox", window_title) != -1){
						break;
					}
				}
				
				open_history();

				for(int i = 0; i<16; i++){
					if(i == 8 || i == 6){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}
		
		case 7:
			{
				open_jobs();
				for(int i = 0; i<16; i++){
					if(i == 8 || i==14 || i == 7){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}
			
		case 8:
			{
				open_browser();
				for(int i=0; i<16; i++){
					if(i == 8 || i == 14){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 9:
			{
				open_outlook();
				for(int i=0; i<16; i++){
					if( i == 9 || i == 8 || i==14){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}
		
		case 10:
			{
				open_payments();
				for(int i=0; i<16; i++){
					if( i==8 || i == 10 || i==14){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}
		
		case 11:
			{
				open_portal();
				for(int i = 0; i<16; i++){
					if(i == 8 || i==14 || i == 11){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 12:
			{
				open_quora();
				for(int i = 0; i<16; i++){
					if(i == 8 || i==14 || i == 12){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 13:
			{
				search_google();
				for(int i = 0; i<16; i++){
					if(i == 8 || i==14 || i == 13){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		case 14:
			{
				t14 = 5;
				for(int i=t14; t14>=0; t14--){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); // get handle of currently active window
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					if (is_substring("Firefox", window_title) != -1) break;
				}

				scrolling_up();
				
				for(int i=0; i<16; i++){
					if(i==0){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}
		
		case 15:
			{
				search_video();
				for(int i = 0; i<16; i++){
					if(i == 8 || i==14 || i == 15){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		default:
			{
				printf("Invalid operation\n");
				break;
			}
	}
}




////////////////////////////////////////Functions related to speech processing////////////////////////////////////////////////////////

//Calculation of alpha variable to find the solution of problem number 1.
void forward_procedure(){
	int i , j , t;
	long double sum ;
	int index = O[1];
	P_O_given_Model = 0;

	for(i=1;i<=N;i++){
		Alpha[1][i] = Pi[i]*B[i][index];
	}
	
	for (t = 1; t < T; t++){
		for (j = 1; j <= N; j++){
			sum = 0;
			for (i = 1; i <= N; i++){
				sum += Alpha[t][i] * A[i][j];
			}
			Alpha[t + 1][j] = sum * B[j][O[t + 1]];
		}
	}
	for(i=1;i<=N;i++){
		P_O_given_Model = P_O_given_Model + Alpha[T][i];
	}
}

//Calculation of alpha variable to find the solution of problem number 1.
void forward_procedure(int iteration, FILE *fp = NULL){
	int i , j , t;
	long double sum ;
	int index = O[1];
	P_O_given_Model = 0;

	for(i=1;i<=N;i++){
		Alpha[1][i] = Pi[i]*B[i][index];
	}
	
	for (t = 1; t < T; t++){
		for (j = 1; j <= N; j++){
			sum = 0;
			for (i = 1; i <= N; i++){
				sum += Alpha[t][i] * A[i][j];
			}
			Alpha[t + 1][j] = sum * B[j][O[t + 1]];
		}
	}
	for(i=1;i<=N;i++){
		P_O_given_Model = P_O_given_Model + Alpha[T][i];
	}
	//finding where the model is matching
	if(P_O_given_Model > max_pobs_model){
		max_pobs_model = P_O_given_Model;
	 	test_ans = iteration;
	}

	cout << "Word : "<<keywords[iteration]<<"\tP(obs/model) : " << P_O_given_Model <<endl;
	if(fp != NULL){
		fprintf(fp, "---> Word %s ----- P(Obs/Model) : %g\n", keywords[iteration], P_O_given_Model);
		//cout << "Digit:"<<iteration<<"\tP(obs/model) : " << P_O_given_Model <<endl;
	}
}

//function for testing with iteration as argument
void solution_to_prob1(int iteration, FILE *fp = NULL){
	if(fp == NULL)
		forward_procedure(iteration);
	else
		forward_procedure(iteration, fp);
}

//Calculation of Beta variable.
void backward_procedure(){
	int i , j , t;
	long double sum;
	int index = 0;
	for(i=1;i<=N;i++){
		Beta[T][i] = 1.0;
	}
	for(t=T-1;t>=1;t--){
		index = O[t+1];
		for(i=1;i<=N;i++){
			sum = 0;
			for(j=1;j<=N;j++){
				sum = sum + B[j][index]*A[i][j]*Beta[t+1][j];
			}
			Beta[t][i]=sum;
		}
	}
}

//calculating alpha and beta values
void calculate_gamma(){
	for(int t=1;t<=T;t++){
		for(int j=1;j<=N;j++){
			long double summation=0;
			for(int k=1;k<=N;k++){
				summation += Alpha[t][k] * Beta[t][k];
			}
			Gamma[t][j]=(Alpha[t][j] * Beta[t][j])/summation;
		}
	}
}

//loading the model parameters with new calculated values
void load_calculated_model(){
	int i, j;
	for(i=1;i<=N;i++){
		Pi[i]=Pibar[i];
	}
	
	for(i=1;i<=N;i++){
		for(j=1;j<=N;j++){
			A[i][j]= Abar[i][j];
		}
	}
	for(i=1;i<=N;i++){
		for(j=1;j<=M;j++){
			B[i][j] = Bbar[i][j];
		}
	}
}

void reevaluate_model_parameters(){
	int i, j, k, t;
	long double sum1=0 , sum2 =0;
	//Re-evaluating Pi
	for(i=1;i<=N;i++){
		Pibar[i] = Gamma[1][i];
	}
	
	for(i = 1; i<=N; i++){
		for(j = 1; j <= N; j++){
			long double t1 = 0, t2 = 0;
			for(t = 1; t <= T-1; t++){
				t1 += Xi[t][i][j];
				t2 += Gamma[t][i];
				//cout<<"Xi["<<t<<"]["<<i<<"]["<<j<<"]: "<<Xi[t][i][j]<<", Gamma["<<t<<"]["<<j<<"]: "<<Gamma[t][j]<<endl;
			}
			//cout<<"t1 "<<t1<<" t2: "<<t2<<endl;
			//cout<<"t1/t2: "<<t1/t2<<endl;
			//system("pause");
			Abar[i][j] = t1/t2;
		}
	}
	//Re-evaluating B
	for(j=1;j<=N;j++){
		int count=0;
		long double max=0;
		int ind_j=0, ind_k=0;
		
		for(k=1;k<=M;k++){
			sum1 =0 , sum2 =0;
			for(t=1;t<T;t++){
				sum1 = sum1 + Gamma[t][j];
				if(O[t]==k){
					sum2 = sum2 + Gamma[t][j];				
				}
			}
			Bbar[j][k] = sum2/sum1;
			
			//finding max
			if(Bbar[j][k]>max){
				max=Bbar[j][k];
				ind_j = j;
				ind_k = k;
			}
			
			//updating new bij with threshold value if it is zero
			if(Bbar[j][k] == 0){
				Bbar[j][k]=threshold;
				count++;
			}
		}
		Bbar[ind_j][ind_k] = max - count*threshold;
	}
	//loading the new model
	load_calculated_model();
}

//Adjusting Model Parameters
void calculate_xi(){

	int i , j , t , index;
	long double summation[FRAMES + 1];

	for(t=1;t<=T;t++){
		// index = ;
		summation[t] = 0;
		for(i=1;i<=N;i++){
			for(j=1;j<=N;j++){
				summation[t] += Alpha[t][i]*A[i][j]*B[j][O[t+1]]*Beta[t+1][j];
			}
		}

		for(i=1;i<=N;i++){
			long double x;
			for(j=1;j<=N;j++){
				x = Alpha[t][i]*A[i][j]*B[j][O[t+1]]*Beta[t+1][j];
				Xi[t][i][j]= x/summation[t];
			}
		}
	}
}

//viterbi algorithm
void viterbi(){
	//initialization
    for(int i=1; i<=N; i++){
        Delta[1][i] = Pi[i] * B[i][O[1]];
        Psi[1][i] = 0;
    }

	//induction
	for(int j=1; j<=N; j++){
		for(int t=2; t<=T; t++){
            long double max = 0, ti = 0;
            int ind = 0;
            
            for(int i=1; i<=N; i++){
                ti = Delta[t-1][i] * A[i][j];
                if(ti > max){
					max = ti;
					ind = i;
				}
            }

            Delta[t][j] = max * B[j][O[t]];
			Psi[t][j] = ind;
        }
    }

	//termination
    long double max = 0;
    for(int i=1; i<=N; i++){
        if(Delta[T][i] > max) {
			max = Delta[T][i];
			Q[T] = i;
		}

        pstar = max;
    }

	//backtracking
    for(int t = T-1; t>0; t--){
        Q[t] = Psi[t+1][Q[t+1]];
    }
}

//writing updated A matrix to file
void write_final_A_matrix(FILE *fp){
	int i, j;
	fprintf(fp, "---------------A Matrix----------------\n");
	for (i = 1; i <= N; i++){
		for (j = 1; j <= N; j++){
			fprintf(fp,"%Le   ",A[i][j]);
		}
		fprintf(fp,"\n");
	}
}

//writing updated B matrix to file
void write_final_B_matrix(FILE *fp){
	int i, j;
	fprintf(fp, "---------------B Matrix----------------\n");
	for (i = 1; i <= N; i++){
		for (j = 1; j <= M; j++){
			fprintf(fp, "%Le   ", B[i][j]);
		}
		fprintf(fp, "\n");
	}
}

//writing updated pi values to file
void write_final_pi_matrix(FILE *fp){
	fprintf(fp, "---------------Pi values----------------\n");
	int i, j;
	for (i = 1; i <= N; i++){
		fprintf(fp, "%Le   ", Pi[i]);
	}
}

//dump the model
void dump_converged_model(FILE *fp){
	write_final_A_matrix(fp);
	write_final_B_matrix(fp);
	write_final_pi_matrix(fp);
}

//read A
bool readA(char *filename){
    fstream fin;
	fin.open(filename);    

    //file does not exist
	if(!fin){
		cout<<"Couldn't open file: "<<filename<<"\n";
		return false;
	}
	long double word;

    int row = 1, col = 1;
    //until input is available
	while(fin >> word){
        col = 1;
        A[row][col++] = word;

        for(int i=2; i<=N; i++){
            fin>>word;
            A[row][col++] = word;
        }
        row++;
    }

	fin.close();
	return true;
}

//read B
bool readB(string filename){
	fstream fin;
	fin.open(filename);    

    //file does not exist
	if(!fin){
		cout<<"Couldn't open file: "<<filename<<"\n";
		return false;
	}
	long double words;

    int row = 1, col = 1;

	while(fin>>words){
		col = 1;
		B[row][col++] = words;

		for(int i=1; i<M; i++){
			fin>>words;
			B[row][col++] = words;
		}
		row++;
	}
	//cout<<"row: "<<row<<endl;
	fin.close();
	return true;
}

//read Pi
bool readPi(string filename){
	fstream fin;
	fin.open(filename);    

    //file does not exist
	if(!fin){
		cout<<"Couldn't open file: "<<filename<<"\n";
		return false;
	}
	long double word;

    int col = 1;
    //until input is available
	while(fin >> word){
		col = 1;
        Pi[col++] = word;

        //save whole row
		for(int i=1;i<N;i++){
			fin>>word;
            Pi[col++] = word;
		}
	}

	fin.close();
	return true;
}

// make the model values, average model values and bar model values -  0
void erase_all_model(){
	for(int i=1; i<=N; i++){
		for(int j=1; j<=N; j++){
			A[i][j] = 0;
			a_average[i][j] = 0;
			Abar[i][j] = 0;
		}
	}

	for(int i=1; i<=N; i++){
		for(int j=1; j<=M; j++){
			B[i][j] = 0;
			b_average[i][j] = 0;
			Bbar[i][j] = 0;
		}
	}

	for(int i=1; i<=N; i++){
		Pi[i] = 0;
		Pibar[i] = 0;
		pi_average[i] = 0;
	}
}

//erasing the current model
void erase_model(){
	for(int i=1; i<=N; i++){
		for(int j=1; j<=N; j++){
			A[i][j] = 0;
		}
	}

	for(int i=1; i<=N; i++){
		for(int j=1; j<=M; j++){
			B[i][j] = 0;
		}
	}

	for(int i=1; i<=N; i++){
		Pi[i] = 0;
	}
}

// erasing average model
void erase_avg_model(){
	for(int i=1; i<=N; i++){
		for(int j=1; j<=N; j++){
			a_average[i][j] = 0;
		}
	}

	for(int i=1; i<=N; i++){
		for(int j=1; j<=M; j++){
			b_average[i][j] = 0;
		}
	}

	for(int i=1; i<=N; i++){
		pi_average[i] = 0;
	}
}

//reading average model
void read_average_model(int digit){
	
	char filename[100];
	sprintf(filename, "output/avgmodels/word_%s_A.txt", keywords[digit]);
	readA(filename);

	sprintf(filename, "output/avgmodels/word_%s_B.txt", keywords[digit]);
	readB(filename);

	sprintf(filename, "output/avgmodels/word_%s_PI.txt", keywords[digit]);
	readPi(filename);
}

//initialize model according to parameters
void initialize_model(int digit, int seq, char *filename = "--"){
	char a_file[100], b_file[100], pi_file[100], obs_file[100];

	if(filename == "--"){
		readA(A_file);
		readB(B_file);
		readPi(PI_file);
	}else if(filename  == "avg")
	{
		read_average_model(digit);
		
	}
	// else if(filename == "init"){
	// 	sprintf(a_file, "validation/Digit %d/A_%d.txt", digit, digit);
	// 	sprintf(b_file, "validation/Digit %d/B_%d.txt", digit, digit);
	// 	sprintf(pi_file, "validation/Digit %d/pi_%d.txt", digit, digit);

	// 	readA(a_file);
	// 	readB(b_file);
	// 	readPi(pi_file);
	// z
}

//adding current model values to avg model
void add_to_avg_model(){
	int i, j;
	for (i = 1; i <= N; i++){
		for (j = 1; j <= N; j++){
			a_average[i][j] += A[i][j];
		}
	}
	for (i = 1; i <= N; i++){
		pi_average[i] += Pi[i];
	}
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= M; j++){
			b_average[i][j] += B[i][j];
		}
	}
}

//writing updated a values
void write_final_A_values(char filename[]){
	FILE *fp = fopen(filename, "w");
	int i, j;
	for (i = 1; i <= N; i++){
		for (j = 1; j <= N; j++){
			// out << a_i_j[i][j] << " ";
			fprintf(fp, "%Le   ", A[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

//writing updated b values
void write_final_B_values(char filename[]){
	ofstream out(filename);
	for(int i=1; i<=N; i++){
		for(int j=1; j<=M; j++){
			out<<B[i][j]<<"   ";
		}
		out<<endl;
	}
	out.close();
}

//writing updated pi values
void write_final_pi_values(char filename[]){
	FILE *fp = fopen(filename, "w");
	int i, j;
	for (i = 1; i <= N; i++){
		// out << pi[i] << " ";
		fprintf(fp, "%Le   ", Pi[i]);
	}
	fclose(fp);
}

// dump the final model in output/digit d/ folder wise
void dump_final_model(int seq, int digit){
	char index[10];
	char a_file_final[100], b_file_final[100], pi_file_final[100];

	sprintf(pi_file_final, "output/%s/model_PI_%d.txt", keywords[digit], seq);
	write_final_pi_values(pi_file_final);

	sprintf(a_file_final, "output/%s/model_A_%d.txt", keywords[digit], seq);
	write_final_A_values(a_file_final);

	sprintf(b_file_final, "output/%s/model_B_%d.txt", keywords[digit], seq);
	write_final_B_values(b_file_final);

	cout<<"word "<<keywords[digit]<<", sequence : "<<seq<<" model dumped successfully\n";
}

// taking average of the avg model
void average_of_avg_model(int total_iterations){
	int i, j;
	for (i = 1; i <= N; i++){
		pi_average[i] /= total_iterations;
	}
	for (i = 1; i <= N; i++){
		for (j = 1; j <= N; j++){
			a_average[i][j] /= total_iterations;

		}
	}
	for (i = 1; i <= N; i++){
		for (j = 1; j <= M; j++){
			b_average[i][j] /= total_iterations;
		}
	}
}

// dumping average model to file
void dump_avg_model(int digit){
	char a_file_avg[100], b_file_avg[100], pi_file_avg[100], ind[3];

	sprintf(a_file_avg, "output/avgmodels/word_%s_A.txt", keywords[digit]);
	FILE *fp = fopen(a_file_avg, "w");
	for(int i=1; i<=N; i++){
		for(int j=1; j<=N; j++){
			fprintf(fp, "%Le   ", a_average[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	
	sprintf(b_file_avg, "output/avgmodels/word_%s_B.txt", keywords[digit]);
	ofstream fout(b_file_avg);
	for(int i=1; i<=N; i++){
		for(int j=1; j<=M; j++){
			//fprintf(fp, "%Le   ", b_average[i][j]);
			fout<<b_average[i][j]<<"   ";
		}
		fout<<endl;
		//fprintf(fp, "\n");
	}
	fout.close();

	sprintf(pi_file_avg, "output/avgmodels/word_%s_PI.txt", keywords[digit]);
	fp = fopen(pi_file_avg, "w");
	for(int i=1; i<=N; i++){
		fprintf(fp, "%Le   ", pi_average[i]);
	}
	fclose(fp);
}

//finding dc shift
void get_DC_shift(){
	long int sample_count = 0;
	int cnt = 0;
    FILE *fp;
    char line[80];
	double cValue;
	double cEnergy = 0;

    //reading dc_shift.txt file
	if(is_live_testing == 0)
		fp = fopen("silence.txt", "r");
	else 
		fp = fopen("silence_file.txt", "r");
    
    if(fp == NULL){
        printf("Silence File not found\n");
        exit(1);
    }
    
	dcShift = 0;
	silenceEnergy = 0; //resetting the silence Energy to 0
    while(!feof(fp)){
        fgets(line, 80, fp);
		cValue = atof(line);
        dcShift += cValue;

		if(cnt == 100){
			if(silenceEnergy < cEnergy){
				silenceEnergy = cEnergy;
			}
			cEnergy = 0;
			cnt = 0;
		}
		cnt++;
		cEnergy += cValue * cValue;

        sample_count++;
    }
    dcShift /= sample_count;
    
    fclose(fp);

}

// dumping average model to file generated in live training
void dump_avg_model_live(int digit){
	char a_file_avg[100], b_file_avg[100], pi_file_avg[100], ind[3];

	sprintf(a_file_avg, "output/live_avgmodels/word_%s_A.txt", keywords[digit]);
	FILE *fp = fopen(a_file_avg, "w");
	for(int i=1; i<=N; i++){
		for(int j=1; j<=N; j++){
			fprintf(fp, "%Le   ", a_average[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	
	sprintf(b_file_avg, "output/live_avgmodels/word_%s_B.txt", keywords[digit]);
	ofstream fout(b_file_avg);
	for(int i=1; i<=N; i++){
		for(int j=1; j<=M; j++){
			//fprintf(fp, "%Le   ", b_average[i][j]);
			fout<<b_average[i][j]<<"   ";
		}
		fout<<endl;
		//fprintf(fp, "\n");
	}
	fout.close();

	sprintf(pi_file_avg, "output/live_avgmodels/word_%s_PI.txt", keywords[digit]);
	fp = fopen(pi_file_avg, "w");
	for(int i=1; i<=N; i++){
		fprintf(fp, "%Le   ", pi_average[i]);
	}
	fclose(fp);
}


//function to setup the global variable like, max and nFactor
//max and nFactor depends on the vowel recording file and are used to do the normalization
void setupGlobal(char *filename){
    FILE *fp;
    long int totalSample = 0;
    char line[100];

    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("Error opening file\n");
    }

    //get max value
    mx = 0;
    while(!feof(fp)){
        fgets(line, 100, fp);
        if(!isalpha(line[0])){
            totalSample++;
            if(mx < abs(atoi(line)))
                mx = abs(atoi(line));
        }
    }
    
    nFactor = (double)LIMIT/mx;
   
    //setup dcShift
    get_DC_shift();
    fclose(fp);
}

//Calculating Tokhura's Distance Using Code Book
void calculate_tokhura_distance(long double cepstralCoeff[12], int index, FILE *fp){
	int  min_index = 0;
	long double min = DBL_MAX;
	long double sum[CB_SIZE] = { 0 };
	string temp, temp1;

	for (int j = 0; j < CB_SIZE; j++){
		for (int i = 0; i < P; i++){
			sum[j] += tokhuraWeights[i] * (cepstralCoeff[i] - codeBook[j][i])*(cepstralCoeff[i] - codeBook[j][i]);
		}
		if (sum[j] < min){
			min = sum[j];
			min_index = j;
		}
	}

	O[index] = min_index + 1;

	//cout << O[index] << " ";
	fprintf(fp, "%4d ", O[index]);
}

//This function calulate the cepstral coefficients Ci's
void calculate_Cis(){
	//if(fake == 62) system("pause");
	double sum=0;
	Ci[0]=log(Ri[0]*Ri[0]);

	for(int m=1;m<=P;m++){
		sum=0;
		for(int k=1;k<m;k++){
			sum += (k*Ci[k]*Ai[m-k])/(m*1.0);
		}
		Ci[m]=Ai[m]+sum;
	}
	//fake++;
}

// Function to apply Durbin Algorithm And Find The value of ai's 
void durbinAlgo(){
	double alpha[13][13],E[13],K[13];
	double sum = 0;
	E[0] = Ri[0];
	//loop for p from 1 to 12
	for(int i=1;i<=P;i++){
		sum=0;
		for(int j=1;j<=i-1;j++){
			sum += alpha[i-1][j]*Ri[i-j];	
		}
		
		K[i]=(Ri[i]-sum)/E[i-1];
			
		alpha[i][i]=K[i];
	
		for(int j=1;j<=i-1;j++){
			alpha[i][j]=alpha[i-1][j] - K[i]*alpha[i-1][i-j];
		}
	
		E[i]=(1-(K[i]*K[i]))*E[i-1];
	}

	//storing the ai values
	for(int i=1;i<=P;i++){
		Ai[i]= alpha[P][i];
	}	
}

//calculating the Ris values
void calculate_Ris(double *samp){
	//if(fake == 62) system("pause");
	for(int m =0; m<=P; m++){
		Ri[m] = 0;
		for(int k=0; k<FRAME_SIZE-m; k++){
			Ri[m] += samp[k]*samp[k+m];
		}
	}
}

//function to apply the Raised Sine Window in Ci of each frame
void applyingRaisedSinWindow(){
	long double sum=0;
	for(int m=1;m<=P;m++){
		sum = (P/2)*sin((PI*m)/P);
		Ci[m]*=sum;	
	}	
}

//calculating c prime values
void calculate_c_prime(double *samp){
	calculate_Ris(samp);
	//calling durbinAlgo to find ai values
	durbinAlgo();
	//finding cepstral constants
	calculate_Cis();
	//applying raised sin window on cis
	applyingRaisedSinWindow();

	//code for universe generation
	for(int i=1; i<=P; i++){
		if(i == P)
			uni<<setw(10)<<Ci[i];
		else
			uni<<setw(10)<<Ci[i]<<", ";
	}
	uni<<endl;
	
}

void trim_word(){
	int num_frames = 0;
	int cnt =0;
	enSize = 0;
	double cEnergySum = 0, multiplier = 3;
	int startMarker = -1, endMarker = -1;

	for(int i=0; i<sSize; i++){
		double cEnergy = sample[i]*sample[i];
		if(cnt == 100){
			energy[enSize++] = cEnergySum;
			cEnergySum = 0;
			cnt = 0;
		}
		cnt++;
		cEnergySum += cEnergy;
	}
	
	//printf("\nenergy - \n");
	for(int i=0; i<enSize; i++){
		//printf("%d: %lf\n", i, energy[i]);
	}
	int min_samples = 11200;

	for(int i=0; i<enSize-4; i++){
		if(startMarker == -1 && endMarker == -1 && energy[i+1] > multiplier * silenceEnergy && energy[i+2] > multiplier * silenceEnergy && energy[i+3] > multiplier * silenceEnergy && energy[i+4] > multiplier * silenceEnergy){
			startMarker = i*100;
		}else if(startMarker != -1 && endMarker == -1 && energy[i+1] <= multiplier * silenceEnergy && energy[i+2] <= multiplier * silenceEnergy && energy[i+3] <= multiplier * silenceEnergy && energy[i+4] <= multiplier * silenceEnergy){
			int diff = i*100 - startMarker;
			if(diff < min_samples){
				startMarker = 0 > (startMarker - (min_samples - diff)/2) ? 0 : (startMarker - (min_samples - diff)/2);
				endMarker = enSize*100 < (i*100 + (min_samples - diff)/2) ? enSize*100 : (i*100 + (min_samples - diff)/2);
			}
			else 
				endMarker = i*100;
		}else if(startMarker != -1 && endMarker!= -1) break;
	}

	sampSize = 0;
	ofstream out("trimmed.txt");
	for(int i=startMarker; i<=endMarker; i++){
		steadySamp[sampSize++] = sample[i];
		out<<sample[i]<<endl;
	}
	out.close();
	//system("pause");
}

//[not using]
double* trim(){
	int i;
	long double total_energy, current_value;
	char trimmed_file[100];
	int start_marker = 0, end_marker = 0;
	
	total_energy = 0;
	int interval = WIN_SIZE, index_count = 0, arr_index = 0;
	double samp[WIN_SIZE] = {0};
	long double total_max_energy = 0;
	int initial_shift_pointer = 0, shift_pointer_count = 0, writing_count = 0;
	long double arr_energy[50];
	long count1 = 0;
	string temp = "";
	//cout << "\n................Trimming Waveform..................." << endl;
	int count = interval;
	//in.open(normalized_file);
	ofstream out("energy_trim.txt");

	//window shifting logic
	for (i = 0; i < sSize; i++){
		count1++;
		//if (shift_pointer_count >= initial_shift_pointer){

		//current_value = stold(temp);
		current_value = sample[i];
		current_value *= 0.1;
		total_energy += (current_value*current_value);
		count--;
		if (!count){

			out << to_string(total_energy) << endl;

			if (total_energy > total_max_energy){
				total_max_energy = total_energy;
				start_marker = initial_shift_pointer;
				end_marker = initial_shift_pointer + interval;
			}

			total_energy = 0;
			count = interval;
			initial_shift_pointer += 500;
			i = initial_shift_pointer - 1;
			shift_pointer_count = 0;
		}
	}
	out.close();
	
	//cout << "Start Marker : " << start_marker << endl;
	//cout << "End Marker : " << end_marker << endl;

	if(end_marker > sSize){
		start_marker -= (end_marker - sSize);
		end_marker = sSize;
	}
	
	
	sprintf(trimmed_file, "trim.txt");
	ofstream out1(trimmed_file);
	for(int i=0; i<sSize; i++){
		index_count++;
		current_value = sample[i]; //stold(temp);
		//current_value = current_value - dc_shift_value;
		//current_value = current_value * normalization_ratio;

		if (index_count >= start_marker && index_count < end_marker){
			samp[arr_index++] = current_value;
			out1 << samp[arr_index - 1] << endl;
		}
	}

	out1.close();
	return samp;
}

//generate observation sequence
void generate_obs_sequence(char *filename){
	int obs_ind = 1;
	FILE *op = fopen(filename, "w");
	if(op == NULL) {
		printf("locha hai idhar bhaiiiii.. \n");
		exit(1);
	}
	
	trim_word();
	double fsamp[FRAME_SIZE];
	int num_frames = 0;
	for(int i=0; i<sampSize; i++){
		num_frames++;
		for(int j = 0; j<320; j++)
			fsamp[j] = steadySamp[i++];

		calculate_c_prime(fsamp);
		calculate_tokhura_distance(Ci, obs_ind++, op);
	}
	T = num_frames;
	cout<<"Number of frames: "<<num_frames<<endl;
	fprintf(op, "\n");
	fclose(op);
	cout<<"wrote observation seq in file: "<<filename<<"\n";
}

//generate observation sequence[not using]
void generate_obs_sequence(char *filename, FILE *op){
	int obs_ind = 1;	
	double *frame_samples = trim();
	double fsamp[FRAME_SIZE];

	for(int i=0; i<WIN_SIZE; i++){
		for(int j = 0; j<320; j++)
			fsamp[j] = frame_samples[i++];

		calculate_c_prime(fsamp);
		calculate_tokhura_distance(Ci, obs_ind++, op);
	}
	cout<<"wrote observation seq in file: "<<filename<<"\n";
}

//training particular file [not using]
void train_file(char *filename, int digit){
	char line[100], obs_file[100];
	// for(int d = 0; d<=1; d++){
		erase_model();
		
		// for(int u = 1; u<=20; u++){
		// sprintf(filename, "input/recordings/Digit %d/rec_%d.txt", d, u);

		FILE *f = fopen(filename, "r");

		if(f == NULL){
			printf("Issue in opening file %s", filename);
			exit(1);
		}
		
		//setting dcshift and nfactor
		setupGlobal(filename);

		sSize = 0;
		//reading the samples and normalizing them
		while(!feof(f)){
			fgets(line, 100, f);
			
			//input file may contain header, so we skip it
			if(!isalpha(line[0])){
				int y = atof(line);
				double normalizedX = floor((y-dcShift)*nFactor);
				//if(abs(normalizedX) > 1)
				sample[sSize++] = normalizedX;
			}
		}
		fclose(f);

		//framing
		sprintf(obs_file, "output/delete/digit_%d_obs_seq.txt", digit);
		generate_obs_sequence(obs_file);
		initialize_model(digit, 1, "--");

		int iteration = 1;
		while(iteration <= CONVERGE_ITERATIONS){
			//cout<<"iteration: "<<iteration++<<endl;
			iteration++;
			forward_procedure(0);
			backward_procedure();
			viterbi();
			calculate_xi();
			calculate_gamma();
			reevaluate_model_parameters();
			//print();
		}

		char a_file_final[100], b_file_final[100], pi_file_final[100];

		sprintf(a_file_final, "output/delete/digit_%d_model_A.txt", digit);
		write_final_A_values(a_file_final);
		sprintf(b_file_final, "output/digit_%d_model_B.txt", digit);
		write_final_B_values(b_file_final);
		sprintf(pi_file_final, "output/digit_%d_model_PI.txt", digit);
		write_final_pi_values(pi_file_final);

		system("pause");
	// }
}

//making only observation sequence from the recordings folder [not using]
void make_all_obs_seq(){
	char filename[100], line[100], obs_file[100];
	for(int d = 0; d<=9; d++){

		sprintf(obs_file, "output/obs_seq/HMM_OBS_SEQ_%d.txt", d);
		FILE *op = fopen(obs_file, "w");
		
		for(int u = 1; u<=20; u++){
			sprintf(filename, "input/recordings/Digit %d/rec_%d.txt", d, u);

			FILE *f = fopen(filename, "r");

			if(f == NULL){
				printf("Issue in opening file %s", filename);
				exit(1);
			}
			
			//setting dcshift and nfactor
			setupGlobal(filename);

			sSize = 0;
			//reading the samples and normalizing them
			while(!feof(f)){
				fgets(line, 100, f);
				
				//input file may contain header, so we skip it
				if(!isalpha(line[0])){
					int y = atof(line);
					double normalizedX = floor((y-dcShift)*nFactor);
					//if(abs(normalizedX) > 1)
					sample[sSize++] = normalizedX;
				}
			}
			fclose(f);
			//calling helper function to generate observation seq
			generate_obs_sequence(obs_file, op);
			fprintf(op, "\n");
		}
		fclose(op);
	}
}

//trains the 20 files
void training(){
	char filename[100], line[100], obs_file[100], dump_file[100], com_dump[100];
	erase_all_model();
	FILE *digit_dump;
	int total_files_trained = 20;
	int total_words = sizeof(keywords)/sizeof(keywords[0]);

	for(int d = 0; d<total_words; d++){
		erase_model();

		sprintf(dump_file, "results/training/training_word_%s.txt", keywords[d]);
		FILE *dig_dump = fopen(dump_file, "w");

		fprintf(common_dump, "\n------------------------------------------------> Word %s <------------------------------------------------\n", keywords[d]);
		fprintf(dig_dump, "\n------------------------------------------------> Word %s <------------------------------------------------\n", keywords[d]);
		
		for(int u = 1; u <= total_files_trained; u++){

			sprintf(filename, "input/recordings/%s/rec_%d.txt", keywords[d], u);

			FILE *f = fopen(filename, "r");

			if(f == NULL){
				printf("Issue in opening file %s", filename);
				exit(1);
			}

			fprintf(dig_dump, "\n------------------------------------------------ opening file %s ------------------------------------------------\n", filename);
			fprintf(common_dump, "\n------------------------------------------------ opening file %s ------------------------------------------------\n", filename);
			
			//setting dcshift and nfactor
			setupGlobal(filename);

			sSize = 0;
			//reading the samples and normalizing them
			while(!feof(f)){
				fgets(line, 100, f);
				
				//input file may contain header, so we skip it
				if(!isalpha(line[0])){
					int y = atof(line);
					double normalizedX = floor((y-dcShift)*nFactor);
					//if(abs(normalizedX) > 1)
					sample[sSize++] = normalizedX;
				}
			}
			fclose(f);

			//framing
			//generating observation seq
			sprintf(obs_file, "output/obs_seq/HMM_OBS_SEQ_%s_%d.txt", keywords[d], u);
			generate_obs_sequence(obs_file);
			fprintf(dig_dump, "->obs seq: ");
			fprintf(common_dump, "->obs seq: ");

			for(int i=1; i<=T; i++){
				fprintf(dig_dump, "%4d ", O[i]);
				fprintf(common_dump, "%4d ", O[i]);
			}

			fprintf(dig_dump, "\n");
			fprintf(common_dump, "\n");
			
			//initializing model
			if(train == 0)
				initialize_model(d, 1, "--");
			else
				initialize_model(d, 1, "avg");

			int iteration = 1;
			//starts converging model upto CONVERGE_ITERATIONS or till convergence whichever reach early
			pstar = 0, prev_p_star = -1;
			while(pstar > prev_p_star && iteration < 1000){
				//cout<<"iteration: "<<iteration++<<endl;
				iteration++;
				prev_p_star = pstar; 
				forward_procedure();
				backward_procedure();
				viterbi();
				
				//printing in log file
				fprintf(dig_dump, "iteration: %d\n", iteration);
				fprintf(dig_dump, "-->pstar : %g\n", pstar);
				fprintf(dig_dump, "-->qstar : ");
				for(int i=1; i<=T; i++){
					fprintf(dig_dump, "%d ", Q[i]);
				}
				fprintf(dig_dump, "\n");

				calculate_xi();
				calculate_gamma();
				//cout<<"difference: "<<prev_p_star - pstar<<endl;
				reevaluate_model_parameters();
			}

			//writing final state sequence
			fprintf(common_dump, "-->qstar : ");
			for(int i=1; i<=T; i++){
				fprintf(common_dump, "%d ", Q[i]);
			}
			fprintf(common_dump, "\n");
			
			//writing final model in the log file
			fprintf(dig_dump, "-------------------------------Final Model Lambda (Pi, A, B) after iterations %d--------------------------------\n", iteration);
			fprintf(common_dump, "-------------------------------Final Model Lambda (Pi, A, B) after iterations %d--------------------------------\n", iteration);
			dump_converged_model(dig_dump);
			dump_converged_model(common_dump);

			add_to_avg_model();
			dump_final_model(u, d);
		}
		fclose(dig_dump);
		average_of_avg_model(total_files_trained);
		dump_avg_model(d);
		erase_avg_model();
		
		//system("pause");
	}
	train++;
}

//TO READ CODEBOOK FROM FILE
void read_codebook(){
	ifstream in("new_codebook.txt");
	for (int i = 0; i < CB_SIZE; i++){
		for (int j = 0; j < P; j++){
			in >> codeBook[i][j];
		}
	}
	in.close();
}

//runs prediction by loading the model and running solution to prob1
void test_prediction(){
	test_ans = 0;
	max_pobs_model = 0;
	int total_words = sizeof(keywords)/sizeof(keywords[0]);
	//checking for all the models
	for(int k = 0; k<total_words; k++){
		if(next_prob_word[k] == 1){
			read_average_model(k);
			solution_to_prob1(k);
		}
	}

	printf("Detected word %s\n", keywords[test_ans]);
	
	char correct;
	printf("Is it correct? (y/n): ");
	cin>>correct;
	
	if(correct == 'y')
		perform(test_ans);
	else
		printf("Sorry!\n");
}

//performs live prediction of the data
void live_testing(){
	char obs_file[100], line[100];
	printf("\n----------Live testing----------\n");

	system("Recording_Module.exe 3 input.wav input_file.txt");
	initialize_model(0, 0, "--");

	FILE *f = fopen("input_file.txt", "r");
	if(f == NULL){
		printf("Issue in opening file input_file.txt");
		exit(1);
	}

	//setting dcshift and nfactor
	setupGlobal("input_file.txt");

	sSize = 0;
	//reading the samples and normalizing them
	while(!feof(f)){
		fgets(line, 100, f);
		
		//input file may contain header, so we skip it
		if(!isalpha(line[0])){
			int y = atof(line);
			double normalizedX = floor((y-dcShift)*nFactor);
			//if(abs(normalizedX) > 1)
			sample[sSize++] = normalizedX;
		}
	}
	fclose(f);
	generate_obs_sequence("output/live_test/obs_seq.txt");
	
	test_prediction();
}

//print alpha beta gamma on screen
void print(){
	cout<<"Alpha values - \n";
	for(int i = 1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			cout<<Alpha[i][j]<<"   ";
		}
		cout<<endl;
	}

	cout<<"Beta values -\n";
	for(int i = 1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			cout<<Beta[i][j]<<"   ";
		}
		cout<<endl;
	}

	cout<<"Gamma values -\n";
	for(int i = 1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			cout<<Gamma[i][j]<<"   ";
		}
		cout<<endl;
	}
}

//function to validate output each model
void validation(){
	char filename[100], line[100];
	initialize_model(1, 0);

	int iteration = 0;
	ofstream dump("dump.txt");


	////////////////////////////////Block to use sir's data/////////////////////////////////////

	// ifstream fin("validation/Digit 1/obs_seq_1.txt");
	// int temp;
	// int obs_ind = 1;
	// while(fin >> temp){
	// 	O[obs_ind] = temp;

	// 	for(int i=2; i<=85; i++){
	// 		fin>>temp;
	// 		O[i] = temp;
	// 	}
	// 	break;
	// }

	// cout<<"Observation seq\n";
	// for(int i=1; i<=85; i++){
	// 	cout<<O[i]<<"   ";
	// }
	// cout<<endl;

	///////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////Block to use own recordings //////////////////////////////////
	sprintf(filename, "input/recordings/Digit 0/rec_1.txt");

	FILE *f = fopen(filename, "r");

	if(f == NULL){
		printf("Issue in opening file %s", filename);
		exit(1);
	}
	
	//setting dcshift and nfactor
	setupGlobal(filename);

	sSize = 0;
	//reading the samples and normalizing them
	while(!feof(f)){
		fgets(line, 100, f);
		
		//input file may contain header, so we skip it
		if(!isalpha(line[0])){
			int y = atof(line);
			double normalizedX = floor((y-dcShift)*nFactor);
			//if(abs(normalizedX) > 1)
			sample[sSize++] = normalizedX;
		}
	}
	fclose(f);

	char obs_file[100];
	sprintf(obs_file, "output/delete/HMM_OBS_SEQ_delete.txt");
	generate_obs_sequence(obs_file);

	///////////////////////////////////////////////////////////////////////////////////////////

	//convergence of the model
	while(pstar > prev_p_star){
		//cout<<"iteration: "<<iteration++<<endl;
		iteration++;
		prev_p_star = pstar; 
		forward_procedure(0);
		backward_procedure();
		viterbi();
		dump<<"Pstar: "<<pstar<<endl;
		dump<<"Qstar: ";
		for(int i=1; i<=T; i++){
			dump<<Q[i]<<"   ";
		}
		dump<<endl;
		calculate_xi();
		calculate_gamma();
		cout<<"difference: "<<prev_p_star - pstar<<endl;
		reevaluate_model_parameters();
	}

	cout<<"Output converged on iteration: "<<iteration<<endl;
	dump<<"Output converged on iteration: "<<iteration<<endl;

	dump<<"----------------------Alpha values--------------------------\n";
	for(int i = 1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			dump<<Alpha[i][j]<<"   ";
		}
		dump<<endl;
	}

	dump<<"------------------------Beta values--------------------------\n";
	for(int i = 1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			dump<<Beta[i][j]<<"   ";
		}
		dump<<endl;
	}

	dump<<"-----------------------------Gamma values----------------------\n";
	for(int i = 1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			dump<<Gamma[i][j]<<"   ";
		}
		dump<<endl;
	}

	dump<<"------------------------------Xi values ------------------------\n";
	for(int i=1; i<=T; i++){
		for(int j = 1; j<=N; j++){
			for(int k = 1; k<=N; k++){
				dump<<Xi[i][j][k]<<"   ";
			}
			dump<<endl;
		}
		dump<<"---\n\n";
	}

	dump<<"------------------------------Model ----------------------------\n";
	dump<<"------------------------------A matrix --------------------------\n";
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++){
			dump << A[i][j] << "   ";
		}
		dump<<endl;
	}

	dump<<"------------------------------B matrix ----------------------------\n";
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= M; j++){
			dump << B[i][j] << "   ";
		}
		dump<<endl;
	}

	dump<<"------------------------------Pi matrix ----------------------------\n";
	for (int i = 1; i <= N; i++){
		dump<<Pi[i]<<"   ";
	}

}

//function to test the models
void testing(){
	char filename[100], line[100], test_file[100];
	int correctAns = 0, totalCorrectAns = 0;
	int total_words = sizeof(keywords)/sizeof(keywords[0]);

	for(int d = 0; d<total_words; d++){
		sprintf(test_file, "results/testing/offline/offline_testing_word_%s.txt", keywords[d]);
		FILE *fp = fopen(test_file, "w");
		correctAns = 0;
		fprintf(fp, "--------------------------------------------* Word %s *--------------------------------------------------------\n", keywords[d]);
		
		for(int j = 41; j<=45; j++){
			sprintf(filename, "input/recordings/%s/rec_%d.txt", keywords[d], j);
			printf("\n\n--------Reading input from the file: %s------\n", filename);
			
			FILE *f = fopen(filename, "r");
			if(f == NULL){
				printf("Issue in opening file input_file.txt");
				exit(1);
			}
			fprintf(fp, "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
			fprintf(fp, "\n---------> Reading file %s <---------\n", filename);

			//setting dcshift and nfactor
			setupGlobal(filename);

			sSize = 0;
			//reading the samples and normalizing them
			while(!feof(f)){
				fgets(line, 100, f);
				
				//input file may contain header, so we skip it
				if(!isalpha(line[0])){
					int y = atof(line);
					double normalizedX = floor((y-dcShift)*nFactor);
					sample[sSize++] = normalizedX;
				}
			}
			fclose(f);

			//generating observation sequence
			generate_obs_sequence("output/live_test/obs_seq.txt");

			fprintf(fp, "observation seq obtained -- ");
			for(int i=1; i<=T; i++){
				fprintf(fp, "%d\t", O[i]);
			}
			fprintf(fp, "\n");

			test_ans = 0;
			max_pobs_model = 0;
			for(int k = 0; k<total_words; k++){
				//if(next_prob_word[k] == 1){
					read_average_model(k);
					solution_to_prob1(k);
					erase_avg_model();
				//}
			}
			
			printf("\nPredicted utterance: %s\n", keywords[test_ans]);
			printf("Actual utterance: %s\n", keywords[d]);

			fprintf(fp, "Predicted utterance: %s\n", keywords[test_ans]);
			fprintf(fp, "Actual utterance: %s\n", keywords[d]);
			if(test_ans == d) correctAns++, totalCorrectAns++;
		}
		printf("Accuracy for the word %s is : %lf % \n", keywords[d], (correctAns / 5.0f)*100);
		fprintf(fp, "Accuracy for the word %s is : %lf % \n", keywords[d], (correctAns / 5.0f)*100);
		//system("pause");
		fclose(fp);
	}

	printf("Accuracy of the system: %lf %\n\n", (totalCorrectAns/80.0f)*100);
}

//testing particular file only
void test_file(char *filename, char *test){
	char line[100], test_file[100];
	int correctAns = 0, totalCorrectAns = 0;

	sprintf(test_file, "results/testing/offline/%s.txt", test);
	FILE *fp = fopen(test_file, "w");
	printf("\n\n--------Reading input from the file: %s------\n", filename);
			
	FILE *f = fopen(filename, "r");
	if(f == NULL){
		printf("Issue in opening file input_file.txt or file does not exist\n");
		exit(1);
	}
	fprintf(fp, "\n---------> Reading file %s <---------\n", filename);

	//setting dcshift and nfactor
	setupGlobal(filename);

	sSize = 0;
	//reading the samples and normalizing them
	while(!feof(f)){
		fgets(line, 100, f);
				
		//input file may contain header, so we skip it
		if(!isalpha(line[0])){
			int y = atof(line);
			double normalizedX = floor((y-dcShift)*nFactor);
			sample[sSize++] = normalizedX;
		}
	}
	fclose(f);

	//generating observation sequence
	generate_obs_sequence("results/testing/offline/manual_test_obs_seq.txt");

	fprintf(fp, "observation seq obtained -- ");
	for(int i=1; i<=T; i++){
		fprintf(fp, "%d\t", O[i]);
	}
	fprintf(fp, "\n");

	test_ans = 0;
	max_pobs_model = 0;
	for(int k = 0; k<=9; k++){
		read_average_model(k);
		solution_to_prob1(k, fp);
		erase_avg_model();
	}
			
	printf("\nPredicted utterance: %d\n", test_ans);

	fprintf(fp, "Predicted utterance: %d\n", test_ans);
	fclose(fp);
}

//live training of a word
void live_training(int choice){

	//itr_count will keep track of no of times a particular word is recorded as part of live training.
	int itr_count=10;
	int listen=0;
	printf("---------------------------Live Training Module----------------------------------\n");
	printf("Now you'll be asked to record your voice for %d times\n",itr_count);
	system("pause");

	//Giving option for user.
	//printf("Enter 1 to listen training audio else enter 2\n");
	//scanf("%d",&listen);

	for(int i=1; i<=itr_count; i++){
		char command[500], filename[50], obs_file[100], line[50];
		char save='a', save_file[100]="";
		
	    cout<<"Press s for saving training data else enter n"<<endl;
		cin>>save;
		//Will save the live training data.
		if(save=='s'||save=='S')
		{
			cout<<"Enter file name to be saved"<<endl;
			cin>>save_file;
			sprintf(filename,"input/live_training/");
			strcat(filename,save_file);
			strcat(filename,".txt");
			sprintf(command, " Recording_Module.exe 3 o.wav ");
	        strcat(command, filename);
		    system(command);
		}
		else
		{
		sprintf(filename, "input/live_training/rec_%d.txt" , i);
		sprintf(command, " Recording_Module.exe 3 input.wav ");
	    strcat(command, filename);
		system(command);
		}

		//Will playback the audio.
		//if(listen==1)
	    //bool played= PlaySound("input.wav", NULL, SND_SYNC);

		FILE *f = fopen(filename, "r");
		
		if(f == NULL){
			printf("Issue in opening file %s", filename);
			exit(1);
		}
		
		//setting dcshift and nfactor
		setupGlobal(filename);

			sSize = 0;
			//reading the samples and normalizing them
			while(!feof(f)){
				fgets(line, 100, f);
				
				//input file may contain header, so we skip it
				if(!isalpha(line[0])){
					int y = atof(line);
					double normalizedX = floor((y-dcShift)*nFactor);
					//if(abs(normalizedX) > 1)
					sample[sSize++] = normalizedX;
				}
			}
			fclose(f);

			//framing
			//generating observation seq
			//sprintf(obs_file, "live_training_observation.txt" );
			//generate_obs_sequence(obs_file);
			sprintf(obs_file, "output/obs_seq/HMM_OBS_SEQ_%s_%d.txt", keywords[choice], i);
			generate_obs_sequence(obs_file);

			// for(int i=1; i<=T; i++){
			// 	fprintf(dig_dump, "%4d ", O[i]);
			// 	fprintf(common_dump, "%4d ", O[i]);
			// }

			// fprintf(dig_dump, "\n");
			// fprintf(common_dump, "\n");
			
			//initializing model
			initialize_model(choice, 1, "--");

			int iteration = 1;
			//starts converging model upto CONVERGE_ITERATIONS or till convergence whichever reach early
			pstar = 0, prev_p_star = -1;
			while(pstar > prev_p_star && iteration < 1000){
				//cout<<"iteration: "<<iteration++<<endl;
				iteration++;
				prev_p_star = pstar; 
				forward_procedure();
				backward_procedure();
				viterbi();
				
// 				//printing in log file
// 				// fprintf(dig_dump, "iteration: %d\n", iteration);
// 				// fprintf(dig_dump, "-->pstar : %g\n", pstar);
// 				// fprintf(dig_dump, "-->qstar : ");
// 				// for(int i=1; i<=T; i++){
// 				// 	fprintf(dig_dump, "%d ", Q[i]);
// 				// }
// 				// fprintf(dig_dump, "\n");

// 				calculate_xi();
// 				calculate_gamma();
// 				//cout<<"difference: "<<prev_p_star - pstar<<endl;
// 				reevaluate_model_parameters();
// 			}

// 			//writing final state sequence
// 			// fprintf(common_dump, "-->qstar : ");
// 			// for(int i=1; i<=T; i++){
// 			// 	fprintf(common_dump, "%d ", Q[i]);
// 			// }
// 			// fprintf(common_dump, "\n");
			
			//writing final model in the log file
			// fprintf(dig_dump, "-------------------------------Final Model Lambda (Pi, A, B) after iterations %d--------------------------------\n", iteration);
			// fprintf(common_dump, "-------------------------------Final Model Lambda (Pi, A, B) after iterations %d--------------------------------\n", iteration);
			// dump_converged_model(dig_dump);
			// dump_converged_model(common_dump);

			add_to_avg_model();
			dump_final_model(i, choice);
	}
	average_of_avg_model(itr_count);
	dump_avg_model_live(choice); //check here
	erase_avg_model();
	
	
}
}

//driver function
int _tmain(int argc, _TCHAR* argv[]){

	uni.open("universe.csv");
	printf("This code is authored by Jay Khatri - 214101023\n");
	char com_dump[100];
	sprintf(com_dump, "results/training/common_dump.txt");
	common_dump = fopen(com_dump, "w");
	
	read_codebook();

	for(int i=0; i<16; i++){
		if(i == 1 || i == 3 || i==2 || i==6 || i==14)
			next_prob_word[i] = 0;
		else
			next_prob_word[i] = 1;
	}
	
	//training();
	char choice;
	
	while(1){
		cout<<"\nPress 1. for automated test on test files\nPress 2. for manual test using the file\nPress 3. for live testing\nPress 4. for live training\nPress 0. to exit\nEnter your choice: "; cin>>choice;

		switch(choice){
			case 't':
				{
					training();
					break;
				}
			case '1':
				{
					
					testing();

					break;
				}

			case '2':
				{
					char filename[100], test[100], test_fn[100];
					printf("Make sure file is available in input/manual_testing/ folder and write .txt (extension in the input)\nEnter the filename you want to test  - ");
					scanf("%s", &test_fn);
					sprintf(filename, "input/manual_testing/%s", test_fn);

					printf("Enter the filename to store the results in - ");
					scanf("%s", &test);

					test_file(filename, test);
					break;
				}
			case '3':
				{
					if(environment_known == 0){
						printf("--------------Recording silence--------------\n");
						system("Recording_Module.exe 3 silence.wav silence_file.txt");	
						environment_known = 1;
					}
					is_live_testing = 1;
					live_testing();
					is_live_testing = 0;
					break;
				}
			
			case '4':
				{
					if(environment_known == 0){
						printf("--------------Recording silence--------------\n");
						system("Recording_Module.exe 1 silence.wav silence_file.txt");	
						environment_known = 1;
					}
					int choice;
					for(int i=0;i<16;i++)
					{
						cout<<"Enter "<<i+1<<" for training "<<keywords[i]<<endl;
					}
					cin>>choice;
					choice--;
					
					live_training(choice);
				break;
					

					

				}
			case '0':
				{
					cout<<"Exiting the program\n";
					return 0;
				}
		}
	}
	
	uni.close();
	return 0;
}
