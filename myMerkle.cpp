#include <iostream>
#include <ctime>
#include <list>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class Transaction
{
private:
    string sender;
    string reciver;
    unsigned long money;

public:
    Transaction()
    {
    }

    Transaction(string sender, string reciver, unsigned long money)
    {
        this->sender = sender;
        this->reciver = reciver;
        this->money = money;
    }

    friend class Block;
    friend class myBlock;
};

class Block
{
public:
    int nonse;
    int index;
    unsigned long long prevHash;
    unsigned long long currHash;
    string merkleRoot="";

    time_t timestamp;
    vector<Transaction> t;

    Block()
    {
    }

    Block(int index, unsigned long long prevHash, vector<Transaction> t)
    {
        this->index = index;
        this->prevHash = prevHash;
        this->t = t;
        timestamp = time(0);

        int lb = 1, ub = 9999;
        nonse = (rand() % (ub - lb + 1)) + lb;
        currHash = calculateHash();
        merkleRoot=findMerkleRoot();
    }

    unsigned long long calculateHash()
    {
        string in, nn, ts, ph;
        in = to_string(index);
        nn = to_string(nonse);
        ts = to_string(timestamp);
        ph = to_string(prevHash);

        string whole;
        whole.append(in + nn + ts + ph);

        hash<string> myHash;
        return myHash(whole);
    }

    string findMerkleRoot()
    {
        vector<string> rootHashes;

        for (auto trnx : t)
        {
            rootHashes.push_back(transactionHash(trnx));
        }

        return calculateRoot(rootHashes);
    }

    string transactionHash(Transaction trnx)
    {
        string wholeTr;
        wholeTr=trnx.sender + trnx.reciver + to_string(trnx.money);

        hash<string> merkleHash;
        return to_string(merkleHash(wholeTr)%1000000);
    }

    string calculateRoot(vector<string> rootHashes)
    {
        if(rootHashes.empty())
        {
            return nullptr;
        }

        if(rootHashes.size()==1)
        {
            return rootHashes[0];
        }

        vector<string> newGenratedHash;

        for (int i = 0; i < (rootHashes.size() - 1); i += 2)
        {
            newGenratedHash.push_back((rootHashes[i]+rootHashes[i+1]));
        }

        if (rootHashes.size() % 2 == 1)
        {
            newGenratedHash.push_back((rootHashes.back()+rootHashes.back()));
        }

        return calculateRoot(newGenratedHash);
    }
};

class myBlock
{
    list<Block> blockChain;

public:
    myBlock()
    {
    }

    myBlock(vector<Transaction> &t)
    {
        Block *b = new Block((blockChain.size() + 1), 0, t);
        blockChain.push_back(*b);
    }

    void addBlock(vector<Transaction> &t)
    {
        Block *b = new Block((blockChain.size() + 1), blockChain.back().currHash, t);
        blockChain.push_back(*b);
    }

    void printChain()
    {

        if (!blockChain.size() > 0)
        {
            cout << "Blockchain is not exist";
            return;
        }

        for (auto &block : blockChain)
        {
            Transaction *t;
            cout << "Block #" << block.index << "\n";
            cout << "Timestamp: " << asctime(localtime(&block.timestamp));
            cout << "Previous Hash: " << block.prevHash << "\n";
            cout << "Current Hash: " << block.currHash << "\n";
            cout << "Merkle Root: " << block.merkleRoot << "\n";
            int c = 1;
            
            cout << "\n";
            for (auto tx : block.t)
            {
                cout << "Transaction : " << c << "\n";
                cout << "Sender : " << tx.sender << "\n";
                cout << "Receiver : " << tx.reciver << "\n";
                cout << "Sender : " << tx.money << "\n";
                cout << "\n";
                c += 1;
            }
            cout << "\n";
        }
    }
};

class Mine
{
public:
    void mine()
    {
        Transaction *t;
        bool init = true;

        string sender;
        string reciver;
        unsigned long money;

        int ch;

        do
        {
            cout << "1. Mining  the Block" << endl;
            cout << "2. Print the Blockchain" << endl;
            cout << "0. Stop Mining" << endl;

            cout << "Enter your Choice : ";
            cin >> ch;
            cout << endl;

            vector<Transaction> trnx;

            myBlock *b;

            switch (ch)
            {
                case 1:
                    int ich;

                    do
                    {
                        cout << "1. Add Trasaction" << endl;
                        cout << "0. End Trasaction" << endl;

                        cout << "Enter your Choice : ";
                        cin >> ich;
                        cout << endl;

                        switch (ich)
                        {
                            case 1:                                
                                cout<<"Sender : ";
                                cin>>sender;

                                cout<<"Reciver : ";
                                cin>>reciver;

                                cout<<"Money : ";
                                cin>>money;
                                cout<<endl;

                                t=new Transaction(sender,reciver,money);
                                trnx.push_back(*t);

                                break;
                            case 0:
                                cout<<"Transaction Finished..\n"<<endl;
                            break;

                            default:
                                cout<<"Invalid Choice\n"<<endl;
                                break;
                        }

                    } while (ich != 0);

                    if(init)
                    {
                        b=new myBlock(trnx);
                        init=false;
                    }
                    else
                    {
                        b->addBlock(trnx);
                    }

                    break;
                case 2:
                    b->printChain();
                    break;

                case 0:
                    cout<<"Mining Finished\n"<<endl;
                    break;

                default:
                    cout<<"Invalid Choice\n"<<endl;
                    break;
            }

        } while (ch != 0);
    }
};

int main()
{
    Mine m;

    m.mine();

    return 0;
}
