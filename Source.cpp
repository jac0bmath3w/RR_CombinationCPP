#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<sstream>
#include<algorithm>
#include<map>

struct Tree
{
	int data;
	int parent; 
	int arr[1000];
	int node;
	int outlier;
	Tree()
	{
		data = 0;
		parent = 0;
		node = 0;
		outlier = 0;
		for (int i = 0; i < 10; i++)
			arr[i] = -1;
	}
	~Tree(){}
};
Tree t[22][950]; 
Tree b[22][950];

using namespace std;
int main()
{
	const int attributes = 22;
	const int num_accidents = 950;
	const int max_divisions = 11;
	const int max_locations = 15;
	int largest_modifier[attributes]= {0};
	float score[attributes] = {0};
	string xno = "ABCD";
	string str_warning_device[num_accidents];
	double long number[max_locations][5] = {0};
	int a[num_accidents][attributes][max_divisions] = {0};
	int temp1[attributes][max_divisions] = {0};
	int temp = 0;
	int max_subcat[attributes] = {0};
	int number_counter = 0;
	int new_test = 0;
	int test1 = 0;
	int tie = -1;
	float expectation[attributes][max_divisions] = {0};
	int std_arr[attributes] = {4,2,3,3,4,4,2,4,6,4,10,4,4,3,3,3,5,8,2,2,3,2};
	int count_all[attributes][max_divisions] = {0};
	int largest_first_level[attributes] = {0};
	int largest_leaf[attributes] = {0};
	int for_grouping = 0;
	int grouping[1400] = {0};
	string inventory_crossing[6800];
	int inventory_angle[6800] = {0};

	string line;
	string dummy;
	ofstream newfile;
	ifstream myfile;
	ifstream locationfile;
	ifstream expectationfile;
	ifstream anglefile;

	expectationfile.open("Expectations_Historic.csv");
	int expectation_counter = 0;
	if (!expectationfile.is_open())
	{
		cout<<"Expectations file not found"<<endl;
		cin.get();
		return 0;
	}
	while (!expectationfile.eof())
	{
		getline(expectationfile,line);
		if (line.compare("") != 0)
		{
			istringstream ss (line);
			int j = 0;
			while (getline(ss,dummy,','))
			{
				float exp;
				stringstream(dummy) >> exp;
				expectation[expectation_counter][j] = exp;
				j++;
			}
		}
		expectation_counter++;
	}
	expectationfile.close();

	map<char,int> AngleMap;
	anglefile.open("All.csv");
	if (!anglefile.is_open())
	{
		cout<<"Angles file not found"<<endl;
		cin.get();
		return 0;
	}
	int no_of_lines = 0;
	while (!anglefile.eof())
	{
		getline(anglefile,line);
		no_of_lines++;
		if (no_of_lines == 1)
		{
			continue;
		}
		int counter = 0;
		
		istringstream ss(line);
		string crossing;
		int angle = 0;
		int y = 0;
		while (getline(ss,dummy,','))
		{
			
			counter++;
			if (counter == 1)
			{
				crossing.assign(dummy);
			}
			if (counter == 8)
			{
				stringstream(dummy)>>angle;
			}
//			AngleMap.insert(pair<string, int>("ABCD", 12));
			inventory_crossing[y] = crossing;
			inventory_angle[y] = angle;
			y++;
		}
	}
	no_of_lines = 0;
	int accCount = 0;
//	cout<<"Enter file name"<<endl;
	string filename;
	string location[950];

	filename = "Illinois Compiled Database 2002-11.csv";
	cout<<"Reading from file "<<filename<<endl;
//	std::getline(cin,filename);
//	myfile.open("Illinois_Compiled_Accident.csv");         //Not relevant now. 

	myfile.open(filename);
	if (!myfile.is_open())
	{
		cout<<"File not found"<<endl;
		cin.get();
		cin.get();
		return 0;
	}
	newfile.open("To Avoid Private.txt");
	while (!myfile.eof())
	{
		getline(myfile,line);
		no_of_lines = no_of_lines+1;
		if (no_of_lines == 1)
		{
			continue;
		}
		
		int counter = 0;
		istringstream ss(line);
		while (getline(ss,dummy,','))
		{
			counter++;
			if (counter == 16)
			{
				if (dummy.compare(xno) == 0)
				{
					accCount = accCount+1;
					xno = dummy;
				}
				else
				{
					
					if (accCount > 0)
					{

						for (int i = 0; i < accCount; i++)   //to check all accidents
						{
							for (int j = 0; j < attributes; j++)     // to check all conditions
							{
								for (int k = 0; k < std_arr[j]; k++)  //division in condition
								{
									if (a[i][j][k] == 1)
									{
										temp1[j][k] = temp1[j][k]+1;
									}
								}
							}
						}
						for (int j = 0; j < attributes; j++)
						{
							for (int k = 0; k < std_arr[j]; k++)
							{
								if (largest_modifier[j] < temp1[j][k])
								{
									largest_modifier[j] = temp1[j][k];
								}
							}
						}
						for (int j = 0; j < attributes; j++)
						{
//							score[j] = score[j] + (10-(accCount-largest_modifier[j]+1));
//							cout<<"here"<<endl;
							score[j] = score[j] + largest_modifier[j];//float(accCount);
//							cout<<j<<" | "<<score[j]<<" | "<<largest_modifier[j]<<" | "<<accCount<<" | "<<xno<<endl;
//							cin.get();
						}
					}
					for (int j = 0; j < attributes; j++)
					{
						for (int k = 0; k < max_divisions; k++)
						{
							temp1[j][k] = 0;
						}
						largest_modifier[j] = 0;
					}
					xno = dummy;
					for (int i = 0; i < num_accidents; i++)
					{
						for (int j = 0; j < attributes; j++)
						{
							for (int k = 0; k < std_arr[j]; k++)
							{
								a[i][j][k] = 0;
							}
						}
					}
					accCount = 1;
					continue;
				}
			}
			if (counter == 18)   //Month
				{

					if (!dummy.empty())
					{
						int month;
//						cout<<dummy<<endl;
//						cin.get();
						stringstream(dummy) >> month;
						temp = (month-1)/3;
//						cout<<month<<" | "<<temp<<endl;
//						cin.get();
						a[accCount-1][0][temp] = 1;
						count_all[0][temp]++;
					}
					continue;
				}
				if (counter == 22)   //AMPM
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("AM"))
						{
							a[accCount-1][1][0] = 1;
							count_all[1][0]++;
						}
						else 
						{
							a[accCount-1][1][1] = 1;
							count_all[1][1]++;
						}
					}
					continue;
				}
				if (counter == 30)    //Vehicle Speed
				{
					if (!dummy.empty())
					{
						int vehspd;
						stringstream(dummy) >> vehspd;
						temp = (vehspd)/20;
						a[accCount-1][2][temp] = 1;
						count_all[2][temp]++;
					}
					continue;
				}
				if (counter == 31)    //Type of highway user
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("K"))
						{
							a[accCount-1][3][0] = 1;
							count_all[3][0]++;
						}
						else if (!dummy.compare("A") || !dummy.compare("B") || !dummy.compare("C") || !dummy.compare("D") || !dummy.compare("E") || !dummy.compare("F") || !dummy.compare("G") || !dummy.compare("H") || !dummy.compare("I") || !dummy.compare("J"))
						{
							a[accCount-1][3][1] = 1;
							count_all[3][1]++;
						}
						else
						{
							a[accCount-1][3][2] = 1;
							count_all[3][2]++;
						}
					}
					continue;
				}
				if (counter == 32)   //Highway user direction
				{
					if (!dummy.empty())
					{
						int vehdir;
						stringstream(dummy) >> vehdir;
						a[accCount-1][4][vehdir-1] = 1;
						count_all[4][vehdir-1]++;
					}
					continue;
				}
				if (counter == 33) // Position of vehicle
				{
					if (!dummy.empty())
					{
						int position;
						stringstream(dummy) >> position;
						a[accCount-1][5][position-1] = 1;
						count_all[5][position-1]++;
					}
					continue;
				}
				if (counter == 36)   //Type of Accident
				{
					if (!dummy.empty())
					{
						int typacc;
						stringstream(dummy) >> typacc;
						a[accCount-1][6][typacc-1] = 1;
						count_all[6][typacc-1]++;
					}
					continue;
				}
				if (counter == 39)   //Visibility
				{
					if (!dummy.empty())
					{
						int visiblty;
						stringstream(dummy) >> visiblty;
						a[accCount-1][7][visiblty-1] = 1;
						count_all[7][visiblty-1]++;
					}
					continue;
				}
				if (counter == 40)   //Weather
				{
					if (!dummy.empty())
					{
						int weather;
						stringstream(dummy) >> weather;
						a[accCount-1][8][weather-1] = 1;
						count_all[8][weather-1]++;
					}
					continue;
				}
				if (counter == 42)  //Type of track
				{
					if (!dummy.empty())
					{
						int typtrk;
						stringstream(dummy) >> typtrk;
						a[accCount-1][9][typtrk-1] = 1;
						count_all[9][typtrk-1]++;
					}
					continue;
				}
				if (counter == 44)   //Track Class
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("X"))
						{
							a[accCount-1][10][0] = 1;
							count_all[10][0]++;
							continue;
						}
						int trkclas;
						stringstream(dummy) >> trkclas;
						a[accCount-1][10][trkclas] = 1;
						count_all[10][trkclas]++;
					}
					continue;
				}
				if (counter == 47)   //Train Speed
				{
					if (!dummy.empty())
					{
						int trnspd;
						stringstream(dummy) >> trnspd;
						temp = (trnspd)/20;
						a[accCount-1][11][temp] = 1;
						count_all[11][temp]++;
						if (trnspd > 60)
						{
							a[accCount-1][11][3] = 1;
							count_all[11][3]++;
						}
					}
					continue;
				}
				if (counter == 49)  //Train Direction
				{
					if (!dummy.empty())
					{
						int trndir;
						stringstream(dummy) >> trndir;
						a[accCount-1][12][trndir-1] = 1;
						count_all[12][trndir-1]++;
					}
					continue;
				}
				if (counter == 51)  //Location of warning device
				{
					if (!dummy.empty())
					{
						int locwarn;
						stringstream(dummy) >> locwarn;
						a[accCount-1][13][locwarn-1] = 1;
						count_all[13][locwarn-1]++;
					}
					continue;
				}
				if (counter == 52) // Warning sign connected to highway signs
				{
					if (!dummy.empty())
					{
						int warnsign;
						stringstream(dummy) >> warnsign;
						a[accCount-1][14][warnsign-1] = 1;
						count_all[14][warnsign-1]++;
					}
					continue;
				}
				if (counter == 53) //lights at crossing
				{
					if (!dummy.empty())
					{
						int lights;
						stringstream(dummy) >> lights;
						a[accCount-1][15][lights-1] = 1;
						count_all[15][lights-1]++;
					}
					continue;
				}
				if (counter == 56) // Action of motorist
				{
					if (!dummy.empty())
					{
						int motorist;
						stringstream(dummy) >> motorist;
						a[accCount-1][16][motorist-1] = 1;
						count_all[16][motorist-1]++;
					}
					continue;
				}
				if (counter == 57)   //view
				{
					if (!dummy.empty())
					{
						int view;
						stringstream(dummy) >> view;
						a[accCount-1][17][view-1] = 1;
						count_all[17][view-1]++;
					}
					continue;
				}
				if (counter == 70) //warning devices
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("12"))
							{
								a[accCount-1][18][0] = 1;
								count_all[18][0]++;
							}
						else
							{
								a[accCount-1][18][1] = 1;
								count_all[18][1]++;
							}
					}
					continue;
				}
				if (counter == 75) // public or private
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("Y"))
							{
								grouping[for_grouping] = no_of_lines;
								for_grouping++;
								newfile<<xno<<endl;
								a[accCount-1][19][0] = 1;
								count_all[19][0]++;
							}
						else 
							{
								a[accCount-1][19][1] = 1;
								count_all[19][0]++;
							}
					}
					continue;
				}
				if (counter == 84) // driver age
				{
					if (!dummy.empty())
					{
						int age;
						stringstream(dummy) >> age;
						temp = age/30;
						a[accCount-1][20][temp] = 1;
						count_all[20][temp]++;
					}
					continue;
				}
				if (counter == 85)
				{
					if (!dummy.empty())
					{
						int gen;
						stringstream(dummy) >> gen;
						a[accCount-1][21][gen-1] = 1;
						count_all[21][gen-1]++;
					}
					continue;
				}
		}
	}

