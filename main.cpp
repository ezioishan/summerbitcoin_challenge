#include <bits/stdc++.h>
#define ll long long
using namespace std;

class Transaction
{
public:
    string tx_id;
    int fee, weight;
    vector<string> parents;
    Transaction(string id, int cur_fee, int cur_weight, vector<string> cur_parents)
    {
        tx_id = id;
        fee = cur_fee;
        weight = cur_weight;
        parents = cur_parents;
    }
};
/*
tokenize function splits the string with semi-colon(';') delimiter and returns
a vector of such splitted strings
*/
vector<string> tokenize(string line)
{

    vector<string> tokens;

    stringstream check1(line);

    string intermediate;

    while (getline(check1, intermediate, ';'))
    {
        tokens.push_back(intermediate);
    }
    return tokens;
}

/*
readFromFile function reads line by line from the given file specified by the
parameter - 'fileName' and stores all the values in a vector of user-defined
data type(Transactions) called as records
*/
void readFromFile(string fileName, vector<Transaction> &records)
{
    ifstream file;
    file.open(fileName);

    string tx_id, fee, weight, parents;

    int viewCount = 2, iteration = 0;

    while (file.good())
    {
        iteration++;
        getline(file, tx_id, ',');
        getline(file, fee, ',');
        getline(file, weight, ',');
        getline(file, parents, '\n');
        if (iteration == 1)
            continue;
        vector<string> par_vec = tokenize(parents); //semi-colon separated strings
        int cur_fee = stoi(fee);                    //fee is converted from string to integer
        int cur_weight = stoi(weight);              //weight is converted from string to integer
        // if (viewCount-- > 0)
        // {
        //     cout << "TXID: " << tx_id << "\n";
        //     cout << "FEE: " << fee << "\n";
        //     cout << "WEIGHT: " << weight << "\n";
        //     cout << "PARENTS: " << parents << "\n";
        // }
        Transaction temp(tx_id, cur_fee, cur_weight, par_vec);
        records.push_back(temp);
    }

    file.close();
}

/*
writeToFile function writes strings one by one from block and stores it into
the file specified by parameter fileName by creating a file if not already present
or by modifying it
*/
void writeToFile(string fileName, vector<string> block)
{
    ofstream file(fileName);
    for (string txid : block)
    {
        file << txid << "\n";
    }
    file.close();
}

int main()
{
    vector<Transaction> data;
    readFromFile("mempool.csv", data);
    cout << "\n\n";

    //An unordered set to store parent transactions already added to the block
    unordered_set<string> parentsAdded;

    const int MAX_WEIGHT = 4 * 1e6;

    //to keep count of the current weight and fees of the block
    int cur_weight = 0, tot_fees = 0;

    //to store txids in the resultant block
    vector<string> result;

    for (int i = 0; i < data.size(); i++)
    {
        bool ok = true; //boolean to find if anyone parent transaction already found or not

        for (int j = 0; i < data[i].parents.size(); j++)
        {
            //check if parent[j] is already adde to our block, if not then we cannot add this transaction
            //hence ok = FALSE
            if (parentsAdded.find(data[i].parents[j]) == parentsAdded.end())
            {
                ok = false;
                break;
            }
        }

        //Otherwise ok = TRUE which means all parent transactions are already added
        //so we can check the next step i.e. if current weight of the block + weight of current transaction
        //is less than MAX_WEIGHT then add it to the block
        if (ok && cur_weight + data[i].weight <= MAX_WEIGHT)
        {
            cur_weight += data[i].weight;
            tot_fees += data[i].fee;

            result.push_back(data[i].tx_id);
            parentsAdded.insert(data[i].tx_id);
        }
    }

    // cout << "TRANSACTIONS ADDED (TX_ID): \n";
    // for (string s : result)
    // {
    //     cout << s << "\n";
    // }
    cout << "TOTAL WEIGHT = " << cur_weight << ", TOTAL FEES = " << tot_fees << "\n";
    cout << "\n\n";
    writeToFile("block.txt", result);
    return 0;
}
