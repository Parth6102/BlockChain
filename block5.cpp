#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

struct Transaction
{
  string sender;
  string receiver;
  double amount;

  // public:
  //   Transaction(string &sender, string &receiver, double amount) : sender(sender), receiver(receiver), amount(amount) {}
  //   friend class Block;
  //   friend class Blockchain;
};

class Block
{
  int index;
  unsigned int prevHash;
  time_t timestamp;
  vector<Transaction> transaction;
  string merkleRoot;
  unsigned int hash;

  unsigned int calculateHash()
  {
    stringstream ss;
    ss << index << prevHash << timestamp;
    for (const auto &txn : transaction)
    {
      ss << txn.sender << txn.receiver << txn.amount;
    }

    string hashStr = ss.str();
    const int prime = 31;
    unsigned int hashVal = 0;
    for (char ch : hashStr)
    {
      hashVal = hashVal * prime + static_cast<int>(ch);
    }
    return hashVal;
  }

  string calculateMerkleRoot()
  {
    vector<string> transactionHashes;
    for (const auto &txn : transaction)
    {
      transactionHashes.push_back(calculateHashOfTransaction(txn));
    }

    return calculateMerkleRootHelper(transactionHashes);
  }

  string calculateHashOfTransaction(const Transaction &txn)
  {
    stringstream ss;
    ss << txn.sender << txn.receiver << txn.amount;

    const int prime = 31;
    string hashStr = ss.str();
    unsigned int hashVal = 0;

    for (char ch : hashStr)
    {
      hashVal = hashVal * prime + static_cast<unsigned int>(ch);
    }

    return to_string(hashVal);
  }

  string calculateMerkleRootHelper(const vector<string> &hashes)
  {
    if (hashes.empty())
    {
      return 0;
    }

    if (hashes.size() == 1)
    {
      return hashes[0];
    }

    vector<string> newHashes;
    for (auto i = 0; i < hashes.size() - 1; i += 2)
    {
      stringstream ss;
      ss << hashes[i] << hashes[i + 1];
      // cout << hashes[i] << " " << hashes[i + 1] << " " << ss.str() << endl;
      newHashes.push_back(calculateHashOfPair(hashes[i], hashes[i + 1]));
    }

    if (hashes.size() % 2 == 1)
    {
      newHashes.push_back(calculateHashOfPair(hashes.back(), hashes.back()));
      // newHashes.push_back(hashes.back());
    }

    return calculateMerkleRootHelper(newHashes);
  }

  string calculateHashOfPair(string hash1, string hash2)
  {
    return hash1 + hash2;
  }

public:
  Block(int index, unsigned int prevHash, const vector<Transaction> &txns) : index(index), prevHash(prevHash), transaction(txns)
  {
    timestamp = time(nullptr);
    hash = calculateHash();
    merkleRoot = calculateMerkleRoot();
  }

  friend class Blockchain;
};

class Blockchain
{
private:
  list<Block> chain;

public:
  Blockchain()
  {
    vector<Transaction> txns{{"", "", 0.0}};
    chain.emplace_back(0, 0, txns);
  }

  void addBlock(const vector<Transaction> &transactions)
  {
    const Block &prevBlock = chain.back();
    chain.emplace_back(chain.size(), prevBlock.hash, transactions);
  }

  void printChain()
  {
    for (const auto &block : chain)
    {
      cout << "Block #" << block.index << endl;
      cout << "Timestamp: " << asctime(localtime(&block.timestamp));
      cout << "Previous Hash: " << block.prevHash << endl;
      cout << "Hash: " << block.hash << endl;
      cout << "Merkle Root: " << block.merkleRoot << endl; // Print Merkle root
      cout << "Transactions: " << endl;
      for (const auto &txn : block.transaction)
      {
        cout << "  Sender: " << txn.sender << ", Receiver: " << txn.receiver << ", Amount: " << txn.amount << endl;
      }
      cout << endl;
    }
  }
};

int main()
{
  Blockchain blockchain;

  blockchain.addBlock({{"John", "Broke", 2.5}, {"Tej", "Roman", 5.2}});
  blockchain.addBlock({{"Dominic", "Brain", 5.0}, {"Hun", "Letty", 5.5}, {"Jim", "Kirti", 5.5},{"arr", "ds", 5.0}, {"efsfg", "dfsd", 5.5}, {"sdfs", "eaee", 5.5}});

  blockchain.printChain();
  return 0;
}