//	cout<<accCount<<endl;
//	cin.get();

	for (int i = 0; i < attributes; i++)
		for (int j = 0; j < max_divisions; j++)
		{
			count_all[i][j] = 0;
		}

	int sorted_index[attributes];
	int tie_break[attributes] = {0};
	for (int i = 0; i < 22; i++)
	{
		sorted_index[i] = i;
	}
	int swap, swap2 = 0;
	for (int i = 0 ; i < attributes; i++)
	{
		for (int j = 0; j < attributes; j++)
		{
			if (score[i] > score[j])
			{
				swap = score[i];
				score[i] = score[j];
				score[j] = swap;
				swap2 = sorted_index[i];
				sorted_index[i] = sorted_index[j];
				sorted_index[j] = swap2;
			}
		}
	}
	for (int i = 0; i < attributes; i++)
	{
		tie_break[sorted_index[i]] = i;
	}
	for (int i = 0; i < attributes; i++)
	{
		cout<<i+1<<" | "<<sorted_index[i]+1<<" | "<<score[i]<<endl;
	}

	//FINAL METHOD B BEGINS HERE

	cout<<"Reading from Corridor.txt file"<<endl;
	locationfile.open("SingleOver60Bucks.txt");
	int location_counter = 0;
	while (!locationfile.eof())
	{
		getline(locationfile,xno);
		if (xno.compare("") != 0)
		{
			location[location_counter] = xno;                             //you can go up to a max of 3000. Change index if you want more. 
			location_counter++;
		}
		else
		{
			continue;
		}
	}
	locationfile.close();

	locationfile.open("To Avoid Private.txt");
	string *publicloc;
	publicloc = new string[for_grouping];
	int public_counter = 0;
	while (!locationfile.eof())
	{
		getline(locationfile,xno);
		if (xno.compare("") != 0)
		{
			publicloc[public_counter] = xno;
			public_counter++;
		}
		else
		{
			continue;
		}
	}
	locationfile.close();
	int rel = min(location_counter,public_counter);
	int relevant_loc_counter = 0;
	string *relevant_loc;
	relevant_loc = new string[for_grouping];
	for (int i = 0; i < location_counter; i++)
	{
		for (int j = 0; j < public_counter; j++)
		{
			if (location[i].compare(publicloc[j]) == 0)
			{
				relevant_loc[relevant_loc_counter] = location[i];
				relevant_loc_counter++;
			}
		}
	}

	int priority[attributes] = {0};
	int selected[attributes] = {0};
	int store_in_z = 0, z = 0, m = 0, j = 0, counter = 0, test = 0, large = 0, largest = 0, num_levels = 0;
	int var = 0;     //to check if we reach a breakdown to 1 for the first time. This is relevent in prioritizing
	int q = 0, r = 0, w = 0, i = 0, p = 0;   //just counters for loops

		myfile.clear();
		myfile.seekg(0,myfile.beg);                     //this prevents repeted opening and closing of the same file

		for (q = 0; q < num_accidents; q++)
		{
			for (r = 0; r < attributes; r++)
			{
				for (w = 0; w < max_divisions; w++)
				{
					a[q][r][w] = 0;                      //reinitializing the variables
				}
			}
		}
		accCount = 0;

		//where the file is read and a is filled.
		no_of_lines = 0;
		while (!myfile.eof())    
		{
			no_of_lines++;
			getline(myfile,line);
			int counter = 0;
			istringstream ss(line);
			while (getline(ss,dummy,','))
			{
				counter++;
				if (counter == 16)
				{
					int test = 0;
					for (int l = 0; l < relevant_loc_counter; l++)
					{
						xno = relevant_loc[l];
						if (dummy.compare(xno)!= 0)        //if dummy not in Corridor.txt file, then there is no need to read the rest
						{
							//basically do nothing
						}
						else
						{
							for (int k = 0; k < for_grouping; k++) 
								if (grouping[k] == no_of_lines)
								{
									test = 1;
									break;
								}
						}
					}
					if (test == 1)
					{
						accCount++;
					}
					else 
					{
						break;
					}
				}
				if (counter == 18)   //Month
				{
					if (!dummy.empty())
					{
						int month;
						stringstream(dummy) >> month;
						temp = (month-1)/3;
						a[accCount-1][0][temp] = 1;
						count_all[0][temp]++;
					}
					continue;
				}
				if (counter == 22)   //AMPM
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("AM"))
						{
							a[accCount-1][1][0] = 1;
							count_all[1][0]++;
						}
						else
						{	
							a[accCount-1][1][1] = 1;
							count_all[1][1]++;
						}
					}
					continue;
				}
				if (counter == 30)    //Vehicle Speed
				{
					if (!dummy.empty())
					{
						int vehspd;
						stringstream(dummy) >> vehspd;
						temp = (vehspd)/20;
						a[accCount-1][2][temp] = 1;
						count_all[2][temp]++;
					}
					continue;
				}
				if (counter == 31)    //Type of highway user
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("K"))
						{
							a[accCount-1][3][0] = 1;
							count_all[3][0]++;
						}
						else if (!dummy.compare("A") || !dummy.compare("B") || !dummy.compare("C") || !dummy.compare("D") || !dummy.compare("E") || !dummy.compare("F") || !dummy.compare("G") || !dummy.compare("H") || !dummy.compare("I") || !dummy.compare("J"))
						{
							a[accCount-1][3][1] = 1;
							count_all[3][1]++;
						}
						else
						{
							a[accCount-1][3][2] = 1;
							count_all[3][2]++;
						}
					}
					continue;
				}
				if (counter == 32)   //Highway user direction
				{
					if (!dummy.empty())
					{
						int vehdir;
						stringstream(dummy) >> vehdir;
						a[accCount-1][4][vehdir-1] = 1;
						count_all[4][vehdir-1]++;
					}
					continue;
				}
				if (counter == 33) // Position of vehicle
				{
					if (!dummy.empty())
					{
						int position;
						stringstream(dummy) >> position;
						a[accCount-1][5][position-1] = 1;
						count_all[5][position-1]++;
					}
					continue;
				}
				if (counter == 36)   //Type of Accident
				{
					if (!dummy.empty())
					{
						int typacc;
						stringstream(dummy) >> typacc;
						a[accCount-1][6][typacc-1] = 1;
						count_all[6][typacc-1]++;
					}
					continue;
				}
				if (counter == 39)   //Visibility
				{
					if (!dummy.empty())
					{
						int visiblty;
						stringstream(dummy) >> visiblty;
						a[accCount-1][7][visiblty-1] = 1;
						count_all[7][visiblty-1]++;
					}
					continue;
				}
				if (counter == 40)   //Weather
				{
					if (!dummy.empty())
					{
						int weather;
						stringstream(dummy) >> weather;
						a[accCount-1][8][weather-1] = 1;
						count_all[8][weather-1]++;
					}
					continue;
				}
				if (counter == 42)  //Type of track
				{
					if (!dummy.empty())
					{
						int typtrk;
						stringstream(dummy) >> typtrk;
						a[accCount-1][9][typtrk-1] = 1;
						count_all[9][typtrk-1]++;
					}
					continue;
				}
				if (counter == 44)   //Track Class
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("X"))
						{
							a[accCount-1][10][0] = 1;
							count_all[10][0]++;
							continue;
						}
						int trkclas;
						stringstream(dummy) >> trkclas;
						a[accCount-1][10][trkclas] = 1;
						count_all[10][trkclas]++;
					}
					continue;
				}
				if (counter == 47)   //Train Speed
				{
					if (!dummy.empty())
					{
						int trnspd;
						stringstream(dummy) >> trnspd;
						temp = (trnspd)/20;
						if (trnspd < 60)
						{
							a[accCount-1][11][temp] = 1;
							count_all[11][temp]++;
						}
						else 
						{
							a[accCount-1][11][3] = 1;
							count_all[11][3]++;
						}
					}
					continue;
				}
				if (counter == 49)  //Train Direction
				{
					if (!dummy.empty())
					{
						int trndir;
						stringstream(dummy) >> trndir;
						a[accCount-1][12][trndir-1] = 1;
						count_all[12][trndir-1]++;
					}
					continue;
				}
				if (counter == 51)  //Location of warning device
				{
					if (!dummy.empty())
					{
						int locwarn;
						stringstream(dummy) >> locwarn;
						a[accCount-1][13][locwarn-1] = 1;
						count_all[13][locwarn-1]++;
					}
					continue;
				}
				if (counter == 52) // Warning sign connected to highway signs
				{
					if (!dummy.empty())
					{
						int warnsign;
						stringstream(dummy) >> warnsign;
						a[accCount-1][14][warnsign-1] = 1;
						count_all[14][warnsign-1]++;
					}
					continue;
				}
				if (counter == 53) //lights at crossing
				{
					if (!dummy.empty())
					{
						int lights;
						stringstream(dummy) >> lights;
						a[accCount-1][15][lights-1] = 1;
						count_all[15][lights-1]++;
					}
					continue;
				}
				if (counter == 56) // Action of motorist
				{
					if (!dummy.empty())
					{
						int motorist;
						stringstream(dummy) >> motorist;
						a[accCount-1][16][motorist-1] = 1;
						count_all[16][motorist-1]++;
					}
					continue;
				}
				if (counter == 57)   //view
				{
					if (!dummy.empty())
					{
						int view;
						stringstream(dummy) >> view;
						a[accCount-1][17][view-1] = 1;
						count_all[17][view-1]++;
					}
					continue;
				}
				if (counter == 70) //warning devices
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("12"))
						{
							str_warning_device[accCount-1] = dummy.substr(0,2);
							a[accCount-1][18][0] = 1;
							count_all[18][0]++;
						}
						else if (!dummy.compare("11") || !dummy.compare("10"))
						{
							str_warning_device[accCount-1] = dummy.substr(0,2);
						}
						else
						{
							str_warning_device[accCount-1] = dummy.substr(0,1);
							a[accCount-1][18][1] = 1;
							count_all[18][1]++;
						}
					}
					continue;
				}
				if (counter == 75) // public or private
				{
					if (!dummy.empty())
					{
						if (!dummy.compare("Y"))
						{
							a[accCount-1][19][0] = 1;
							count_all[19][0]++;
						}
						else 
						{
							a[accCount-1][19][1] = 1;
							count_all[19][1]++;
						}
					}
					continue;
				}
				if (counter == 84) // driver age
				{
					if (!dummy.empty())
					{
						int age;
						stringstream(dummy) >> age;
						temp = age/30;
						a[accCount-1][20][temp] = 1;
						count_all[20][temp]++;
					}
					continue;
				}
				if (counter == 85)
				{
					if (!dummy.empty())
					{
						int gen;
						stringstream(dummy) >> gen;
						a[accCount-1][21][gen-1] = 1;
						count_all[21][gen-1]++;
					}
					continue;
				}
			} 
		}

	for (int i = 0; i < attributes; i++)
	{
		for (int j = 0; j < std_arr[i]; j++)
		{
			if (count_all[i][j] > largest_first_level[i])
			{
				largest_first_level[i] = count_all[i][j];
				largest_leaf[i] = j;
			}
		}
	}
	
	cout<<accCount<<endl;

