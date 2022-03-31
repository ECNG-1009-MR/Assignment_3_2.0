#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

//Function forward declaration
std::vector<std::vector<int>> readfile(int&, int&);
std::vector<std::vector<int>> isNaN(std::vector<std::vector<std::string>>& rawData);
int cleanImage(std::vector<std::vector<std::string>>& Data, int& i, int& j);
void sortvector(std::vector<int>& v);
float findmedian(std::vector<std::vector<int>> v);
void thresholding(std::vector <std::vector<int>>, int, int);
void pgmPrint(std::string filename, std::string MagicNum, std::string comment, int width, int height, int maxGrey, std::vector<std::vector<int>> pixelData);



int main() {

	int width =0, length = 0;
	std::vector<std::vector<int>> processedData = readfile(width, length);
	
	thresholding(processedData, width, length);
	
	return 0;
}

//-------------- QUESTION 1 ----------------

std::vector<std::vector<int>> readfile(int& width, int& length)
{
	std::vector<std::vector<std::string>> rawData; //Creates 2D vector to store values

	std::ifstream readfile;

	readfile.open("imageA3.txt"); //Opens file so it can be read

	if (!readfile.is_open())
		std::cout << "File not found" << std::endl;
	else
	{
		std::cout << "File found" << std::endl;

		int rowNum = 0;

		while ((!readfile.eof()))
		{

			std::string row = "";

			std::getline(readfile, row);

			
			rawData.push_back(std::vector<std::string>());

			std::string dlim = "\t";
			std::string token;
			while (row.find(dlim) != std::string::npos) //Loops until there isn't any more deliminators in the string
			{


				token = row.substr(0, row.find(dlim)); //Token will be equal to the value between the delims
				//std::cout << "token: " << token << std::endl; //Used to make sure the string was being split into its values
				rawData[rowNum].push_back(token);

				row.erase(0, row.find(dlim) + dlim.length()); // Erases the value that was just pushed into the vector from the string
				
			/*	Since the while loop termination condition is when there are no delims reaining in the string,
				then if there is a single value left in the string it won't be pushed back into the vector.
				This if function checks for this termination condition and if it has a value still in the string
				and then equates the token to the string so the last value in the sting can be pushed back into the vector. */

				if (row.find(dlim) == std::string::npos && row.substr(0, row.size()) != "") {
					token = row;
					//std::cout << "token: " << token << std::endl; //Used to make sure the string was being split into its values
					rawData[rowNum].push_back(token);

				}
			}

			rowNum = rowNum + 1;

		}
	}


	//included to find dimensions of the image
	length = rawData.size();
	width = rawData[0].size();

	std::vector<std::vector<int>> cleanData = isNaN(rawData);

	float median = findmedian(cleanData);

	std::stringstream ss;
	ss << "# Median = " << median;
	std::string comment = ss.str();

	pgmPrint("Processed Image.pgm", "P2", comment, width, length, 255, cleanData);

	return cleanData;
}




std::vector<std::vector<int>> isNaN(std::vector<std::vector<std::string>>& rawData)
{

	std::cout << "CLEAN DATA:" << std::endl; 

	std::vector<std::vector<int>> Data;

	for (int i = 0; i < rawData.size(); i++) {

		Data.push_back(std::vector<int>());

		for (int j = 0; j < rawData[i].size(); j++) {


			if (rawData[i][j] == "NaN")
			{
				int avg = cleanImage(rawData, i, j); //If a NaN value is found then the clean function is called
				Data[i].push_back(avg);				//Pushes the calculated clean value into the clean data vector

			}
			else
			{
				Data[i].push_back(std::stoi(rawData[i][j])); //If value is not corrupted then it is converted to an int and pushed back

			}

			std::cout << Data[i][j] << '\t'; 

		}

		std::cout << std::endl;

	}
	return Data;
}



int cleanImage(std::vector<std::vector<std::string>>& rawData, int& i, int& j)
{
	//This function cleans the image by estimating a the corrupted value using the mean filter method

	int sum = 0;
	int count = 0;

	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{						//Gets the position index for each value surrounding the NaN using the two for loops above
			int ypix = i + k;   // ypix| x  x  x
			int xpix = j + l;   //     | x NaN x
								//     | x  x  x
								//      ----------> xpix
			if (ypix >= 0 && ypix < rawData.size() && xpix >= 0 && xpix < rawData[i].size())	//Ensures that each position index falls within the vector size
			{																					//in the event the NaN is at on the edge of the image	
				if (rawData[ypix][xpix] != "NaN")  //Makes sure the value is not a NaN so it could be used in the avg calculation
				{
					sum = stoi(rawData[ypix][xpix]) + sum;
					count++;
				}
			}

		}

	}

	int avg = sum / count;
	return avg;

}



//-------------- QUESTION 2 ----------------


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

float findmedian(std::vector<std::vector<int>> v)
{
	std::vector<int> tempv;
	float median = 0; 
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v[i].size(); j++)
		{
			tempv.push_back(v[i][j]);
		}
	}
	sortvector(tempv);
	float mid = tempv[tempv.size() / 2]; // need one operand to be a float for output to be a float
	if (v.size() % 2 != 0) // checks if odd
	{
		median = mid;
	}
	else if (tempv.size() % 2 == 0) // checks if even
	{
		median = (mid + tempv[(tempv.size() / 2) - 1]) / 2;
	}
	
	std::cout << "median = " << median << std::endl; // needs to be removed just prints median to terminal
	
	return median;
}



//-------------- QUESTION 3 ----------------


void thresholding(std::vector<std::vector<int>> pixels, int width, int length)
{
	int countFreq[4] = { 0, 0, 0, 0 };
	//The 0 index in 'countFreq' corresponds to the frequency of class 1.
	//The 1 index in 'countFreq' corresponds to the frequency of class 2.
	//The 2 index in 'countFreq' corresponds to the frequency of class 3.
	//The 3 index in 'countFreq' corresponds to the frequency of class 4.

	for (int j = 0; j < pixels.size(); j++)
	{
		for (int i = 0; i < pixels[j].size(); i++)
		{
			if (pixels[j][i] >= 192 && pixels[j][i] <= 255)
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
	
	std::stringstream sscomment;

	sscomment << "#" << "Frequency of Classes 1,2,3,4 respectively: ";		//comment
	for (int i = 0; i < 4; i++)
	{
		sscomment << countFreq[i] << " ";
	}

	std::string comment = sscomment.str();

	pgmPrint("Segmented Image.pgm", "P2", comment, width, length, 4, pixels);
	
}

void pgmPrint(std::string filename, std::string magicNum, std::string comment, int width, int height, int maxGrey, std::vector<std::vector<int>> pixelData)
{
	std::ofstream outfile;

	outfile.open(filename);

	outfile << magicNum << "\n";		//pgm type

	outfile << comment << "\n";			//comment
	

	outfile << width << " " << height << "\n";		//dimensions
	
	outfile << maxGrey << "\n";						//maximum grey level

	for (int j = 0; j < pixelData.size(); j++)		//pixel data
	{
		for (int i = 0; i < pixelData[j].size(); i++)
		{
			outfile << pixelData[j][i] << "\t";
		}
		outfile << "\n";
	}

	outfile.close();

}