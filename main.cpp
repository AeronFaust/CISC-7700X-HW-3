#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int NUM_VARIABLES = 20;

struct Data
{
    double variables[NUM_VARIABLES];
    string label;
};

class CMModel
{
    private:
        vector<Data> givenData;
        double modelVariable[NUM_VARIABLES + 1];

    public:
        //Allows data to be added to the private vector givenData
        void addData(const double* variables, const string& label)
        {
            Data data;
            copy(variables, variables + NUM_VARIABLES, data.variables);
            data.label = label;
            givenData.push_back(data);
        }
        
        //Reads data from a csv file as training data
        void loadTrainingData(const string& filename) 
        {
            ifstream file(filename);
            if (!file.is_open())
            {
                cerr << "Unable to open file " << filename << endl;
                return;
            }

            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string token;
                double variables[NUM_VARIABLES];
                string label;

                bool invalidLine = false;//Flag to check invalid line

                for (int i = 0; i < NUM_VARIABLES; ++i) 
                {
                    if (getline(ss, token, ',')) 
                    {
                        try
                        {
                            variables[i] = stod(token);
                        }
                        catch(const invalid_argument& e)
                        {
                            cerr << "warning: Invalid argument in line: " << line << endl;
                            cerr << "Token: " << token << endl;
                            invalidLine = true;
                            break;
                        }
                    } 
                    else 
                    {
                        cerr << "Invalid csv format in line: " << line << endl;
                        return;
                    }
                }

                if (invalidLine)
                {
                    continue;//Skips current line if invalid
                }

                if (getline(ss, label, ',')) 
                {
                    addData(variables, label);
                } 

                else 
                {
                    cerr << "Invalid csv format in line: " << line << endl;
                    return;
                }
            }

            file.close();
        }

        void setVariable(const double* variables)
        {
            copy(variables, variables + NUM_VARIABLES + 1, modelVariable);
        }

        double calculateCosts()
        {
            int costs = 0;

            for (int i = 0; i < givenData.size(); i++)
            {
                double sum = 0;
                for (int j = 0; j < NUM_VARIABLES; j++)
                    sum += givenData[i].variables[j] * modelVariable[j];
                sum += modelVariable[NUM_VARIABLES];

                if (sum > 0)
                {
                    if (givenData[i].label == "1") costs += 0;
                    else costs -= 100;
                } 
                else
                {
                    if (givenData[i].label == "1") costs -= 1000;
                    else costs += 0;
                }  
            }
            
            return costs;
        }
};

int main()
{
    CMModel model;
    double modelValues[NUM_VARIABLES + 1];

    //Load data from csv file as data
    model.loadTrainingData("hw3.data1.csv");

    //Setting initial value of model
    modelValues[0] = 24;
    modelValues[1] = -15;
    modelValues[2] = -38;
    modelValues[3] = -7;
    modelValues[4] = -41;
    modelValues[5] = 35;
    modelValues[6] = 0;
    modelValues[7] = -2;
    modelValues[8] = 19;
    modelValues[9] = 33;
    modelValues[10] = -3;
    modelValues[11] = 7;
    modelValues[12] = 3;
    modelValues[13] = -47;
    modelValues[14] = 26;
    modelValues[15] = 10;
    modelValues[16] = 40;
    modelValues[17] = -1;
    modelValues[18] = 3;
    modelValues[19] = 0;
    modelValues[20] = -6;

    //Sets variables in model
    model.setVariable(modelValues);

    cout << "The cost of running the model is $" << model.calculateCosts() << endl;

    //Test case to find the correlation of the polynomial, what their turning point is and applying the change to the data overall
    double testModelValue[NUM_VARIABLES + 1];

    for (int j = 0; j < NUM_VARIABLES + 1; j++)
    {
        double lowestcost = -1000000000;
        int valueChange = 0;

        for (int k = 0; k < NUM_VARIABLES + 1; k++)
        {
            testModelValue[k] = modelValues[k];
        }
        for (int i = -400; i < 1; i++)
        {
            testModelValue[j] = modelValues[j] + i;

            model.setVariable(testModelValue);
                
            if (model.calculateCosts() > lowestcost)
            {
                lowestcost = model.calculateCosts();
                valueChange = i;
            }
        }
        
        cout << "Value to change for variable " << j << " is " << valueChange << ". The cost of it will be $" << lowestcost << endl;
        modelValues[j] += valueChange;
    } 

    //Sets variables in model
    model.setVariable(modelValues);

    cout << "The final cost of running the model after changes is $" << model.calculateCosts() << endl;
    cout << "The final polynomial is: " << endl;

    for (int j = 0; j < NUM_VARIABLES + 1; j++)
    {
        cout << modelValues[j] << " ";
    }

    return 0;
}