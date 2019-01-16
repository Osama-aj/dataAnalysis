

#include "pch.h"
#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<chrono>
#include <math.h>
#include <algorithm>


//using namespace std;
enum sorting
{
	E_byHotestToColdestIn = 1, E_byHotestToColdestOut, E_byDryestToHumidIn, E_byDryestToHumidOut, E_bymogelRiskInne,
	E_bymogelRiskUte, E_bytempDiff

};

class MyClass
{

private:

	struct oneDayDataFull
	{
		long int S_datum;
		float S_tempIn, S_tempOut, S_humidityIn, S_humidityOut;
		double S_moldRiskIn, S_moldRiskOut;
		/*oneDayDataFull() {};
		oneDayDataFull(long int datuM, float tempIN, float tempOuT, float humidityIN, float humidityOuT)
			:S_datum(datuM), S_tempIn(tempIN), S_tempOut(tempOuT), S_humidityIn(humidityIN), S_humidityOut(humidityOuT) {};*/
	};
	struct oneDayData
	{
		long int S_datum, S_time;
		float S_temp, S_humidity;
		oneDayData(long int datum, long int time, float temp, float humidity)
			: S_datum(datum), S_time(time), S_temp(temp), S_humidity(humidity) {};

	};

	std::vector<oneDayData> dataBaseIn, dataBaseOut;
	std::vector<oneDayDataFull> average;





	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	void toAverage(bool statistics)
	{
		oneDayDataFull toPush;
		int counter{ 0 }, counter2{ 0 };
		float tempInSum{ 0 }, tempOutSum{ 0 }, humiInSum{ 0 }, humiOutSum{ 0 };

		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();


		//oneDayDataFull toPush;
		///////for-loop for inside
		for (unsigned int i = 0; i < (dataBaseIn.size() - 1); i++)
		{
			tempInSum = humiInSum = 0;
			counter = 0;
			while (i < (dataBaseIn.size() - 1) && (dataBaseIn[i].S_datum == dataBaseIn[i + 1].S_datum)) {
				tempInSum += dataBaseIn[i].S_temp;
				humiInSum += dataBaseIn[i].S_humidity;
				counter++;
				i++;
			}
			tempInSum += dataBaseIn[i].S_temp;
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

		///////for-loop for outside
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
		if (statistics)
		{
			////calculating time
			std::chrono::duration<long double> diff = std::chrono::system_clock::now() - start;
			std::cout << "Done calculating average in " << diff.count() << " seconds\n";
			std::cout << "There is " << dataBaseIn.size() << " values for indoor\n";
			std::cout << "There is " << dataBaseOut.size() << " values for outdoor\n";
			std::cout << "There is vlaues for " << average.size() << " days\n\n";

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

	struct byDryestToHumidIn
	{
		inline bool operator() (const oneDayDataFull& struct1, const oneDayDataFull& struct2)
		{
			return (struct1.S_humidityIn < struct2.S_humidityIn);
		}
	};
	struct byDryestToHumidOut
	{
		inline bool operator() (const oneDayDataFull& struct1, const oneDayDataFull& struct2)
		{
			return (struct1.S_humidityOut < struct2.S_humidityOut);
		}
	};


	/////////////////////////
	/////////////////////////
	/////////////////////////

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

	void typeToAFile(std::vector<oneDayDataFull> toType, std::string fileOut, bool coutIt = true)
	{
		std::string datumStr;
		std::ofstream outFil(fileOut);
		bool done1{ true }, done2{ true };

		if (outFil.is_open())
		{
			for (auto& elem : toType)
			{
				auto i = &elem - &toType[0];

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

			//old way of for-loop
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
	void merge_sort_by_temp_IN(int low, int high, std::vector<oneDayDataFull>& xxx)
	{

		int mid;
		if (low < high)
		{
			mid = low + (high - low) / 2; //This avoids overflow when low, high are too large
			merge_sort_by_temp_IN(low, mid, xxx);
			merge_sort_by_temp_IN(mid + 1, high, xxx);
			merge_temp_IN(low, mid, high, xxx);
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
				b.at(i).S_tempIn = xx[h].S_tempIn;
				h++;
			}
			else
			{
				b.at(i).S_tempIn = xx[j].S_tempIn;
				j++;
			}
			i++;
		}
		if (h > mid)
		{
			for (k = j; k <= high; k++)
			{
				b.at(i).S_tempIn = xx[k].S_tempIn;

				i++;
			}
		}
		else
		{
			for (k = h; k <= mid; k++)
			{
				b.at(i).S_tempIn = xx[k].S_tempIn;
				i++;
			}
		}
		for (k = low; k <= high; k++) 			xx.at(k).S_tempIn = b[k].S_tempIn;
	}




	//////////////////////////////////////////
	////////////////merge-temp-down///////////
	/////////////////////////////////////////
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
				b.at(i).S_tempOut = xx[h].S_tempOut;
				h++;
			}
			else
			{
				b.at(i).S_tempOut = xx[j].S_tempOut;
				j++;
			}
			i++;
		}
		if (h > mid)
		{
			for (k = j; k <= high; k++)
			{
				b.at(i).S_tempOut = xx[k].S_tempOut;

				i++;
			}
		}
		else
		{
			for (k = h; k <= mid; k++)
			{
				b.at(i).S_tempOut = xx[k].S_tempOut;
				i++;
			}
		}
		for (k = low; k <= high; k++) 			xx.at(k).S_tempOut = b[k].S_tempOut;
	}






	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	//~MyClass() { /*cout << "deco\n";*/ };







public:
	//denna metod ska vara en vanligt metod eller konstruktör
	//void setAll(bool statistics = true)
	MyClass(bool statistics = true)
	{
		/////deleration
		//oneDayData  dataBaseToPush;
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		std::string   tempString;
		bool inOrOut{ false };
		long int date, time;
		float temp, humi;
		std::string fileIn;
		bool done{ true };
		////reading right file
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
		std::ifstream myfilein(fileIn);
		if (myfilein.is_open())
		{
			while (!myfilein.eof())
			{
				std::getline(myfilein, tempString, ' ');
				tempString.erase(4, 1);
				tempString.erase(6, 1);
				date = stol(tempString);

				std::getline(myfilein, tempString, ',');
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
				temp = stof(tempString);

				std::getline(myfilein, tempString);
				humi = stof(tempString);

				oneDayData  dataBaseToPush(date, time, temp, humi);

				if (inOrOut)
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
		if (statistics)
		{
			////calculating time
			std::chrono::duration<long double> diff = std::chrono::system_clock::now() - start;
			std::cout << "Done loading the file in " << diff.count() << " seconds\n";
		}
		//calling toAverage method 
		toAverage(statistics);

	};


	void sort(int sortBy)
	{
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		std::vector<oneDayDataFull> tempV(average);
		std::chrono::duration<long double> diff;

		switch (sortBy)
		{
		case E_byHotestToColdestIn:
			//std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return (i.S_tempIn > j.S_tempIn); });
			merge_sort_by_temp_IN(0, tempV.size() - 1, tempV);
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by temperature by merge-sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "TEMP-hTOcIN-BYsort.txt");
			break;

		case E_byHotestToColdestOut:
			merge_sort_by_temp_OUT(0, tempV.size() - 1, tempV);
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by temperature by merge-sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "TEMP-hTOcOUT-BYsort.txt");
			break;

		case E_byDryestToHumidIn:
			std::stable_sort(tempV.begin(), tempV.end(), byDryestToHumidIn());
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by humidity by std::stable_sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "HUMI-dTOhIN-BYsort.txt");
			break;

		case E_byDryestToHumidOut:
			std::stable_sort(tempV.begin(), tempV.end(), byDryestToHumidOut());
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by humidity by std::stable_sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "HUMI-dTOhOUT-BYsort.txt");
			break;

		case E_bymogelRiskInne:
			std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return i.S_moldRiskIn < j.S_moldRiskIn; });
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by mold-risk by std::sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "MOLD-lTOhIN-BYsort.txt");
			break;

		case E_bymogelRiskUte:
			std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return i.S_moldRiskOut < j.S_moldRiskOut; });
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by mold-risk by std::sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "MOLD-lTOhOUT-BYsort.txt");
			break;

		case E_bytempDiff:
			std::sort(tempV.begin(), tempV.end(), [](const oneDayDataFull& i, const oneDayDataFull& j) {return i.S_tempIn - i.S_tempOut > j.S_tempIn - j.S_tempOut; });
			diff = std::chrono::system_clock::now() - start;
			std::cout << "sorting by std::sort took " << diff.count() << " seconds\n\n";
			typeToAFile(tempV, "DIFFtEMP-BYsort.txt");
			break;

		default:
			break;
		}


	};



	void search(long int dateToFind)
	{
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		std::chrono::duration<long double> diff;
		int found = binarySearch(average, 0, average.size(), dateToFind);
		diff = std::chrono::system_clock::now() - start;
		if (found == -1)
			std::cout << "NOT found but took " << diff.count() << "seconds.\n\n";
		else
		{
			std::cout << "found and took " << diff.count() << "seconds.\n";
			std::cout << " date \t\t tempIn\ttempOut\thumiIN\thumiOut\tmoldIn\tmoldOut\n";
			std::cout << average[found].S_datum << "\t"
				<< round(average[found].S_tempIn * 100) / 100 << "\t"
				<< round(average[found].S_tempOut * 100) / 100 << "\t"
				<< round(average[found].S_humidityIn * 100) / 100 << "\t"
				<< round(average[found].S_humidityOut * 100) / 100 << "\t"
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


	bool meteorologicalAutumn()
	{
		int counter{ 0 };
		//for (auto it = average.begin(); it !=average.end(); it++)
		for (size_t i = 0; i < average.size(); i++)
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

	bool meteorologicalWinter()
	{
		int counter{ 0 };
		//for (auto it = average.begin(); it !=average.end(); it++)
		for (size_t i = 0; i < average.size(); i++)
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
		std::cout << "meteorological Winter not found\n" << std::endl;
		return false;
	};


};


/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
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
		std::cout << "welcome to the weathe processing app:\n"
			<< ">>1 - To show all values for a chosen date.\n"
			<< ">>2 - To sort by ...\n"
			<< ">>3 - Date of meteorological fall.\n"
			<< ">>4 - Date of meteorological winter.\n"
			<< ">>5 - Exit.\n";


		switch (getUserInput())
		{
		case E_dataForchosenDate:
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
			<< ">>1 - Temperature\n"
			<< ">>2 - Humidity\n"
			<< ">>3 - Mold risk\n"
			<< ">>4 - Back\n";
		switch (getUserInput())
		{
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


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Lists newList;
	newList.init();
}