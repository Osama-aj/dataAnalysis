///////////       Osama Alajam

////all include
#include "pch.h"
#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<chrono>
#include <math.h>
#include <algorithm>


//using namespace std; //not using it anymore

//just a enum to make things clearly
enum sorting
{
	E_byHotestToColdestICountingSort = 0, E_byHotestToColdestIn = 1, E_byHotestToColdestOut, E_byDryestToHumidIn, E_byDryestToHumidOut, E_bymogelRiskInne,
	E_bymogelRiskUte, E_bytempDiff

};

//The main class
class MyClass
{
	//in private there are two structz, three vectorz and and copule oof methds.
private:
	//struct for all data with time, just reading the file and save things in two vectorz
	struct oneDayDataFull
	{
		long int S_datum;
		float S_tempIn, S_tempOut, S_humidityIn, S_humidityOut;
		double S_moldRiskIn, S_moldRiskOut;
		/*oneDayDataFull() {};
		oneDayDataFull(long int datuM, float tempIN, float tempOuT, float humidityIN, float humidityOuT)
			:S_datum(datuM), S_tempIn(tempIN), S_tempOut(tempOuT), S_humidityIn(humidityIN), S_humidityOut(humidityOuT) {};*/
	};
	//struct for data after calculating average 
	struct oneDayData
	{
		long int S_datum, S_time;
		float S_temp, S_humidity;
		oneDayData(long int datum, long int time, float temp, float humidity)
			: S_datum(datum), S_time(time), S_temp(temp), S_humidity(humidity) {};

	};