//		cout<<a[22][0][0]<<" | "<<a[22][0][1]<<" | "<<a[22][0][2]<<" | "<<a[22][0][3]<<endl;
//		cin.get();
//prioritizing
		for (p = 0; p < attributes; p++)
		{
			priority[p] = 0;                            //reinitializing the variables
			selected[p] = 0;
		}

		z = 0;
		m = 0;
		j = 0;
		counter = 0;
		test = 0;
		large = 0;
		largest = 0;
		num_levels = 0;
		var = 0;                     //to check if we reach a breakdown to 1 for the first time (not relevant in this code)
		t[0][0].data = accCount;
		t[0][0].node = 0;
		for (i = 0; i < accCount; i++)       //reinitializing the structure variable
		{
			t[0][0].arr[i] = i; 
		}
		for (i = 1; i < attributes; i++)       
		{
			for (p = 0; p < num_accidents; p++)
			{
				t[i][p].data = 0;                    //reinitializing the structure variables
				for (r = 0; r < num_accidents; r++)
				{
					t[i][p].arr[r] = 0;
				}
			}
		}
		for (i = 0; i < attributes; i++)                     
		{																
			tie = -1;
			store_in_z = 0;
			if (i == attributes-1)                  //will not reach here in this code for sure. 
			{
				for (z = 0; z < attributes; z++)
				{
					if (selected[z] == 0)
						{
							priority[i] = z;
							selected[z] = 1;
							break;
						}
				}
			}
			for (m = accCount-1; m > 0; m--)      //Finding the value of m (which corresponds to the index having 0 in next level i+1
			{
				if (t[i+1][m].data != 0)
				{
					m++;
					break;
				}
			}
			largest = 0;

			for (j = 0; j <= attributes; j++)      //look at all unselected attributes to fill out the next level
			{
				for (int x = 0; x < m; x++)               //reinitializing the values
				{
					t[i+1][x].data = 0;
					t[i+1][x].node = 0;
					for (int y = 0; y < num_accidents; y++)
					t[i+1][x].arr[y] = 0;
				}
				m = 0;
				test = 0;
				if (selected[j] == 1)
				{
					continue;
				}
				if (j == attributes)
				{
					j = priority[i];
					selected[j] = 1;
					test = 1;
				}
				large = 0; 
				for (int n = 0; n < std_arr[j]; n++)
				{

					for (int k = 0; k < accCount; k++)
					{
						for (int l = 0; l < accCount; l++)
						{
							if (t[i][z].arr[l] == k)
							{
								if (a[k][j][n] == 1)
								{
									t[i+1][m].data = t[i+1][m].data+1;                          
									t[i+1][m].arr[counter] = k;
									counter++;
								}
								break;
							}
						}
					}
					if (large < t[i+1][m].data)
					{
						large = t[i+1][m].data;
						store_in_z = m;
						max_subcat[i] = n;
					}

					if (t[i+1][m].data != 0)
					{
						m++;
					}
					counter = 0;
					
				}
				if (largest < large)                  //setting up the priority based on the largest number
				{
					priority[i] = j;
					largest = large;
					tie = tie_break[j];
				}
				else if (large == largest)
				{
					if (tie_break[j] < tie)              //using the prioriy that we built earlier. 
					{
						priority[i] = j;
						tie = tie_break[j];
					}
				}
				
				if (test == 1)
				{
					z = store_in_z;              //since we would be splitting the largest value in the next level
//					max_subcat[i] = largest;
					break;
				}
			}

		}  // end of outermost loop while prioritizing. (here I have not found out at what level the largest value turns out as 1

