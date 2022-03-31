#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <functional>


//function used to replace a string, with another string
std::string do_replace(std::string const& in, std::string const& from, std::string const& to)
{
    return std::regex_replace(in, std::regex(from), to);
}



std::vector<std::string> openFile(std::string input)
{
    std::ifstream infile;
    std::vector<std::string> data;
    std::vector<std::string> cleandata;
    std::string stringData;
    infile.open(input);

    //getline removes the issue of new lines in the text file
    while (getline(infile, stringData))
    {
        data.push_back(stringData);
    }

    //Every delimiter is replace with a space ' ', in order to push all values to a vector later on
    //More conditions can be added to cater for different delimiters
    for (int i = 0; i < data.size(); i++)
    {
        //Replaces all double spaces with 1 space
        data[i] = do_replace(data[i], "  ", " ");

        //Replaces all triple spaces with 1 space
        data[i] = do_replace(data[i], "   ", " ");

        //Removes all spaces at the beginning of lines
        data[i].erase(data[i].begin(), std::find_if(data[i].begin(), data[i].end(), std::bind1st(std::not_equal_to<char>(), ' ')));
    }


    infile.close();

    return data;
}

std::vector<std::vector<int>> columns(std::vector<std::string> data, int width, int length)
{
    std::string temp;
    std::stringstream sstemp;
    std::vector<std::string> oneVec;
    std::vector<int> tempVec;
    std::vector<std::vector<int>> finalVec;

    //Since all values are now delimited by a space " ", this is used to push ALL data into 1 vecotr 
    for (int i = 0; i < data.size(); i++)
    {
        sstemp.str(data[i]);
        while (!getline(sstemp, temp, ' ').eof())
        {
            oneVec.push_back(temp);
        }
        sstemp.clear();     //string stream cleared each time to get next line of data
    }


    //Splits the vector into a 2d vector, where each 1d vector inside corresponds to a row pixels
    for (int i = 0; i < oneVec.size(); i++)
    {
        
        if ((i+1) % width != 0)     //This expression splits the long vector into sections of what ever the image width is
        {
            std::cout << oneVec[i] << ": "<< i << std::endl;
            tempVec.push_back(stoi(oneVec[i]));
        }
        else
        {
            tempVec.push_back(stoi(oneVec[i]));
            finalVec.push_back(tempVec);
            tempVec.clear();
        }
        
        
    }


    return finalVec;

}

void outputFile(std::string output, std::vector<std::vector<int>> final)
{
    std::ofstream outfile;
    outfile.open(output);


    //Same loop from assignment 3 code to output text file in rows and columns
    for (int j = 0; j < final.size(); j++)
    {
        for (int i = 0; i < final[j].size(); i++)
        {
            outfile << final[j][i] << "\t";
        }

        outfile << "\n";
    }

    outfile.close();

}



int main()
{
    std::vector<std::vector<int>> final;
    final = columns(openFile("tracks.txt"), 300, 200);

    outputFile("tracks_fixed.txt", final);

    return 0;
}