	std::vector<oneDayData> dataBaseIn, dataBaseOut; //with all information 
	std::vector<oneDayDataFull> average; // just average



	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	//counting sort ... copid from internet with some changes to fit a vector 
	void countSort(std::vector<oneDayDataFull>& arr)
	{
		for (auto& elem : arr)
			elem.S_tempIn = elem.S_tempIn * 100;


		float maxx{ 0 };
		float minn{ 0 };
		for (size_t i = 0; i < arr.size(); i++)
		{
			maxx = std::max(maxx, arr[i].S_tempIn);
			minn = std::min(minn, arr[i].S_tempIn);
		}
		//maxx = (int)maxx;
		//minn = (int)minn;
		float range = maxx - minn + 1;

		std::vector<oneDayDataFull>  output(arr.size());
		std::vector<float> count(range);
		for (int i = 0; i < arr.size(); i++)
			count[arr[i].S_tempIn - minn]++;

		for (int i = 1; i < count.size(); i++)
			count[i] += count[i - 1];

		for (int i = arr.size() - 1; i >= 0; i--)
		{
			output[count[arr[i].S_tempIn - minn] - 1] = arr[i];
			count[arr[i].S_tempIn - minn]--;
		}

		for (int i = 0; i < arr.size(); i++)
			arr[i].S_tempIn = output[i].S_tempIn;

		for (int i = 0; i < arr.size(); i++)
		{
			arr[i] = output[i];
			arr[i].S_tempIn = arr[i].S_tempIn / 100;
		}
	}



	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	//a method which calculate average  
	void toAverage(bool statistics)
	{
		oneDayDataFull toPush; //just an object to push to the vector 
		int counter{ 0 }, counter2{ 0 };
		float tempInSum{ 0 }, tempOutSum{ 0 }, humiInSum{ 0 }, humiOutSum{ 0 };

		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now(); //calculating time 


		///////for-loop to read and save all inside information
		for (unsigned int i = 0; i < (dataBaseIn.size() - 1); i++)
		{
			tempInSum = humiInSum = 0;
			counter = 0;
			while (i < (dataBaseIn.size() - 1) && (dataBaseIn[i].S_datum == dataBaseIn[i + 1].S_datum)) { //if it's the same day and inside the vectors range
				tempInSum += dataBaseIn[i].S_temp;
				humiInSum += dataBaseIn[i].S_humidity;
				counter++;
				i++;
			}
			//adding the last values for the day 
			tempInSum = tempInSum + dataBaseIn[i].S_temp;
			humiInSum += dataBaseIn[i].S_humidity;
			counter++;

			tempInSum /= counter;
			humiInSum /= counter;

			toPush.S_datum = dataBaseIn[i].S_datum;
			toPush.S_tempIn = tempInSum;
			toPush.S_tempOut = 0;
			toPush.S_humidityIn = humiInSum;
			toPush.S_humidityOut = 0;
			//oneDayDataFull toPush(dataBaseIn[i].S_datum, tempInSum, tempOutSum, humiInSum, humiOutSum);
			//double x = (-(0.0015*pow(tempInSum, 3)) + (0.1193 * (pow(tempInSum, 2))) - (2.9878*tempInSum) + (102.96));
			toPush.S_moldRiskIn = (float)humiInSum - (-(0.0015*pow(tempInSum, 3)) + (0.1193 * (pow(tempInSum, 2))) - (2.9878*tempInSum) + (102.96));
			toPush.S_moldRiskOut = 0;
			average.push_back(toPush);

		}


		//////////
		//////////
		//////////

		///////the same for-loop but to read and save all outsides information
		for (unsigned int i = 0; i < (dataBaseOut.size() - 1); i++)
		{
			tempOutSum = humiOutSum = 0;
			counter = 0;
			while (i < (dataBaseOut.size() - 1) && (dataBaseOut[i].S_datum == dataBaseOut[i + 1].S_datum)) {
				tempOutSum += dataBaseOut[i].S_temp;
				humiOutSum += dataBaseOut[i].S_humidity;
				counter++;
				i++;
			}
			tempOutSum += dataBaseOut[i].S_temp;
			humiOutSum += dataBaseOut[i].S_humidity;
			counter++;

			tempOutSum /= counter;
			humiOutSum /= counter;


			average[counter2].S_tempOut = tempOutSum;
			average[counter2].S_humidityOut = humiOutSum;
			average[counter2].S_moldRiskOut = (float)humiOutSum - (-(0.0015*pow(tempOutSum, 3)) + (0.1193 * (pow(tempOutSum, 2))) - (2.9878*tempOutSum) + (102.96));

			counter2++;
		}
		if (statistics) //if the statistics are important
		{
			////calculating time
			std::chrono::duration<long double> diff = std::chrono::system_clock::now() - start;
			std::cout << "Done calculating average in " << diff.count() << " seconds\n";
			std::cout << "There is " << dataBaseIn.size() << " values for indoor\n";
			std::cout << "There is " << dataBaseOut.size() << " values for outdoor\n";
			std::cout << "There is values for " << average.size() << " days\n\n";
			std::cout << "there is days between " << average[0].S_datum << " and " << average[average.size() - 1].S_datum << std::endl;

		}

		//////typing to a file 
		typeToAFile(average, "average.txt", false);
	};


	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////



	//compare funktionz
	struct byDryestToHumidIn  //the compare funktion for humidity indoor 
	{
		inline bool operator() (const oneDayDataFull& struct1, const oneDayDataFull& struct2)
		{
			return (struct1.S_humidityIn < struct2.S_humidityIn);
		}
	};
	struct byDryestToHumidOut //the compare funktion for humidity outdoor
	{
		inline bool operator() (const oneDayDataFull& struct1, const oneDayDataFull& struct2)
		{
			return (struct1.S_humidityOut < struct2.S_humidityOut);
		}
	};


	/////////////////////////
	/////////////////////////
	/////////////////////////

