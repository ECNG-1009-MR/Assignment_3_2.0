#include <vector>
#include <iostream>
#include <algorithm> // used for std::sort function can be removed
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

std::vector<std::vector<int>> readfile();
std::vector<std::vector<int>> isNaN(std::vector<std::vector<std::string>>& rawData);
int cleanImage(std::vector<std::vector<std::string>>& Data, int& i, int& j);
void sortvector(std::vector<int>& v);
void findmedian(std::vector<int> v);
void thresholding(std::vector <std::vector<int>>);



int main() {
	//std::vector<int> v{ 1,3,6,2,5,7,8,12 }; // if vector from part 1 is float need to change function declarations
	/* // needs to be removed just shows that the std sort function and the written function has the same output
		std::sort(v.begin(), v.end());
		for (int i = 0; i < v.size(); i++)
		{
			std::cout << v[i] << std::endl;
		}
	*/
	//findmedian(v); // change v to name of vector

	std::vector<std::vector<int>> processedData = readfile();
	thresholding(processedData);

	return 0;
}


int cleanImage(std::vector<std::vector<std::string>>& rawData, int& i, int& j)
{
	int sum = 0;
	int count = 0;

	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{
			int ypix = i + k;
			int xpix = j + l;

			if (ypix >= 0 && ypix < rawData.size() && xpix >= 0 && xpix < rawData[i].size()) {

				if (rawData[ypix][xpix] != "NaN") {
					sum = stoi(rawData[ypix][xpix]) + sum;
					count++;
				}
			}

		}

	}

	int avg = sum / count;
	return avg;

}

std::vector<std::vector<int>> isNaN(std::vector<std::vector<std::string>>& rawData)
{

	std::vector<std::vector<int>> Data;

	for (int i = 0; i < rawData.size(); i++) {

		Data.push_back(std::vector<int>());

		for (int j = 0; j < rawData[i].size(); j++) {


			if (rawData[i][j] == "NaN")
			{

				int avg = cleanImage(rawData, i, j);
				//int calVal = (std::stoi( rawData[i].at(j - 1) ) + std::stoi( rawData[i].at(j + 1) ) ) / 2;
				Data[i].push_back(avg);

			}
			else
			{
				Data[i].push_back(std::stoi(rawData[i][j]));

			}

			std::cout << Data[i][j] << '\t';

		}

		std::cout << std::endl;

	}
	return Data;
}

std::vector<std::vector<int>> readfile()
{
	std::vector<std::vector<std::string>> rawData;

	std::ifstream readfile;

	readfile.open("imagefile.txt"); //Opens file so it can be read

	if (!readfile.is_open())
		std::cout << "File not found" << std::endl;
	else
	{
		std::cout << "File found" << std::endl;

		int rowNum = 0;

		while (!readfile.eof())
		{

			std::string row = "";

			std::getline(readfile, row);

			std::cout << row << std::endl << std::endl;// Test line to see the vectors 

			rawData.push_back(std::vector<std::string>());

			std::string dlim = "\t";
			std::string token;
			while (row.find(dlim) != std::string::npos) //Loops until there isn't any more deliminators in the string
			{
				//size_t tmp = row.find(dlim);

				token = row.substr(0, row.find(dlim)); //Token will be equal to the value between the delims
				std::cout << "token: " << token << std::endl;
				rawData[rowNum].push_back(token);

				row.erase(0, row.find(dlim) + dlim.length()); // Erases the value that was just pushed into the vector from the string

				//Since the while loop termination condition is when there are no longer any delims, then there is a single value left in the string
				//This if function checks for this termination condition and then equates the token to the string so the last value in the sting can be pushed back into the vector.
				if (row.find(dlim) == std::string::npos) {
					token = row;
					std::cout << "token: " << token << std::endl;
					rawData[rowNum].push_back(token);
				}
			}

			rowNum = rowNum + 1;

		}
	}





	std::vector<int> data = { 1,2 };
	return isNaN(rawData);
}

void sortvector(std::vector<int>& v)
{
	for (int i = 0; i < v.size(); i++) // index to be checked
	{

		for (int j = i + 1; j < v.size(); j++) // starts at index after the one to be checked and goes to the end of the vector
		{
			if (v[j] < v[i])  // only changes the index to be checked if it finds a smaller value than the value at the i index
			{
				int k = 0;
				k = v[i];  // holds the value of the i index at a temporary variable
				v[i] = v[j];  // sets the i index to the new lowest value
				v[j] = k;   // sets the j index to the original value at the i index
			}
		}
	}
}

void findmedian(std::vector<int> v)
{

	int median = 0; // change to float if decimal necessary
	sortvector(v);
	//float mid = v[(v.size() / 2)];    if decimal of median is necessary need to have at least one value in the calculation be a float unnecessary if the vector is a float
	if (v.size() % 2 != 0) // checks if odd
	{
		median = v[v.size() / 2];
	}
	else if (v.size() % 2 == 0) // checks if even
	{
		median = (v[v.size() / 2] + v[(v.size() / 2) - 1]) / 2;
	}
	for (int i = 0; i < v.size(); i++) // just shows the sorted vector needs to be removed
	{
		std::cout << v[i] << std::endl;
	}
	std::cout << "median = " << median; // needs to be removed just prints median to terminal
	std::ofstream output;
	output.open("imagefile.pgm"); // change to name of image file
	output << "# Median = " << median << std::endl;
	output.close();
}

void thresholding(std::vector<std::vector<int>> pixels)
{
	int countFreq[4] = { 0, 0, 0, 0 };
	//The 0 index in 'countFreq' corresponds to the frequency of class 1.
	//The 1 index in 'countFreq' corresponds to the frequency of class 2.
	//The 2 index in 'countFreq' corresponds to the frequency of class 3.
	//The 3 index in 'countFreq' corresponds to the frequency of class 4.

	std::ofstream outfile;
	outfile.open("Segmented.pgm");

	for (int j = 0; j < pixels.size(); j++)
	{
		for (int i = 0; i < pixels[j].size(); i++)
		{
			if (pixels[j][i] >= 192 && pixels[j][i] <= 256)
			{
				pixels[j][i] = 1;
				countFreq[0]++;
			}

			else if (pixels[j][i] >= 128 && pixels[j][i] <= 191)
			{
				pixels[j][i] = 2;
				countFreq[1]++;
			}

			else if (pixels[j][i] >= 64 && pixels[j][i] <= 127)
			{
				pixels[j][i] = 3;
				countFreq[2]++;
			}

			else if (pixels[j][i] >= 0 && pixels[j][i] <= 63)
			{
				pixels[j][i] = 4;
				countFreq[3]++;
			}

			else
				std::cout << "Pixel not within range" << std::endl;
		}
	}
	

	outfile << "P2\n";		//pgm type

	outfile << "#" << "Frequency of Classes 1,2,3,4 respectively: ";		//comment
	for (int i = 0; i < 4; i++)
	{
		outfile << countFreq[i] << " ";
	}

	outfile << "\n" << 16 << " " << 20 << "\n";		//dimensions
	outfile << 4 << "\n";							//maximum grey level

	for (int j = 0; j < pixels.size(); j++)			//pixel data
	{
		for (int i = 0; i < pixels[j].size(); i++)			
		{
			outfile << pixels[j][i] << "\t";
		}
	}
	
	outfile.close();

}