// speech-project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>

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
int live_model=0;
long double k;

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

//gui variables
double ans;
char choice;
int manual_test_result;

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
int live_update=0;
int train_index=-1;
int itr_count=0;
long double P_O_given_Model = 0;
ofstream uni, dump;
FILE *common_dump;
int environment_known = 0, is_live_testing = 0;
char keywords[][100] = {"calendar", "close", "down", "exit", "faculties", "github", "history", "jobs", "open", "outlook", "payments", "portal", "quora", "search", "up", "video"};

//////////////////////////////////////////////////////////Browser related code///////////////////////////////////////////////////////////////////
bool browser_open = false;
#define SCROLL_DOWN_VAL -700
#define SCROLL_UP_VAL 700
#define CONFIRM_BOX_YES 6
#define CONFIRM_BOX_NO 7
#define CONFIRM_BOX_CANCEL 2

static const WORD TAB_SCANCODE = 0x0f;
static const WORD LEFT_ALT_SCANCODE = 0x38;
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
static const WORD W_SCANCODE = 0x11;

bool next_prob_word[16];
string browserName = "Firefox";
char window_title[256];
HWND hwnd;


void sendkey(WORD scan_code, bool tf_down_up, bool extended = false) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = scan_code;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	if (!tf_down_up) input.ki.dwFlags |= KEYEVENTF_KEYUP;
	if (extended)    input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	//SendInput(1, &input, sizeof(input));
}

void keydown(WORD scan_code, bool extended = false) {
	sendkey(scan_code, true, extended);
}

void keyup(WORD scan_code, bool extended = false) {
	sendkey(scan_code, false, extended);
}

void alt_tab(int tab_count = 1) {
	keydown(LEFT_ALT_SCANCODE);

	Sleep(keypress_delay_ms);
	for (int i = 0; i<tab_count; ++i) {
		keydown(TAB_SCANCODE);
		Sleep(keypress_delay_ms);
		keyup(TAB_SCANCODE);
		Sleep(keypress_delay_ms);
	}
	keyup(LEFT_ALT_SCANCODE);
}

void toggle_to_browser(){
	Sleep(500);
	alt_tab(2);
}