	//binary search using recursive function 
	int binarySearch(std::vector<oneDayDataFull> toSearchIn, int first, int last, long int tofind)
	{
		int  middle = (int)round((first + last) / 2);
		if (middle != first || toSearchIn[middle].S_datum == tofind)
		{
			if (toSearchIn[middle].S_datum == tofind)
				return middle;
			else
			{
				if (toSearchIn[middle].S_datum > tofind)
					return binarySearch(toSearchIn, first, middle, tofind);
				else
					return binarySearch(toSearchIn, middle, last, tofind);
			}
		}
		return -1;
	};


	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	//listing the three first and last values and typing the vector on a file 
	void typeToAFile(std::vector<oneDayDataFull> toType, std::string fileOut, bool coutIt = true)/*coutIt:if you want to see the three first and last values */
	{
		std::string datumStr;
		std::ofstream outFil(fileOut);
		bool done1{ true }, done2{ true };

		if (outFil.is_open())
		{
			for (auto& elem : toType) //for-each loop
			{
				auto i = &elem - &toType[0]; //because i need to use i

				if (((i >= 0 && i <= 2) || (i >= toType.size() - 3)) && coutIt)
				{
					if ((i >= 0 && i <= 2) && done1)
					{
						std::cout << "The first three values\n";
						std::cout << " date \t\t tempIn\ttempOut\thumiIN\thumiOut\tmoldIn\tmoldOut\n";
						done1 = false;
					}
					if ((i >= toType.size() - 3) && done2)
					{
						std::cout << "\nThe last three values\n";
						std::cout << " date \t\t tempIn\ttempOut\thumiIN\thumiOut\tmoldIn\tmoldOut\n";
						done2 = false;
					}
					datumStr = std::to_string(elem.S_datum);
					datumStr.insert(4, "-");
					datumStr.insert(7, "-");
					std::cout
						<< datumStr << "\t"
						<< round(elem.S_tempIn * 100) / 100 << "\t"
						<< round(elem.S_tempOut * 100) / 100 << "\t"
						<< round(elem.S_humidityIn * 100) / 100 << "\t"
						<< round(elem.S_humidityOut * 100) / 100 << "\t"
						<< round(elem.S_moldRiskIn * 100) / 100 << "\t"
						<< round(elem.S_moldRiskOut * 100) / 100 << "\n";
				}

				//this three lines are to add '-' to the date 
				datumStr = std::to_string(elem.S_datum);
				datumStr.insert(4, "-");
				datumStr.insert(7, "-");
				outFil
					<< datumStr << "\t"
					<< round(elem.S_tempIn * 100) / 100 << "\t"
					<< round(elem.S_tempOut * 100) / 100 << "\t"
					<< round(elem.S_humidityIn * 100) / 100 << "\t"
					<< round(elem.S_humidityOut * 100) / 100 << "\t"
					<< round(elem.S_moldRiskIn * 100) / 100 << "\t"
					<< round(elem.S_moldRiskOut * 100) / 100 << "\n";
			}

			//old way of for-loop .... the way with iterator is faster, as i understood
			//for (size_t i = 0; i < toType.size(); i++)
			//{
			//	datumStr = std::to_string(toType[i].S_datum);
			//	datumStr.insert(4, "-");
			//	datumStr.insert(7, "-");
			//	outFil
			//		<< datumStr << "\t"
			//		<< round(toType[i].S_tempIn * 100) / 100 << "\t"
			//		<< round(toType[i].S_tempOut * 100) / 100 << "\t"
			//		<< round(toType[i].S_humidityIn * 100) / 100 << "\t"
			//		<< round(toType[i].S_humidityOut * 100) / 100 << "\t"
			//		<< round(toType[i].S_moldRiskIn * 100) / 100 << "\t"
			//		<< round(toType[i].S_moldRiskOut * 100) / 100 << "\n";
			//}
			outFil.close();

		}
		std::cout << "\n";
	};


