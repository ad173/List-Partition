#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int getSum(int o, int* s, int start)
//function used to get the sum of a single partition
{
	int sum = 0;
	for (int i = start; i < start + o; i++)
	{
		sum += s[i];
	}
	return sum;
}

int getInequality(int o, int* s, int k, int start)
//function used to get the Inequality of a single partition
{
	int IE = 0;
	int sum = 0;
	sum += getSum(o, s, start);
	IE += (k - sum) * (k-sum);
	return IE;
}

struct Score
	//structure that allows use to keep track of what the current Inequality of a group of partitions are at certain points and how many numbers are in those groups
{
	int score;
	vector<int> list;
};

Score IterativePartitionSolver(int* A, int n, int t)
//Iteratively runs through the array given in reverse and finds the optimal Inequality at index i given all the numbers it has already visited
{
	vector<Score> map; //creates a pointer to an array of scores the size of the array given
	int index = 0;
	Score init;
	for (int i = 0; i < n; i++)
		//initializes our map array of scores to 10000 each
	{
		init.score = 10000;
		map.push_back(init);
	}

	for (int i = n-1; i >= 0; i--)
		//loops through the array in reverse
		//given all the numbers checked from i to n we will set the map score to the lowest possible partition found thus far at index i of the map array
		//the map array is used to memorize the best possible partition found at point i in the array
	{
		int size_of_partition = 0;
		int map_value = 0;
		Score temp;
		if (i < n - 1)
			//sets temp to the best partition found at the index visited before the current index at i
		{
			temp.score = map.at(i + 1).score;
			temp.list = map.at(i + 1).list;
			temp.list.push_back(0);
			index = temp.list.size() - 1;
		}
		while (map_value <= t && i - size_of_partition >= 0)
			//we will go through the array creating a new partition with every next number at i - the size of the current partition
			//with this we will see if we have visited this index i - size_of_partition before
			//if not, we will initialize it to the map array and continue
			//if so, we will compare it to the saved partition and if the Inequality score is lower in the new partition we will replace the old with the new and continue
		{
			map_value += A[i - size_of_partition];
			if (map_value <= t)
			{
				if (temp.list.size() == 0)
				{
					temp.list.push_back(1);
				}
				else
				{
					int num = temp.list.at(index);
					num++;
					temp.list.at(index) = num;
				}
				if (i < n -1)
				{
					int s = 0;
					s = map.at(i + 1).score + getInequality(temp.list.at(temp.list.size() -1), A, t, i - size_of_partition);
					if (s < map.at(i - size_of_partition).score)
					{
						temp.score = s;
						map.at(i - size_of_partition).score = temp.score;
						map.at(i - size_of_partition).list = temp.list;
					}
				}
				else
				{
					temp.score = getInequality(temp.list.at(temp.list.size() - 1), A, t, i - size_of_partition);
					map.at(i - size_of_partition).score = temp.score;
					map.at(i - size_of_partition).list = temp.list;
				}
				size_of_partition++;
			}
		}
		index++;
	}
	return map.at(0);
}

int main()
{
		ifstream infile;
		ofstream outfile;
		Score optimal;
		infile.open("input.txt");
		outfile.open("output.txt");
		if (!infile.is_open())
		{
			cout << "Failed to open file \n";
			cin.get();
			cin.get();
		}

		int instances = 0;
		infile >> instances;
		infile.ignore();
		while (infile.good())
		{
			for (int i = 0; i < instances; i++)
			{
				int n = 0;
				int Max_Cap = 0;
				infile >> n;
				infile >> Max_Cap;
				int* A = new int[n];
				int index = 0;
				while (index < n)
				{
					infile >> A[index];
					index++;
				}
				optimal = IterativePartitionSolver(A, n, Max_Cap);
				outfile << optimal.list.size() << " ";
				for (int i = optimal.list.size() - 1; i >= 0; i--)
				{
					outfile << optimal.list.at(i) << " ";
				}
				outfile << endl;
				delete A;
			}
			break;
		}

		infile.close();
		outfile.close();
}