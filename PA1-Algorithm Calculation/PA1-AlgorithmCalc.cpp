//
// Lamport Logical Clock Calculation
//
// Description:
//   Given events at the different processes as input the algorithm should
//   calculate the logical clock for the events.
//
// Contributers:
//   Name: Ajay Hosmar Shenoy, CPSC 474
//   Name: Puneet Jain, CPSC 474
//

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include<fstream>

using namespace std;

struct events {
	string eventType; //Type of the event(i.e Internal/External/Receive)
	int clockValue=-1; //Calculated clock value
};
//Function: calculateLamportClock
//Operation: When we have completed calculating Lamport clock for every Send and Internal events. We start calculating Lamport Clock values for Receive events.
//Declaration
void calculateLamportClock(events clock_temp[10][10], int n, int m, int lamportClock[10][10]);

//Function: calculateSendEventClock
//Operation: Match the correspondent send event and return the Lamport Logical clock.
//Declaration
int calculateSendEventClock(events clock_temp[10][10], int n, int m, string x);

bool checkAll(events clock_temp[10][10], int n, int m, int lamportClock[10][10]);
int main()
{
	int n=0; //n= number of processes
    int m=0; //m=max number of events
	int lc_value=1;
	events clock_temp[10][10];
	int lamportClock[10][10]; //Clock values

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
			cin >> clock_temp[x][y].eventType;
		}
	}

    //Computation of the algorithm
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (clock_temp[i][j].eventType.length()==1 || clock_temp[i][j].eventType[0]=='s') //Check if it is Internal or Send Event
			{
				if (j == 0)//Check if it is the first event at the processor
				{
					clock_temp[i][j].clockValue = 1; //Assign LC=1 for all the first event
					lamportClock[i][j] = clock_temp[i][j].clockValue;
				}
				else if(clock_temp[i][j-1].clockValue==-1)
                {
                    clock_temp[i][j].clockValue=-1;
                    //lamport=-1??
                }
				else
				{
					lc_value = clock_temp[i][j - 1].clockValue + 1;
					clock_temp[i][j].clockValue = lc_value; //Save the value for that particular event in temp
					lamportClock[i][j] = lc_value;
					lc_value++;
				}
			}
			else if (clock_temp[i][j].eventType.length() > 1 && clock_temp[i][j].eventType[0]=='r') //Check if it is a Receive event
			{
				if (j == 0) //Check if it is the first event at the processor
				{
					int k = calculateSendEventClock(clock_temp, n, m, clock_temp[i][j].eventType); //Returns the clock value of send event
					if (k == -1)
					{
						clock_temp[i][j].clockValue = k; // Wait for the corresponding send event
					}
					else
					{
						clock_temp[i][j].clockValue = k + 1;
						lamportClock[i][j] = k + 1;
						lc_value++;
					}
				}
				else
				{
					int send_event = calculateSendEventClock(clock_temp, n, m, clock_temp[i][j].eventType); //Calculate send event LC Value
					if (send_event >0)
					{
						int k = clock_temp[i][j - 1].clockValue; //The event before the receive event in that processor
						clock_temp[i][j].clockValue = max(k, send_event) + 1;
						lamportClock[i][j] = clock_temp[i][j].clockValue;
						lc_value = clock_temp[i][j].clockValue;
						lc_value++;
					}
					else
					{
						//Set the Value to be -1 for now. Will be updated when send events are processed
						clock_temp[i][j].clockValue = -1;
						lamportClock[i][j] =-1;
					}
				}
			}
			else if (clock_temp[i][j].eventType == "0" || clock_temp[i][j].eventType == "NULL" || clock_temp[i][j].eventType == "null" ) //No value
			{
				clock_temp[i][j].clockValue = 0;
				lamportClock[i][j] = 0;
			}
			else
            {
                lamportClock[i][j]=-1;
                clock_temp[i][j].clockValue=-99;
            }
		}
	}
    while(!checkAll(clock_temp, n, m, lamportClock))
    {
	calculateLamportClock(clock_temp, n, m, lamportClock);
    }
	//Print Lamport Clock Values
	cout<<endl<<"Lamport Logical Clock: "<<endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i+1 << ": ";
		for (int j = 0; j < m; j++)
		{
		    if(lamportClock[i][j]!=-1){
			cout<< lamportClock[i][j]<<"\t";
		    }
		}
		cout<< endl;
	}


	ofstream outfile("a1output.txt");
	for (int i = 0; i < n; i++)
	{
		outfile << "P" << i+1 << ":";
		for (int j = 0; j < m; j++)
		{
			if(lamportClock[i][j]!=-1){
			outfile<< lamportClock[i][j]<<" ";
		    }
		}
		outfile << endl;
	}

return 0;
}


//Function Implementation
void calculateLamportClock(events clock_temp[10][10], int n, int m, int lamportClock[10][10])
{
	int send_event = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (clock_temp[i][j].clockValue == -1)
			{
				if (clock_temp[i][j].eventType.length() == 1 || clock_temp[i][j].eventType[0]=='s') //Checking Internal event
				{
				    if(clock_temp[i][j-1].clockValue!=-1)
                    {
					clock_temp[i][j].clockValue = clock_temp[i][j - 1].clockValue + 1; //Calculating LC for Internal Event
					lamportClock[i][j] = clock_temp[i][j].clockValue;
                    }
				}
				else //Checking Receive event
				{
					send_event = calculateSendEventClock(clock_temp, n, m, clock_temp[i][j].eventType);
					if(send_event!=-1)
					{
					clock_temp[i][j].clockValue = max(clock_temp[i][j - 1].clockValue, send_event) + 1; //Calculating LC for Receive Event
					lamportClock[i][j] = clock_temp[i][j].clockValue;
					}
				}
			}
		}
	}
}



int calculateSendEventClock(events clock_temp[10][10], int n, int m, string x)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (x[1] == clock_temp[i][j].eventType[1]) //Checking the index of send values. i.e S1
			{
			    if(x[0]==clock_temp[i][j].eventType[0])
                {

                }
                else {
			    if(clock_temp[i][j].clockValue==-1)
                {
                    return -1; //If corresponding send event is not found
                }
                else
                {
                  return clock_temp[i][j].clockValue;
                }

			}
			}
		}
	}

}

bool checkAll(events clock_temp[10][10], int n, int m, int lamportClock[10][10])
{
    for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (clock_temp[i][j].clockValue == -1)
			{
			    return false;
			}
		}
	}
	return true;
}