	//////////////////////////////////////////
	////////////////merge-temp-up/////////////
	/////////////////////////////////////////
	//merge-sort fuction for temperature indoor
	void merge_sort_by_temp_IN(int low, int high, std::vector<oneDayDataFull>& xxx)
	{

		int mid;
		if (low < high)
		{
			mid = low + (high - low) / 2; //This avoids overflow when low, high are too large
			merge_sort_by_temp_IN(low, mid, xxx); //just divide the vector 
			merge_sort_by_temp_IN(mid + 1, high, xxx); //just divide the vector 
			merge_temp_IN(low, mid, high, xxx);// merge
		}
	}
	void merge_temp_IN(int low, int mid, int high, std::vector<oneDayDataFull>& xx)
	{

		int h, i, j, k;

		std::vector<oneDayDataFull> b(xx.size());
		h = low;
		i = low;
		j = mid + 1;

		while ((h <= mid) && (j <= high))
		{
			if (xx[h].S_tempIn >= xx[j].S_tempIn)
			{
				b.at(i) = xx[h];
				h++;
			}
			else
			{
				b.at(i) = xx[j];
				j++;
			}
			i++;
		}
		if (h > mid)
		{
			for (k = j; k <= high; k++)
			{
				b.at(i) = xx[k];

				i++;
			}
		}
		else
		{
			for (k = h; k <= mid; k++)
			{
				b.at(i) = xx[k];
				i++;
			}
		}
		for (k = low; k <= high; k++) 			xx.at(k) = b[k];
	}




	//////////////////////////////////////////
	////////////////merge-temp-down///////////
	/////////////////////////////////////////

	//merge-sort fuction for temperature outdoor
	void merge_sort_by_temp_OUT(int low, int high, std::vector<oneDayDataFull>& xxx)
	{

		int mid;
		if (low < high)
		{
			mid = low + (high - low) / 2; //This avoids overflow when low, high are too large
			merge_sort_by_temp_OUT(low, mid, xxx);
			merge_sort_by_temp_OUT(mid + 1, high, xxx);
			merge_temp_OUT(low, mid, high, xxx);
		}
	}
	void merge_temp_OUT(int low, int mid, int high, std::vector<oneDayDataFull>& xx)
	{

		int h, i, j, k;

		std::vector<oneDayDataFull> b(xx.size());
		h = low;
		i = low;
		j = mid + 1;

		while ((h <= mid) && (j <= high))
		{
			if (xx[h].S_tempOut >= xx[j].S_tempOut)
			{
				b.at(i) = xx[h];
				h++;
			}
			else
			{
				b.at(i) = xx[j];
				j++;
			}
			i++;
		}
		if (h > mid)
		{
			for (k = j; k <= high; k++)
			{
				b.at(i) = xx[k];

				i++;
			}
		}
		else
		{
			for (k = h; k <= mid; k++)
			{
				b.at(i) = xx[k];
				i++;
			}
		}
		for (k = low; k <= high; k++) 			xx.at(k) = b[k];
	}






	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	//~MyClass() { /*cout << "deco\n";*/ };







public:
	//this piece of code can be a normal method or a constructor 
	//void setAll(bool statistics = true)
	MyClass(bool statistics = true)
	{
		/////deleration
		std::chrono::time_point<std::chrono::system_clock> start;//calculating time
		std::string   tempString;
		bool inOrOut{ false };
		long int date, time;
		float temp, humi;
		std::string fileIn;
		bool done{ true };
		////reading a chosen file 
		std::cout << "Enter the file you want to process >> ";
		while (done)
		{
			std::getline(std::cin, fileIn);
			std::ifstream myfilein(fileIn);
			if (myfilein.is_open())
				done = false;
			else
				std::cout << "File not found, try again >> ";
		}
		////opening the file
		start = std::chrono::system_clock::now();
		std::ifstream myfilein(fileIn);
		if (myfilein.is_open())
		{
			while (!myfilein.eof())//untill the end of the file 
			{
				std::getline(myfilein, tempString, ' ');//from the beginning to the space 
				tempString.erase(4, 1);
				tempString.erase(6, 1);
				date = stol(tempString);

				std::getline(myfilein, tempString, ','); //to the comma 
				tempString.erase(2, 1);
				tempString.erase(4, 1);
				time = stol(tempString);

				std::getline(myfilein, tempString, ',');
				//ute == false
				if (tempString[0] == 'U')
					inOrOut = false;
				else
					inOrOut = true;

				std::getline(myfilein, tempString, ',');
				temp = stof(tempString);//convert to float

				std::getline(myfilein, tempString);
				humi = stof(tempString);//convert to float

				oneDayData  dataBaseToPush(date, time, temp, humi);//gathering information in one variable to push it 

				if (inOrOut) //pushing it back to one of the vectorz
					dataBaseIn.push_back(dataBaseToPush);
				else
					dataBaseOut.push_back(dataBaseToPush);
			}
			myfilein.close();
		}
		else
		{
			std::cout << "Unable to open..." << std::endl;
		}
		if (statistics) //if the statistics are important
		{
			////calculating time
			std::chrono::duration<long double> diff = std::chrono::system_clock::now() - start;
			std::cout << "Done loading the file in " << diff.count() << " seconds\n";
		}
		//calling toAverage method because it is important to have the average values 
		toAverage(statistics);

	};


