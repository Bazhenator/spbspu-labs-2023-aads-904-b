#ifndef SHANNON_FANO_CODES_H
#define SHANNON_FANO_CODES_H

#include <iostream>
#include <fstream>
#include "MyDictionary.h"
#include "DialogBox.h"

namespace flusova {

  struct Node {

    Node(char sym, int freq) : symbol_(sym), frequency_(freq), left_(nullptr), right_(nullptr) {}
    bool operator<(const Node& other) const { return frequency_ > other.frequency_; }
    char symbol_;
    int frequency_;
    Node* left_;
    Node* right_;

  };

  class ShannonFano {
  public:

    ShannonFano() : dialogShannon_(std::cout) {}

    void buildEncodingTable(const std::string& data);
    void printEncodingTable(const std::string& message);
    std::string encrypt(const std::string& message);
    std::string decrypt(const std::string& encryptedMessage);
    Node* buildTree(std::vector< Node* >& symbols);
    double calculateEntropy(const std::string& data);
    std::string userInput(std::istream& in);
    std::string readFile(const std::string& filePath);
    void printFanoTree();
    Node* insertNode(Node* node, char symbol, const std::string& code);
    void printTree(Node* node, std::string prefix);

  private:

    flusova::MyDictionary< char, int > calculateFrequencies(const std::string& data);
    void generateEncodingTable(Node* node, const std::string& code);
    template< typename Iterator >
    void sort(Iterator begin, Iterator end);
    void cleanup(Node* node);

    flusova::DialogBox dialogShannon_;
    flusova::MyDictionary< char, std::string > encodingTable_;

  };

  std::string generateRandomKey(int length);
  void writeFile(const std::string& filePath, flusova::ShannonFano& encoder);
  void mergeFiles(const std::string& filePath1, const std::string& filePath2, const std::string& outputFilePath);

}

#endif