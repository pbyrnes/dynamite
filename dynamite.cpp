// dynamite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

const int numGames=100000;

int _tmain(int argc, _TCHAR* argv[])
{
	int p1=1, p2=1, p3=1000, p4=1000;

	srand(time(NULL));

	int gameNum=0;
	int numWins[4];
	for(int i=0; i<4; i++)
		numWins[i]=0;
	int numRolls[numGames];
	for(int i=0; i<numGames; i++)
		numRolls[i]=0;
	while(gameNum<numGames)
	{
		bool playable[20];
		for(int i=0; i<20; i++)
			if(i%5!=0)
				playable[i]=false;
			else
				playable[i]=true;
		int position[4]={0,5,10,15};
		int currPlayer=-1;
		bool winner=false;
		int rollNum=0;
		while(!winner)
		{
			currPlayer=(currPlayer+1)%4;
			rollNum++;
			//roll die
			int r = (rand() % 6);
			if(r>2)
			{
				//blow a bridge
				int rr=rand()%(p1+p2+p3+p4);
				int b=0;
				if(rr<p1)
					b=0;
				else
					if(rr<p1+p2)
						b=1;
					else
						if(rr<p1+p2+p3)
							b=2;
						else
							b=3;
				for(int i=b*5+1; i<b*5+5; i++)
					playable[i]=false;
				for(int i=0; i<4; i++)
					if(!playable[position[i]])
						position[i]=5*(position[i]/5);
			}
			else
			{
				//build bridges and move
				bool finished=false;
				int finishSpot=(currPlayer*5);
				for(int i=0; i<=r; i++)
				{
					bool moved = false;
					for(int j=1; j<20 && !moved; j++)
					{
						int p=(position[currPlayer]+j)%20;
						if(p==finishSpot)
						{
							winner=true;
							r=0;
							numRolls[gameNum]=rollNum;
							numWins[currPlayer]++;
						//	cout << "game " << gameNum << " won by player " << currPlayer << endl;
							moved=true;
							gameNum++;
						}
						if(!playable[p])
						{
							playable[p]=true;
							int lastSpot=p;
							for(int k=1; k<20; k++)
							{
								if(playable[(p+k)%20])
									lastSpot=(p+k)%20;
								else
									k=20;
								if((p+k)%20==finishSpot)
									k=20;
							}
							if(lastSpot==finishSpot)
							{
								winner=true;
								r=0;
								numRolls[gameNum]=rollNum;
								numWins[currPlayer]++;
					//			cout << "game " << gameNum << " won by player " << currPlayer << endl;
								moved=true;
								gameNum++;
							}
							else
							{
								//move player
								int end=p;
								while(playable[(end+1)%20])
									end=(end+1)%20;
								position[currPlayer]=end;
								moved=true;
							}
						}
					}
				}
			}
		}
	}

	int maxNumRolls=0;
	for(int i=0; i<numGames; i++)
		if(numRolls[i]>maxNumRolls)
			maxNumRolls=numRolls[i];


	int* rollCount = new int[maxNumRolls+1];
	for(int i=0; i<maxNumRolls+1; i++)
		rollCount[i]=0;
	for(int i=0;i<numGames; i++)
	{
	//	cout << numRolls[i] << endl;
		rollCount[numRolls[i]]++;
	}

	ofstream fout;
	fout.open("out.txt");
//	cout << endl;
	for(int i=0; i<maxNumRolls+1; i++)
		fout << i << " rolls: " << rollCount[i] << endl;
	fout << endl;
	for(int i=0; i<4; i++)
		fout << numWins[i] << endl;
	fout.close();

	delete[] rollCount;

	for(int i=0; i<4; i++)
		cout << numWins[i] << endl;
	char a;
	cin >> a;
	return 0;
}