	void sort(int sortBy)//sort function which include all used sort functions
	{
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		std::vector<oneDayDataFull> tempV(average);
		std::chrono::duration<long double> diff;

		switch (sortBy)
		{
		case E_byHotestToColdestICountingSort:
			countSort(tempV); //counting sort ->code
			diff = std::chrono::system_clock::now() - start; //calculate time
			std::cout << "sorting by temperature by count Sort took " << diff.count() << " seconds\n\n"; 
			typeToAFile(tempV, "TEMP-hTOcIN-BYcountSort.txt"); //listing and typing in a file
			break;
		case E_byHotestToColdestIn:
			merge_sort_by_temp_IN(0, tempV.size() - 1, tempV); //merge ->code
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by temperature by merge-sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "TEMP-hTOcIN-BYmergeSort.txt");
			break;

		case E_byHotestToColdestOut:
			merge_sort_by_temp_OUT(0, tempV.size() - 1, tempV); //merge ->code
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by temperature by merge-sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "TEMP-hTOcOUT-BYmergeSort.txt");
			break;

		case E_byDryestToHumidIn:
			std::stable_sort(tempV.begin(), tempV.end(), byDryestToHumidIn());//stable_sort -> compare function
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by humidity by std::stable_sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "HUMI-dTOhIN-BYstableSort.txt");
			break;

		case E_byDryestToHumidOut:
			std::stable_sort(tempV.begin(), tempV.end(), byDryestToHumidOut());//stable_sort -> compare function
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by humidity by std::stable_sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "HUMI-dTOhOUT-BYstableSort.txt");
			break;

		case E_bymogelRiskInne:
			//std::sort -> lambda
			std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return i.S_moldRiskIn < j.S_moldRiskIn; });
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by mold-risk by std::sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "MOLD-lTOhIN-BYsort.txt");
			break;

		case E_bymogelRiskUte:
			//std::sort -> lambda
			std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return i.S_moldRiskOut < j.S_moldRiskOut; });
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by mold-risk by std::sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "MOLD-lTOhOUT-BYsort.txt");
			break;

		case E_bytempDiff:
			//std::sort -> lambda
			std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return i.S_tempIn - i.S_tempOut > j.S_tempIn - j.S_tempOut; });
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by std::sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "DIFFtEMP-BYsort.txt");
			break;

		default:
			break;
		}


	};



	void search(long int dateToFind) //search method ... using binary search
	{
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		std::chrono::duration<long double> diff;
		int found = binarySearch(average, 0, average.size(), dateToFind);
		diff = std::chrono::system_clock::now() - start;
		if (found == -1)
			std::cout << "NOT found but took " << diff.count() << "seconds.\n\n";
		else //if found -> listing all information in that day
		{
			std::cout << "found and took " << diff.count() << "seconds.\n";
			std::cout << " date \t\t tempIn\ttempOut\thumiIN\thumiOut\tmoldIn\tmoldOut\n";
			std::cout << average[found].S_datum << "\t"
				<< round(average[found].S_tempIn * 100) / 100 << "\t"
				<< round(average[found].S_tempOut * 100) / 100 << "\t"
				<< round(average[found].S_humidityIn * 100) / 100 << "\t"
				<< round(average[found].S_humidityOut * 100) / 100 << "\t"
				//this peice for typing the level of the risk instead of the index
				<< (((round((average[found].S_moldRiskIn * 100) / 100)) <= 0) ? "no risk"
					: ((round((average[found].S_moldRiskIn * 100) / 100)) <= 5) ? "low risk"
					: ((round((average[found].S_moldRiskIn * 100) / 100)) <= 10) ? "medium risk"
					: ((round((average[found].S_moldRiskIn * 100) / 100)) <= 15) ? "high risk"
					: "kaput bye bye hello wrld")
				<< "\t"
				<< (((round((average[found].S_moldRiskOut * 100) / 100)) <= 0) ? "no risk"
					: ((round((average[found].S_moldRiskOut * 100) / 100)) <= 5) ? "low risk"
					: ((round((average[found].S_moldRiskOut * 100) / 100)) <= 10) ? "medium risk"
					: ((round((average[found].S_moldRiskOut * 100) / 100)) <= 15) ? "high risk"
					: "kaput bye bye hello wrld") << "\n\n";
		}
	};

	//to find the meteorological Autumn
	bool meteorologicalAutumn()
	{
		int counter{ 0 };
		size_t start;
		for (size_t i = 0; i < average.size(); i++)// to get the index of the first day in August
			if (std::to_string(average[i].S_datum)[5] == '8')
			{
				start = i;
				break;
			}
		for (size_t i = start; i < average.size(); i++) 
		{

			if (average[i].S_tempOut >= 0 && average[i].S_tempOut <= 10)
				counter++;
			else
				counter = 0;
			if (counter == 5)
			{
				std::string datumStr = std::to_string(average[i - 4].S_datum);
				datumStr.insert(4, "-");
				datumStr.insert(7, "-");
				std::cout << "meteorological Autumn found at : " << datumStr << std::endl << std::endl;
				return true;
			}
		}
		std::cout << "meteorological Autumn not found\n" << std::endl;
		return false;
	};
	//to find the meteorological winter
	bool meteorologicalWinter()
	{
		int counter{ 0 };
		size_t start;
		for (size_t i = 0; i < average.size(); i++)
			if (std::to_string(average[i].S_datum)[5] == '8')
			{
				start = i;
				break;
			}		
		for (size_t i = start; i < average.size(); i++)
		{

			if (average[i].S_tempOut <= 0)
				counter++;
			else
				counter = 0;
			if (counter == 5)
			{
				std::string datumStr = std::to_string(average[i - 4].S_datum);
				datumStr.insert(4, "-");
				datumStr.insert(7, "-");
				std::cout << "meteorological Winter found at : " << datumStr << std::endl << std::endl;
				return true;
			}
		}
		std::cout << "Winter Is Coming, but not yet\n" << std::endl;
		return false;
	};

	//a method to show the range of the days 
	void printrange() { std::cout << "there is days between " << average[0].S_datum << " and " << average[average.size() - 1].S_datum << std::endl; };

};