//		                FOR THE MANUAL TREE BUILDING 
/*		priority[0] = 3;
		priority[1] = 16;
		priority[2] = 12;
		priority[3] = 4;
		priority[4] = 6;
		priority[5] = 20;
		priority[6] = 21;
		priority[7] = 8;
		priority[8] = 7;

*/

	for (int i = 0; i < attributes; i++)
	{
		cout<<priority[i]+1<<" | "<<largest_first_level[priority[i]]<<" | leaf = "<<largest_leaf[priority[i]]<<endl;
	}
	//Building the tree
	
	cout<<"starts here"<<endl;
	for (int i = 0; i < attributes; i++)
	{
		for (int j = 0; j < std_arr[priority[i]]; j++)
		{
			if(100*((100*count_all[priority[i]][j]/accCount) - 100*expectation[priority[i]][j])/(100*expectation[priority[i]][j])>50)
					{
						cout<<i+1<<" | "<<priority[i]+1<<" | "<<j<<" | "<<count_all[priority[i]][j]<<" | FLAG | "<<endl;
					}
		}
//		cout<<endl;
	}
	//cin.get();
	//reinitializing the variables
		for (i = 0; i < attributes; i++)
		{
			for (j = 0; j < accCount; j++)
			{
				b[i][j].data = 0;
				for (int k = 0; k < num_accidents; k++)
				{
					b[i][j].arr[k] = -1;
				}
			}
		}

		counter = 0;
