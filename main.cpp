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
        vector<string> par_vec = tokenize(parents);
        int cur_fee = stoi(fee);
        int cur_weight = stoi(weight);
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
}

int main()
{
    vector<Transaction> data;
    readFromFile("mempool.csv", data);
    cout << "\n\n";
    unordered_set<string> parentsAdded;

    const int MAX_WEIGHT = 4 * 1e6;
    int cur_weight = 0, tot_fees = 0;

    vector<string> result;

    for (int i = 0; i < data.size(); i++)
    {
        bool ok = true;
        for (int j = 0; i < data[i].parents.size(); j++)
        {
            if (parentsAdded.find(data[i].parents[j]) == parentsAdded.end())
            {
                ok = false;
                break;
            }
        }
        if (ok && cur_weight + data[i].weight <= MAX_WEIGHT)
        {
            cur_weight += data[i].weight;
            tot_fees += data[i].fee;

            result.push_back(data[i].tx_id);
            parentsAdded.insert(data[i].tx_id);
        }
    }

    cout << "TRANSACTIONS ADDED (TX_ID): \n";
    for (string s : result)
    {
        cout << s << "\n";
    }
    cout << "TOTAL WEIGHT = " << cur_weight << ", TOTAL FEES = " << tot_fees << "\n";
    cout << "\n\n";
    return 0;
}