/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
// a class for the menus
class Lists
{

private:
	MyClass weather;
	int userChoice;
	bool toGo{ true };
	enum e_bigList
	{
		E_dataForchosenDate = 1, E_sort, E_meteoFall, E_meteoWinter, E_Exit, E_WrongInput = 666
	};
	enum e_lists
	{
		E_bigList = 1, E_sortlist, E_sortlistIndoor, E_sortlistOutdoor
	};
	enum e_sortList
	{
		E_temperature = 1, E_humidity, E_moldRisk, E_back
	};
public:
	void init()
	{
		//weather.setAll(true);
		userChoice = E_bigList;
		while (toGo)
		{
			switch (userChoice)
			{
			case E_bigList:
				bigList();
				break;
			case E_sort:
				sortList();
				break;
			case E_sortlistIndoor:
				sortlistIndoor();
				break;
			case E_sortlistOutdoor:
				sortlistOutdoor();
				break;
			default:
				std::cout << "Error nr.1\n";
				userChoice = E_bigList;
				break;
			}
		}
	};


	void bigList()
	{
		std::cout << "welcome to the weather processing app:\n"
			<< ">>1 - To show all values for a chosen date.\n"
			<< ">>2 - To sort by ...\n"
			<< ">>3 - Date of meteorological fall.\n"
			<< ">>4 - Date of meteorological winter.\n"
			<< ">>5 - Exit.\n";


		switch (getUserInput())
		{
		case E_dataForchosenDate:
			weather.printrange();
			std::cout << "Enter the chosen date as, YYYYMMDD : ";
			weather.search(getUserInput());
			userChoice = E_bigList;
			break;
		case E_sort:
			userChoice = E_sortlist;
			break;
		case E_meteoFall:
			weather.meteorologicalAutumn();
			userChoice = E_bigList;
			break;
		case E_meteoWinter:
			weather.meteorologicalWinter();
			userChoice = E_bigList;
			break;
		case E_Exit:
			toGo = false;
			break;

		default:
			std::cout << "Out of range\a\n";
			userChoice = E_bigList;
			break;
		}
	};