//		cout<<"building the tree "<<num_levels<<endl;
		b[0][0].data = accCount;
		b[0][0].node = 0;
		for (int i = 0; i < accCount; i++)
		{
			b[0][0].arr[i] = i; 
		}

		for (int i = 0; i < attributes; i++)
		{
			for (int j = 0; j < accCount; j++)
			{
				if (b[i][j].data == 0)
				{
					break;
				}
				for (int m = accCount-1; m > 0; m--)
				{
					if (b[i+1][m].data != 0)
					{
						m++;
						break;
					}
				}
				if (j == 0)
				{
					m = 0;
				}

				for (int n = 0; n < std_arr[priority[i]]; n++)
				{
					for (int k = 0; k < accCount; k++)
					{
						for (int l = 0; l < accCount; l++)
						{
							if (k == b[i][j].arr[l])
							{

								if (a[k][priority[i]][n] == 1)
								{
									b[i+1][m].data = b[i+1][m].data+1;
									b[i+1][m].parent = j;
									b[i+1][m].arr[counter] = k;           //cout the arr[counter] here and see
									counter++;
								}
							}
						}
					}
					b[i+1][m].node = n;
				
					if (b[i+1][m].data == 0)
					{
						continue;
					}
					if(100*((100*b[i+1][m].data/b[i][j].data) - 100*expectation[priority[i]][n])/(100*expectation[priority[i]][n])>50)
					{
						b[i+1][m].outlier = 1;
					}
					else
					{
						b[i+1][m].outlier = 0;
					}
//					cin.get();

					if (b[i+1][m].data != 0)
					{
						m = m+1;
					}
					counter = 0;
				}
				
			}
		}