void ctrl_h(){
	keydown(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	keydown(H_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(H_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

}

void open_history(){
	ctrl_h();
}

void ctrl_j(){
	keydown(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	keydown(J_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(J_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);

}

void ctrl_w(){
	keydown(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	keydown(W_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(W_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
}

void ctrl_k(){
	keydown(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
	keydown(K_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(K_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(LCTRL_SCANCODE);
	Sleep(keypress_delay_ms);
}

void alt_p(){
	keydown(LEFT_ALT_SCANCODE);
	Sleep(keypress_delay_ms);
	keydown(P_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(P_SCANCODE);
	Sleep(keypress_delay_ms);
	keyup(LEFT_ALT_SCANCODE);
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
	char command[50];
	
	sprintf(command, "taskkill /f /im %s.exe", browserName);
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
	char input[100], final[200];
	char temp;

	printf("Enter the video name - ");
	
	scanf("%c", &temp);
	//fgets(input, 100, stdin);
	cin.getline(input, 100);
	int i=0;
	while(input[i] != '\0'){
		if(input[i] == ' ')
			input[i] = '+';
		i++;
	}
	sprintf(final, "%s", video_search);
	strcat(final, input);
	char command[1000];
	sprintf(command, "start %s", final);
	system(command);
}

void open_payments(){
	system("start https://online.iitg.ac.in/epay/");
}

void search_google(){
	//query replace space with +
	char  search_query[] = "https://www.google.com/search?q=";
	char qinput[100], final[200];
	char temp;

	printf("Enter the search query - ");
	scanf("%c", &temp);
	cin.getline(qinput, 100);
	int i = 0;
	while(qinput[i] != '\0'){
		if(qinput[i] == ' ')
			qinput[i] = '+';
		i++;
	}

	sprintf(final, "%s", search_query);
	strcat(final, qinput);
	char command[100];
	sprintf(command, "start %s", final);
	system(command);
}

void open_quora(){
	system("start https://www.quora.com/");
}

void open_portal(){
	system("start https://swc.iitg.ac.in/placement-portal/");
}

void open_jobs(){
	system("start https://www.naukri.com/");
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

void scrolling_down(){
	Sleep(500);
	//mouse_event(MOUSEEVENTF_WHEEL, 0, 0, SCROLL_DOWN_VAL, 0);
}

void scrolling_up(){
	Sleep(500);
	//mouse_event(MOUSEEVENTF_WHEEL, 0, 0, SCROLL_UP_VAL, 0);
}

void open_browser(){
	system("start www.google.com");
}

int is_substring(string a, string b){
	int m = a.length();
	int n = b.length();

	for (int i = 0; i <= n - m; i++) {
		int j;
		for (j = 0; j < m; j++)
		if (b[i + j] != a[j])
			break;

		if (j == m)
			return i;
	}

	return -1;
}

//perform the operation according to detected operation
void perform(int index){
	
	int i;
	if (system(NULL))
		puts("Proceed");
	else
		exit(1);
		
	//deciding what to do
	switch (index){
		case 0:
			{
				open_calendar();
				for(int i = 0; i<16; i++){
					if(i==0 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
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
				/*
				x2 = 5;
				while (x2>0){

					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); // get handle of currently active window
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					printf("\n%s", window_title);

					if (is_substring(browserName, window_title) != -1){
						break;
					}
					x2--;
				}
				close_tab();
				*/
				break;

			}
		
		//scroll dowwn
		case 2:
			{
				/*
				x3 = 5;
				while (x3>0){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); // get handle of currently active window
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					printf("\n%s", window_title);

					if (is_substring(browserName, window_title) != -1){
						break;
					}
					x3--;
				}
				scrolling_down();
				alt_tab(1);
				
				
				for(int i = 0; i<16; i++){
					if(i == 8){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				*/
				break;
			}


		//exit browser
		case 3:
			{
				
					/*x1 = MessageBox(nullptr, TEXT("Do you want to close browser?"), TEXT("Message"), MB_YESNOCANCEL);
					if (x1 == CONFIRM_BOX_YES)
					{*/
						system("taskkill /IM firefox.exe /F");
						// close_flag = 0;
						// read_flag = 0;
						// stop_flag = 0;
						for(int i = 0; i<16; i++){
							if(i == 1 || i == 3 || i==2 || i==6 || i==14 || i==8 || i==13)
								next_prob_word[i] = 0;
							else
								next_prob_word[i] = 1;
						}

					
					

				// }
				break;
			}

		case 4:
			{
				open_faculty_page();
				for(int i = 0; i<16; i++){
					if(i == 8 || i==4 || i == 1 || i==2 || i==6 || i==14 || i==13){
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
					if(i==5 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}

				break;
			}

		//history
		case 6:
			{
				/*
				x7 = 5;
				while (x7 >0){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); // get handle of currently active window
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					printf("\n%s", window_title);

					if (is_substring(browserName, window_title) != -1){
						break;
					}
					x7--;
				}


				open_history();
				alt_tab(1);		
				*/
				
				break;
			}

		case 7:
			{
				open_jobs();
				for(int i = 0; i<16; i++){
					if( i == 7 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}


		//open browser
		case 8:
			{
				system("start https://www.google.com/");
				for (i = 0; i < 11; i++){
					if (i == 1 || i==2 || i==6 || i==14 || i==8 || i==13)
						next_prob_word[i] = 0;
					else
						next_prob_word[i] = 1;
				}
				break;
			}

		case 9:
			{
				open_outlook();
				for(int i=0; i<16; i++){
					if( i == 9 || i == 1|| i==2 || i==6 || i==14 || i==8 || i==13){
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
					if(i == 10 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
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
					if(i == 11 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
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
					if(i == 12 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
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
					if(i == 13 || i == 1 || i==2 || i==6 || i==14 || i==8){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		//scroll up
		case 14:
			{
				/*
				x2 = 5;
				while (x2>0){
					alt_tab(2);
					Sleep(1000);
					hwnd = GetForegroundWindow(); // get handle of currently active window
					GetWindowTextA(hwnd, window_title, sizeof(window_title));
					//printf("\n%s", window_title);

					if (is_substring(browserName, window_title) != -1){
						break;
					}
					x2--;
				}
				
				scrolling_up();
				
				alt_tab(1);
				for(int i = 0; i<16; i++){
					if(i == 8){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				*/

				break;
			}
	
		case 15:
			{
				search_video();
				for(int i = 0; i<16; i++){
					if(i == 15 || i == 1 || i==2 || i==6 || i==14 || i==8 || i==13){
						next_prob_word[i] = 0;
					}
					else {
						next_prob_word[i] = 1;
					}
				}
				break;
			}

		default :
			system("taskkill /IM Firefox.exe /F");
			break;
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
		long double pi = Pi[i];
		long double biind = B[i][index];
		Alpha[1][i] = Pi[i]*B[i][index];
		long double p = Alpha[1][i];
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
		forward_procedure(iteration, NULL);
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
			k = A[row][col-1];
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
		long double brc = B[row][col-1];
		for(int i=1; i<M; i++){
			fin>>words;
			B[row][col++] = words;
			brc = B[row][col-1];
		}
		row++;
	}
	
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
void read_average_model_for_live_testing(int digit){
	
	char filename[100];
	sprintf(filename, "output/custom_models/word_%s_A.txt", keywords[digit]);
	readA(filename);

	sprintf(filename, "output/custom_avgmodels/word_%s_B.txt", keywords[digit]);
	readB(filename);

	sprintf(filename, "output/custom_avgmodels/word_%s_PI.txt", keywords[digit]);
	readPi(filename);
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

//reading average model
void read_average_model_for_testing(int digit){
	
	char filename[100];
	sprintf(filename, "output/final_avgmodels/word_%s_A.txt", keywords[digit]);
	readA(filename);

	sprintf(filename, "output/final_avgmodels/word_%s_B.txt", keywords[digit]);
	readB(filename);

	sprintf(filename, "output/final_avgmodels/word_%s_PI.txt", keywords[digit]);
	readPi(filename);
}

//initialize model according to parameters
void initialize_model(int digit, int seq, char *filename = "--"){
	char a_file[100], b_file[100], pi_file[100], obs_file[100];

	if(filename == "--"){
		readA(A_file);
		readB(B_file);
		readPi(PI_file);
	}else if(filename  == "avg"){
		read_average_model(digit);
	}
}

//initialize model according to parameters
void initialize_model_live_train(int digit, int seq, char *filename = "--"){
	char a_file[100], b_file[100], pi_file[100], obs_file[100];

        readA(A_file);
		readB(B_file);
		readPi(PI_file);
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


//adding current model values to avg model
void add_to_avg_model_live_train(){
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

void dump_avg_model_live_save(int digit){
	char a_file_avg[100], b_file_avg[100], pi_file_avg[100], ind[3];

	sprintf(a_file_avg, "output/custom_models/word_%s_A.txt", keywords[digit]);
	FILE *fp = fopen(a_file_avg, "w");
	for(int i=1; i<=N; i++){
		for(int j=1; j<=N; j++){
			fprintf(fp, "%Le   ", a_average[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	
	sprintf(b_file_avg, "output/custom_models/word_%s_B.txt", keywords[digit]);
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

	sprintf(pi_file_avg, "output/custom_models/word_%s_PI.txt", keywords[digit]);
	fp = fopen(pi_file_avg, "w");
	for(int i=1; i<=N; i++){
		fprintf(fp, "%Le   ", pi_average[i]);
	}
	fclose(fp);
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

//Calculating Tokhura's Distance Using Code Book
void calculate_tokhura_distance(long double cepstralCoeff[12], int index, ofstream &fout){
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
	//fprintf(fp, "%4d ", O[index]);
	fout<<setw(4)<<O[index];
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
	// for(int i=1; i<=P; i++){
	// 	if(i == P)
	// 		uni<<setw(10)<<Ci[i];
	// 	else
	// 		uni<<setw(10)<<Ci[i]<<", ";
	// }
	// uni<<endl;
	
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

//generate observation sequence
void generate_obs_sequence(char *filename){
	int obs_ind = 1;
	//FILE *op = fopen(filename, "w");
	ofstream fout(filename);
	if(fout == NULL) {
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
		calculate_tokhura_distance(Ci, obs_ind++, fout);
	}
	T = num_frames;
	cout<<"Number of frames: "<<num_frames<<endl;
	//fprintf(op, "\n");
	fout<<endl;
	fout.close();
	//fclose(op);
	cout<<"wrote observation seq in file: "<<filename<<"\n";
}

//generate observation sequence[not using]
void generate_obs_sequence(char *filename, FILE *op){
	int obs_ind = 1;	
	trim_word();
	double fsamp[FRAME_SIZE];

	for(int i=0; i<WIN_SIZE; i++){
		for(int j = 0; j<320; j++)
			fsamp[j] = steadySamp[i++];

		calculate_c_prime(fsamp);
		calculate_tokhura_distance(Ci, obs_ind++, op);
	}
	cout<<"wrote observation seq in file: "<<filename<<"\n";
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

		//fprintf(common_dump, "\n------------------------------------------------> Word %s <------------------------------------------------\n", keywords[d]);
		fprintf(dig_dump, "\n------------------------------------------------> Word %s <------------------------------------------------\n", keywords[d]);
		
		for(int u = 1; u <= total_files_trained; u++){

			sprintf(filename, "input/recordings/%s/rec_%d.txt", keywords[d], u);

			FILE *f = fopen(filename, "r");

			if(f == NULL){
				printf("Issue in opening file %s", filename);
				exit(1);
			}

			fprintf(dig_dump, "\n------------------------------------------------ opening file %s ------------------------------------------------\n", filename);
			//fprintf(common_dump, "\n------------------------------------------------ opening file %s ------------------------------------------------\n", filename);
			
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
				//fprintf(common_dump, "%4d ", O[i]);
			}

			fprintf(dig_dump, "\n");
			//fprintf(common_dump, "\n");
			
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
			/*fprintf(common_dump, "-->qstar : ");
			for(int i=1; i<=T; i++){
				fprintf(common_dump, "%d ", Q[i]);
			}
			fprintf(common_dump, "\n");
			*/
			//writing final model in the log file
			fprintf(dig_dump, "-------------------------------Final Model Lambda (Pi, A, B) after iterations %d--------------------------------\n", iteration);
			//fprintf(common_dump, "-------------------------------Final Model Lambda (Pi, A, B) after iterations %d--------------------------------\n", iteration);
			dump_converged_model(dig_dump);
			//dump_converged_model(common_dump);

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
int test_prediction(){
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

	return test_ans;
}

//performs live prediction of the data
int live_testing(){
	char obs_file[100], line[100];
	printf("\n----------Live testing----------\n");

	system("Recording_Module.exe 3 input.wav input_file.txt");
	//initialize_model(0, 0, "--");

	FILE *f = fopen("input_file.txt", "r");//"input_file.txt"
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

	for(int i=1; i<=T; i++){
		int o = O[i];
	}
	
	return test_prediction();
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

//function to test the models
double testing(){
	double acc;
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
				read_average_model_for_testing(k);
				solution_to_prob1(k);
				erase_avg_model();
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
	return ans = (totalCorrectAns/80.0f)*100;
}

//testing particular file only
int test_file(char *filename, char *test){
	char line[100], test_file[100];
	int correctAns = 0, totalCorrectAns = 0;

	int total_words = sizeof(keywords)/sizeof(keywords[0]);

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
		int o = O[i];
		//fprintf(fp, "%d\t", O[i]);
	}
	fprintf(fp, "\n");

	test_ans = 0;
	max_pobs_model = 0;
	for(int k = 0; k<total_words; k++){
		read_average_model(k);
		solution_to_prob1(k, fp);
		//erase_avg_model();
	}
			
	printf("\nPredicted utterance: %d\n", keywords[test_ans]);

	fprintf(fp, "Predicted utterance: %d\n", test_ans);
	fclose(fp);

	return test_ans;
}

//function to call the test_file
int test_file_helper(){
	read_codebook();
	char filename[100]="input/input_file.txt", test_1[100]="manual_testing_result.txt";
	return test_file(filename,test_1);
}

//live training of a word
void live_training(int choice){
	if(itr_count>10)
		return;

	//itr_count will keep track of no of times a particular word is recorded as part of live training.
	
	int listen=0;
	printf("---------------------------Live Training Module----------------------------------\n");
	printf("Now you'll be asked to record your voice for %d times\n",itr_count);
	system("pause");

	//Giving option for user.
	//printf("Enter 1 to listen training audio else enter 2\n");
	//scanf("%d",&listen);

	for(int i=1; i<=1; i++){
		char command[500], filename[50], obs_file[100], line[50];
		char save='a', save_file[100]="";
		
	   // cout<<"Press s for saving training data else enter n"<<endl;
		//cin>>save;
		//Will save the live training data.
		
			sprintf(filename, "input/live_training/rec_%d.txt" , itr_count);
			sprintf(command, " Recording_Module.exe 3 input.wav ");
			strcat(command, filename);
			system(command);

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
		sprintf(obs_file, "output/obs_seq/HMM_OBS_SEQ_%s_%d.txt", keywords[choice], itr_count);
		generate_obs_sequence(obs_file);

		initialize_model_live_train(choice, 1, "--");

		int iteration = 1;
		//starts converging model upto CONVERGE_ITERATIONS or till convergence whichever reach early
		pstar = 0, prev_p_star = -1;
		while( pstar > prev_p_star &&iteration < 1000){
			//cout<<"iteration: "<<iteration++<<endl;
			iteration++;
			prev_p_star = pstar; 
			forward_procedure();
			backward_procedure();
			viterbi();
			calculate_xi();
			calculate_gamma();
			//cout<<"difference: "<<prev_p_star - pstar<<endl;
			reevaluate_model_parameters();
		}
		add_to_avg_model_live_train();
		dump_final_model(itr_count, choice);
	}

	if(itr_count==10)
	{
	average_of_avg_model(10);
	if(live_update==0)
	dump_avg_model_live(choice); //check here
	else if(live_update=1)
	dump_avg_model_live_save(choice);

	erase_avg_model();
	}
}


//live testing helper funciton to gui
int live_testing_helper(){
	read_codebook();
	for(int i=0; i<16; i++){
		//windows.h functions removing to be used
		if(i == 1 || i == 3 || i==2 || i==6 || i==14 || i==13 || i==15)
			next_prob_word[i] = 0;
		else
			next_prob_word[i] = 1;
	}

	if(environment_known == 0){
		printf("--------------Recording silence--------------\n");
		system("Recording_Module.exe 3 silence.wav silence_file.txt");	
		environment_known = 1;
	}
	is_live_testing = 1;
	int live_ans = live_testing();
	is_live_testing = 0;
	manual_test_result = live_ans;
	return live_ans;
}