	void sortList()
	{
		std::cout << "You want to sort ...\n"
			<< ">>1 - Indoor\n"
			<< ">>2 - Outdoor\n"
			<< ">>3 - By temperatur difference\n"
			<< ">>4 - Back\n";

		switch (getUserInput())
		{
		case 1:
			userChoice = E_sortlistIndoor;
			break;
		case 2:
			userChoice = E_sortlistOutdoor;
			break;
		case 3:
			weather.sort(E_bytempDiff);
			userChoice = E_bigList;
		case 4:
			userChoice = E_bigList;
			break;
		default:
			std::cout << "Out of range.\a\n";
			userChoice = E_sortlist;
			break;
		}

	};
	void sortlistIndoor()
	{
		std::cout << "You want to sort by ...\n"
			<< ">>0 - Temperature by count sort\n"
			<< ">>1 - Temperature\n"
			<< ">>2 - Humidity\n"
			<< ">>3 - Mold risk\n"
			<< ">>4 - Back\n";
		switch (getUserInput())
		{
		case 0:
			weather.sort(0);
			userChoice = E_bigList;
			break;
		case E_temperature:
			weather.sort(E_byHotestToColdestIn);
			userChoice = E_bigList;
			break;
		case E_humidity:
			weather.sort(E_byDryestToHumidIn);
			userChoice = E_bigList;
			break;
		case E_moldRisk:
			weather.sort(E_bymogelRiskInne);
			userChoice = E_bigList;
			break;
		case E_back:
			userChoice = E_sortlist;
			break;
		default:
			std::cout << "Out of range\a\n";
			userChoice = E_sortlistIndoor;
			break;
		}
	};
	void sortlistOutdoor()
	{
		std::cout << "You want to sort by ...\n"
			<< ">>1 - Temperature\n"
			<< ">>2 - Humidity\n"
			<< ">>3 - Mold risk\n"
			<< ">>4 - Back\n";
		switch (getUserInput())
		{
		case E_temperature:
			weather.sort(E_byHotestToColdestOut);
			userChoice = E_bigList;
			break;
		case E_humidity:
			weather.sort(E_byDryestToHumidOut);
			userChoice = E_bigList;
			break;
		case E_moldRisk:
			weather.sort(E_bymogelRiskUte);
			userChoice = E_bigList;
			break;
		case E_back:
			userChoice = E_sortlist;
			break;
		default:
			std::cout << "Out of range\a\n";
			userChoice = E_sortlistOutdoor;
			break;
		}
	};


	int  getUserInput()
	{

		std::string userInputString;
		try
		{
			std::cin >> userInputString;
			userChoice = std::stoi(userInputString);
			system("cls");
			return userChoice;
		}
		catch (const std::exception&)
		{
			std::cout << ("something went wrong,insert only numbers please\n\a");
			return getUserInput();
		}

	};

};

//the main :)
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Lists newList;
	newList.init();
}