//
// Lamport Logical Clock Verification
//
// Description:
//   Given Lamport logical clock input the algorithm should
//   find the events associated with those logical clock
//
// Contributers:
//   Name: Ajay Hosmar Shenoy, CPSC 474
//   Name: Punith Jain, CPSC 474
//

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>


using namespace std;

//Function:checkInternalSend
//Operation: Check if operation is Internal
//Declaration
bool checkInternal(int n, int m, int value, int clock_temp[10][10]);

//Function:checkSend
//Operation: Check if operation is Send
//Declaration
bool checkSend(int n, int m, int value, int clock_temp[10][10]);

//Function:checkReceive
//Operation: Check if operation is Receive
//Declaration
bool checkReceieve(int n, int m, int value, int clock_temp[10][10], string events[10][10]);


//Algorithm Verify
int main()
{
	int n=0;
	int m=0;
	bool correct=true;
	string internal_events[9] = { "a","b","c","d","e","f","g","h","i" };
	string send_events[9] = { "s1","s2","s3","s4","s5","s6","s7","s8","s9" };
	string receive_events[9] = { "r1","r2","r3","r4","r5","r6","r7","r8","r9" };

	string events[10][10];
	int lamportClock[10][10]; //Stores the values

	 //Input for all the processes
	cout << "Enter number of Processes: ";
	cin >> n;

	for (int x = 0; x < n; x++)
	{
		cout << "Enter number of events per process p" << x+1 << ":";
		cin >> m;

		for (int y = 0; y < m; y++)
		{
			cout << "p" << x+1 << ":";
			cin >> lamportClock[x][y];
		}
	}


	int internal_counter = 0;
	int send_counter = 0;
	int receieve_counter = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (lamportClock[i][j] == 1) //It should be internal or send event
			{
				int value = lamportClock[i][j];
				if (checkInternal(n, m, value, lamportClock)) //Verify it is internal or send event
				{

					events[i][j] = internal_events[internal_counter];
					internal_counter++;
				}
				else
				{
					events[i][j] = send_events[send_counter];
					send_counter++;
				}
			}
			else if (lamportClock[i][j] == 0)
			{
				events[i][j] == "NULL";
			}
			else
			{
				int next = lamportClock[i][j + 1];
				int value = lamportClock[i][j];
				if (j == 0) //First event at the process
				{
					if (checkReceieve(n, m, value, lamportClock,events)) //A receive event
					{
						events[i][j] = receive_events[receieve_counter];
						receieve_counter++;
					}
					else if (checkInternal(n,m,value,lamportClock))
					{
						events[i][j] = internal_events[internal_counter];
						internal_counter++;
					}
				}

				if (next==lamportClock[i][j]+1) //when its a receive or send event
				{
					events[i][j] = send_events[send_counter];
					send_counter++;
					events[i][j+1] = internal_events[internal_counter+1];
				}
				else
				{
					correct = false;
					//break;
				}
			}
		}
	}
cout<<endl<<"Lamport Logical Clock Verification: "<<endl;
for (int i = 0; i < n; i++)
		{
			cout << "P" << i+1 << ":";
			for (int j = 0; j < m; j++)
			{
				cout << events[i][j]<<"\t";
			}
			cout<<endl;
		}
	if(correct)
    {
        cout<< "Correct"<<endl;
    }
	else
	{
		cout << "Incorrect" << endl;
	}

	ofstream outfile("a2output.txt");
	for (int i = 0; i < n; i++)
	{
		outfile << "P" << i+1 << ":";
		for (int j = 0; j < m; j++)
		{
			outfile<< events[i][j]<<" ";
		}
		outfile << endl;
	}



	return 0;
}


//Function Implementations
bool checkInternal(int n, int m, int value, int clock_temp[10][10])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (value == clock_temp[i][j])//This means its an internal because they are concurrent
			{
				return true;
			}
		}
	}
	return false;
}

bool checkSend(int n, int m, int value, int clock_temp[10][10])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (value == clock_temp[i][j])
			{
				return true;
			}
		}
	}
	return false;
}

bool checkReceieve(int n, int m, int value, int clock_temp[10][10], string events[10][10])
{
    string target= "s";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{

		    if(events[i][j].find(target)!=string::npos)
            {

			if (value ==clock_temp[i][j]) //if this happens its for sure not a receive event
			{
				return false;
			}
            }

        }
	}
	return true;
}

