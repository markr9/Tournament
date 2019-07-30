// PartB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h" //MVS generated header file
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

//functions
int round(vector <string>, vector <int>, vector<vector <int>>, vector <string>, vector <string>, vector <string>, int);

int main()
{
	//player names
	vector <string> names = { "a01","b02","c03","d04","e05","f06","g07","h08","i09","j10","k11","l12","m13","n14","o15","p16","q17","r18","s19","t20" };
	//Q bonus- if an odd number of players add a no opponent to the container
	if (names.size() % 2 == 1) { names.push_back("No opponent this round"); }
	//size/number of players
	const int s = names.size();
	//random numbers array
	vector <int> nums;
	//add numbers 0 to 19 to array
	for (int i = 0; i < s; i++)
	{
		nums.push_back(i);
	}
	//player vs player table
	vector<vector <int>> sheet;
	//2D vector/array size
	sheet.resize(s);
	for (int i = 0; i < s; ++i)
	{
		sheet[i].resize(s);
	}
	//set all values in sheet to 0
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			sheet.at(i).at(j) = 0;
		}
	}
	//set all same player vs to 8 (eg t20 vs t20 to 8)
	for (int i = 0; i < s; i++)
	{
		sheet.at(i).at(i) = 8;
	}
	//for post19 rounds
	//holds player vs player results
	vector <string> pvsp;
	//temp hold for player vs player results
	vector <string> pvsptemp;
	//file output- array for files names
	vector <string> fnames;
	//add file names to array/vector
	for (int i = 1; i < (s * 2 - 1); i++)
	{
		stringstream stream;
		string num;
		string tx = "Round ";
		//cast int to string then concatenate onto tx
		stream << i;
		stream >> num;
		//clear stream
		stream.str("");
		stream.clear();
		tx += num;
		tx += ".txt";
		//add to array
		fnames.push_back(tx);
	}
	//determine rounds
	int attemps = 1, rr, cond = 1;
	//while true run the function
	while (cond == 1)
	{
		cond = round(names, nums, sheet, pvsp, pvsptemp, fnames, s);
		//number of attemps to find a solution
		attemps += cond;
	}
	//output attemps
	cout << "\nCompleted in " << attemps << " attemps\n" << endl;
	
	//pause to view results
	string end;
	cout << "Enter anything to exit: ";
	cin >> end;

	return 0;
}
//function that determines which player plays which player for each round
int round(vector <string> names, vector <int> nums, vector<vector <int>> sheet, vector <string> pvsp, vector <string> pvsptemp, vector <string> fnames, int s)
{
	//random seed
	srand((int)time(0));
	int i = 0, j = 1, p, q, r, t, g, h, hold = 0, c = 0;
	string a, b, txhold, num;
	stringstream stream;
	//nums2- holds nums for re-initisation each iteration
	vector <int> nums2 = nums;
	//loop for rounds 1 to 19
	while (j < s)
	{
		//change any 3s or 4s in sheet to 1s and 2s- from last iteration
		for (int i = 0; i < s; i++)
		{
			for (int j = 0; j < s; j++)
			{
				if (sheet.at(i).at(j) == 3)
				{
					sheet.at(i).at(j) = 1;
				}
				if (sheet.at(i).at(j) == 4)
				{
					sheet.at(i).at(j) = 2;
				}
			}
		}
		//re-initisation of nums
		nums = nums2;
		//add results from pvsptemp to pvsp- from last iteration
		for (int i = 0; i < pvsptemp.size(); i++)
		{
			pvsp.push_back(pvsptemp.at(i));
		}
		//clear pvsptemp
		pvsptemp.clear();
		//randomise numbers in nums
		for (int k = 0; k < s; k++)
		{
			r = (rand() % s);
			t = nums.at(k); nums.at(k) = nums.at(r); nums.at(r) = t;
		}
		//clear txhold- text hold
		txhold = " ";
		//display round
		cout << "Round " << j << endl;
		//name file after the current round
		ofstream f(fnames.at(j - 1));
		//concatenate round and round number (num) into txhold for start of the file's listing
		stream << j;
		stream >> num;
		stream.str("");
		stream.clear();
		txhold = "Round " + num + ":\n";
		//loop for 1 round
		//while i=0 < size of nums array
		while (i < nums.size())
		{
			//reinitalise g and h if last iteration was sucessful- index to nums
			if (hold == 0) { g = 0, h = 1; }
			//take the 1st 2 numbers in nums element to give players- index to names
			p = nums.at(g);
			q = nums.at(h);
			//if the 2 players have not played each other- for 1st 19 rounds
			if (sheet.at(p).at(q) == 0)
			{
				//sucessful iteration
				hold = 0;
				//get the player names
				a = names.at(p);
				b = names.at(q);
				//set index on sheet to 3 for current round and 4 for post19 rounds to stop players playing each other till after round 19
				sheet.at(p).at(q) = 3;
				sheet.at(q).at(p) = 4;
				//output results and add to txhold
				cout << "Player " << a << " vs " << "Player " << b << endl;
				txhold += "Player " + a + " vs Player " + b + "\n";
				//add result to pvsptemp
				pvsptemp.push_back(a);
				pvsptemp.push_back(b);
				//remove values in nums at g and h posistions
				//need to rmeove the larger index 1st- should always be h
				if (g > h)
				{
					nums.erase(nums.begin() + g);
					nums.erase(nums.begin() + h);
				}
				else
				{
					nums.erase(nums.begin() + h);
					nums.erase(nums.begin() + g);
				}
			}
			//if players have already played each other
			else
			{
				//stop g and h being reset and increase h by 1 to take next element instead and re-run the iteration
				hold = 1; h++;
				//if h reaches the end of the nums array- no matches
				if (h >= nums.size())
				{
					//re-do round and reset
					//output fail
					cout << "fail" << endl;
					//decrese round value j by 1, reset hold to 0 and clear pvsptemp
					j--;
					hold = 0;
					pvsptemp.clear();
					//return all 3s and 4s to 0 to reset this round
					for (int i = 0; i < s; i++)
					{
						for (int j = 0; j < s; j++)
						{
							if (sheet.at(i).at(j) == 3)
							{
								sheet.at(i).at(j) = 0;
							}
							if (sheet.at(i).at(j) == 4)
							{
								sheet.at(i).at(j) = 0;
							}
						}
					}
					//if lots of iterations are completed
					c++;
					if (c > 1000)
					{
						//state and error and redo all the rounds as a possbile soloution was not found- quicker to restart than wait for the correct sequecne of number to be produced
						cout << "Error, restart- failed to match for 1000 iterations, round " << j << endl;
						//return 1 to keep condition true and add to attempt count
						return 1;
					}
					//break loop if iteration failed
					break;
				}
			}
		}
		//output all player vs player results for this round into file then close the file. Note files are overwritten each use
		if (!f)
		{
			cout << "Error file opening" << endl;
			return -1;
		}
		f << txhold << endl;
		f.close();
		//move onto next round
		j++;
	}
	//end of loop- round 19 3s and 4s on sheet not corrected so added here
	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < s; j++)
		{
			if (sheet.at(i).at(j) == 3)
			{
				sheet.at(i).at(j) = 1;
			}
			if (sheet.at(i).at(j) == 4)
			{
				sheet.at(i).at(j) = 2;
			}
		}
	}
	//end of loop- add round 19 to end then clear pvsptemp
	for (int i = 0; i < pvsptemp.size(); i++)
	{
		pvsp.push_back(pvsptemp.at(i));
	}
	pvsptemp.clear();

	//rounds reverse
	int rev = 1, rev2 = 0;
	//for rounds 20 to 38
	while (rev < s)
	{
		//rev2 is current round, rev is the round for the 2nd half (20-38) for the loop
		rev2 = 19 + rev;
		//output round and clear txhold
		cout << "Round " << rev + 19 << endl;
		txhold = " ";
		//create file of current round
		ofstream f(fnames.at(rev + 19 - 1));
		//add round to top of file in txhold
		stream << rev2;
		stream >> num;
		//clear stream
		stream.str("");
		stream.clear();
		txhold = "Round " + num + ":\n";
		//loop to output player vs player for each round
		i = 0;
		while (i < (s/2))
		{
			//output the result for the round, add them to txhold and remove them from pvsp- note play order is reversed
			cout << "Player " << pvsp.at(1) << " vs " << "Player " << pvsp.at(0) << endl;
			txhold += "Player " + pvsp.at(1) + " vs Player " + pvsp.at(0) + "\n";
			pvsp.erase(pvsp.begin());
			pvsp.erase(pvsp.begin());
			i++;
		}
		//write round result to file and close file
		if (!f)
		{
			cout << "Error file opening" << endl;
			return -1;
		}
		f << txhold << endl;
		f.close();
		//move onto next round
		rev++;
	}

	//output sheet
	for (int i = 0; i < s; i++)
	{
		cout << endl;
		for (int j = 0; j < s; j++)
		{
			cout << sheet.at(i).at(j);
		}
	}
	cout << endl;

	//return 0 for a sucessful output, sets cond to 0 and ends the loop
	return 0;
}
//end