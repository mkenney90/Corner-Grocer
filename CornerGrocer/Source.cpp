#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int CallFuncOneParam(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;

	return _PyLong_AsInt(presult);
}

// check that user input is valid
void validateUserInput(int& userInput) {

	while (!(cin >> userInput)) {
		cin.clear();
		cin.ignore(123, '\n');
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "\t\t\t" << "Invalid Input. Please Try Again." << endl;
		cout << "\t\t" << "Please select from the options below:" << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "1: List all products and their quantities" << endl;
		cout << "2: Display quantity of specified product" << endl;
		cout << "3: Display graph of purchased items" << endl;
		cout << "4: Quit" << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << endl;

	}

}

// Get user input to run the functions 
void getUserInput(int& userInput) {

	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "\t\t\t" << "Welcome to Corner Grocer!" << endl;
	cout << "\t\t\t" << "Please select and option below:" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "1: List all products and their quantities" << endl;
	cout << "2: Display quantity of specified product" << endl;
	cout << "3: Display graph of purchased items" << endl;
	cout << "4: Quit" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << endl;
	validateUserInput(userInput);
	while (userInput <= 0 && userInput > 4) { // display input menu as long as user doesn't exit
		getUserInput(userInput);
	}

}

// Get user input for item to search in grocery list
void userValue(string& userVal) {
	cout << "Enter an item to view the frequency it was purchased in a day:" << endl; // Print statement asking the user for a specific item
	while (!(cin >> userVal)) {
		cin.clear();
		cin.ignore(123, '\n');
		cout << "Enter an item to view the frequency it was purchased in a day:" << endl; 
	}
}

int main()
{
	// Variables 
	int userInput = 0;
	ifstream inFS;
	string item;
	int quantity;

	// keep running program until user exits
	while (userInput != 4) { 

		getUserInput(userInput); // show the welcome message and first menu
		string userVal;

		if (userInput == 1) {
			CallProcedure("printItemCount");// Call python function to print the purchase amount of each product
		}
		else if (userInput == 2) {
			userValue(userVal);
			cout << endl;
			cout << "The quantity of " << userVal << " sold today is " << CallFuncOneParam("printCount", userVal) <<  "." << endl; // Calls python function to show the specific item amount
			cout << endl;
		}
		else if (userInput == 3) { 
			CallFuncOneParam("createFile", "frequency.dat"); // Calls python function to read and write file
			inFS.open("frequency.dat"); //Opens the frequency.dat file
			if (!inFS.is_open()) { //If statement to check if the file opened
				cout << "Could not open the frequency.dat file." << endl;
				break;
			}
			cout << endl;
			while (!inFS.eof()) { // Create a histogram of purchase data
				inFS >> item;
				inFS >> quantity;
				cout << item << ": ";
				for (int i = 0; i < quantity; ++i) {
					cout << "*";
				}
				cout << endl;
			}
			cout << endl;
			inFS.close(); // close the file
		}
	}
}