//Printing the tree
		
		cout<<"Tree for location at given corridor"<<endl;
		cout<<"Level 0"<<endl<<b[0][0].data<<endl;
		for (int i = 1; i < attributes+1; i++)
		{
			cout<<"Level "<<i<<" attribute used "<<priority[i-1]+1<<endl;
			int counter = 0;
			for (int j = 0; j < accCount; j++)
			{
				if (b[i][j].data == 0)
				{
					continue;
				}
				else 
				{
					cout<<counter<<" pnt = "<<b[i][j].parent<<" leaf = "<<b[i][j].node<<" data = "<<b[i][j].data<<" ";
					if (b[i][j].outlier == 1)
					{
						cout<<"FLAG;";
					}
					counter++;
				}
			}
		cout<<endl;	
		cin.get();
		}

		//Train and Car Direction
		int combination_dir[4][4] = {0};
		for (int i = 0; i < accCount; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (a[i][4][j] == 1)
				{
					for (int k = 0; k < 4; k++)
					{
						if (a[i][12][k] == 1)
						{
							combination_dir[j][k]++;
						}
					}
				}
			}
		}
		cout<<endl;
		cout<<"Table of Train Speed and Vehicle Speed"<<endl;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0 ; j < 4; j++)
			{
				cout<<combination_dir[i][j]<<" | ";
			}
			cout<<endl;
		}

		//Trainspeed and typacc
		int combination_typ_spd[4][2] = {0};
		for (int i = 0; i < accCount; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (a[i][11][j] == 1)
				{
					for (int k = 0; k < 2; k++)
					{
						if (a[i][6][k] == 1)
						{
							combination_typ_spd[j][k]++;
						}
					}
				}
			}
		}

		//Trainspeed and Action of motorist
		int combination_spd_motorist[4][5] = {0};
		for (int i =0; i < accCount; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (a[i][11][j] == 1)
				{
					for (int k = 0; k < 5; k++)
					{
						if (a[i][16][k] == 1)
						{
							combination_spd_motorist[j][k]++;
						}
					}
				}
			}
		}
		//Vehicle Direction and Position and Motorist
		int combination_vehdir_pos[4][4] = {0};
		int combination_vehdir_mot[4][5] = {0};
		for (int i = 0; i < accCount; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (a[i][4][j] == 1)
				{
					for (int k = 0 ; k < 5; k++)
					{
						if (a[i][5][k] == 1)
						{
							combination_vehdir_pos[j][k]++;
						}
						if (a[i][16][k] == 1)
						{
							combination_vehdir_mot[j][k]++;
						}
					}
				}
			}
		}
		cout<<endl;
		cout<<"Table of Train Speed and Type of Accident"<<endl;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0 ; j < 2; j++)
			{
				cout<<combination_typ_spd[i][j]<<" | ";
			}
			cout<<endl;
		}
		cout<<endl;
		cout<<"Table of Train Speed and Motorist"<<endl;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0 ; j < 5; j++)
			{
				cout<<combination_spd_motorist[i][j]<<" | ";
			}
			cout<<endl;
		}
		cout<<endl;
		cout<<"Table of Vehicle Direction and Position"<<endl;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout<<combination_vehdir_pos[i][j]<<" | ";
			}
			cout<<endl;
		}
		cout<<endl;
		cout<<"Table of Vehicle Direction and Position"<<endl;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				cout<<combination_vehdir_mot[i][j]<<" | ";
			}
			cout<<endl;
		}

		int index_warn_dev[num_accidents] = {0};
		int comb_warn_typacc[5][2] = {0};
		int comb_warn_vehspd[5][4] = {0};

		for (int i = 0; i < accCount; i++)
		{
//			cout<<"here "<<warn_device[i]<<endl;
			if (!str_warning_device[i].compare("1"))
			{
				for (int j = 0 ; j < 2; j++)
				{
					if (a[i][6][j] == 1)
					{
						comb_warn_typacc[0][j]++;
					}
				}
				for (int j = 0; j < 4; j++)
				{
					if (a[i][2][j] == 1 && a[i][19][0] == 1)
					{
						comb_warn_vehspd[0][j]++;
					}
				}
			}
			else if (!str_warning_device[i].compare("2") || !str_warning_device[i].compare("3")) //else if (floor(warn_device[i]) == 2 || floor(warn_device[i]) == 3)
			{
				for (int j = 0 ; j < 2; j++)
				{
					if (a[i][6][j] == 1)
					{
						comb_warn_typacc[1][j]++;
					}
				}
				for (int j = 0; j < 4; j++)
				{
					if (a[i][2][j] == 1 && a[i][19][0] == 1)
					{
						comb_warn_vehspd[1][j]++;
					}
				}
			}
			else if (!str_warning_device[i].compare("7"))//else if (floor(warn_device[i]) == 7 )//|| floor(warn_device[i]) == 8)
			{
				for (int j = 0 ; j < 2; j++)
				{
					if (a[i][6][j] == 1)
					{
						comb_warn_typacc[2][j]++;
					}
				}
				for (int j = 0; j < 4; j++)
				{
					if (a[i][2][j] == 1 && a[i][19][0] == 1)
					{
						comb_warn_vehspd[2][j]++;
					}
				}
			}
			else if (!str_warning_device[i].compare("12"))//else if (floor(warn_device[i]) == 12)
			{
				for (int j = 0 ; j < 2; j++)
				{
					if (a[i][6][j] == 1)
					{
						comb_warn_typacc[3][j]++;
					}
				}
				for (int j = 0; j < 4; j++)
				{
					if (a[i][2][j] == 1 && a[i][19][0] == 1)
					{
						comb_warn_vehspd[3][j]++;
					}
				}
			}
			else
			{
				for (int j = 0 ; j < 2; j++)
				{
					if (a[i][6][j] == 1)
					{
						comb_warn_typacc[4][j]++;
					}
				}
				for (int j = 0; j < 4; j++)
				{
					if (a[i][2][j] == 1 && a[i][19][0] == 1)
					{
						comb_warn_vehspd[4][j]++;
					}
				}
			}
		}
		cout<<endl;
		cout<<"Table of Warning Device and Type of Accident"<<endl;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				cout<<comb_warn_typacc[i][j]<<" | ";
			}
			cout<<endl;
		}
		cout<<endl;
		cout<<"Table of Warning Device and Vehicle Speed"<<endl;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout<<comb_warn_vehspd[i][j]<<" | ";
			}
			cout<<endl;
		}
		cin.get();


//	cout<<"here"<<endl;
//	return 0;
}