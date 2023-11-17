#include "ShannonFanoCodes.h"
#include <random>

void flusova::ShannonFano::buildEncodingTable(const std::string& data) {
  flusova::MyDictionary< char, int > frequencyTable = calculateFrequencies(data);
  std::vector< Node* > symbols;
  for (const auto& entry: frequencyTable) {
    symbols.push_back(new Node(entry.first, entry.second));
  }
  Node* root = buildTree(symbols);
  generateEncodingTable(root, "");
  cleanup(root);
}

void flusova::ShannonFano::printEncodingTable(const std::string& message) {
  buildEncodingTable(message);
  flusova::MyDictionary< char, int > frequencyTable = calculateFrequencies(message);
  double entropy = calculateEntropy(message);
  std::cout << "Encoding Table:" << '\n';
  std::cout << "Symbol\tFrequency\tEntropy\tCode" << '\n';
  for (const auto& entry: encodingTable_) {
    char symbol = entry.first;
    int frequency = frequencyTable[symbol];
    double symbolEntropy = (frequency * log2(frequency) / entropy);
    std::string code = entry.second;
    std::cout << symbol << "\t" << frequency << "\t\t" << symbolEntropy << "\t" << code << '\n';
  }
}

std::string flusova::ShannonFano::encrypt(const std::string& message) {
  std::string encryptedMessage;
  for (char c: message) {
    if (encodingTable_.find(c) != encodingTable_.end()) {
      encryptedMessage += encodingTable_[c];
    }
  }
  return encryptedMessage;
}

std::string flusova::ShannonFano::decrypt(const std::string& encryptedMessage) {
  std::string decryptedMessage;
  std::string currentCode;
  for (char bit: encryptedMessage) {
    currentCode += bit;
    for (const auto& entry: encodingTable_) {
      if (entry.second == currentCode) {
        decryptedMessage += entry.first;
        currentCode = "";
        break;
      }
    }
  }
  return decryptedMessage;
}

flusova::Node* flusova::ShannonFano::buildTree(std::vector< Node* >& symbols) {
  sort(symbols.begin(), symbols.end());
  if (symbols.size() == 1) {
    Node* root = new Node('\0', symbols[0]->frequency_);
    root->left_ = symbols[0];
    return root;
  }
  int totalFrequency = 0;
  for (Node* symbol: symbols) {
    totalFrequency += symbol->frequency_;
  }
  int currentFrequency = 0;
  int splitIndex = -1;
  int minDifference = totalFrequency;
  for (int i = 0; i < symbols.size(); ++i) {
    currentFrequency += symbols[i]->frequency_;
    int difference = abs(totalFrequency - 2 * currentFrequency);
    if (difference < minDifference) {
      minDifference = difference;
      splitIndex = i;
    }
  }
  Node* root = new Node('\0', totalFrequency);
  std::vector< Node* > leftSymbols(symbols.begin(), symbols.begin() + splitIndex + 1);
  std::vector< Node* > rightSymbols(symbols.begin() + splitIndex + 1, symbols.end());
  root->left_ = buildTree(leftSymbols);
  root->right_ = buildTree(rightSymbols);
  return root;
}

double flusova::ShannonFano::calculateEntropy(const std::string& data) {
  flusova::MyDictionary< char, int > frequencyTable = calculateFrequencies(data);
  double entropy = 0.0;
  int totalSymbols = data.length();
  for (const auto& entry: frequencyTable) {
    double probability = static_cast<double>(entry.second) / totalSymbols;
    entropy += probability * log2(1.0 / probability);
  }
  return entropy;
}

std::string flusova::ShannonFano::userInput(std::istream& in) {
  std::string message = "";
  dialogShannon_.printGetMessage();
  std::getline(in, message);
  if (message.empty()) {
    dialogShannon_.printEmptyInput();
  }
  return message;
}

std::string flusova::ShannonFano::readFile(const std::string& filePath) {
  std::ifstream file(filePath);
  std::string fileContents;
  std::string line;
  if (file.is_open()) {
    while (getline(file, line)) {
      fileContents += line + "\n";
    }
    file.close();
  }
  if (fileContents.empty()) {
    dialogShannon_.printEmptyInput();
  }
  return fileContents;
}

void flusova::ShannonFano::printFanoTree() {
  Node* root = nullptr;
  for (const auto& entry : encodingTable_) {
    root = insertNode(root, entry.first, entry.second);
  }
  std::cout << "Encoding Tree:" << '\n';
  printTree(root, "");
  cleanup(root);
}

flusova::Node* flusova::ShannonFano::insertNode(Node* node, char symbol, const std::string& code) {
  if (node == nullptr) {
    node = new Node(symbol, 0);
  }
  if (code.empty()) {
    node->symbol_ = symbol;
    return node;
  }
  if (code[0] == '0') {
    node->left_ = insertNode(node->left_, symbol, code.substr(1));
  } else {
    node->right_ = insertNode(node->right_, symbol, code.substr(1));
  }
  return node;
}

void flusova::ShannonFano::printTree(Node* node, std::string prefix) {
  if (node == nullptr) {
    return;
  }
  if (node->left_ == nullptr && node->right_ == nullptr) {
    std::cout << prefix << "Symbol: " << node->symbol_ << " | Code: " << encodingTable_[node->symbol_] << '\n';
    return;
  }
  std::cout << prefix << "Internal Node" << '\n';
  std::cout << prefix << "  0" << '\n';
  printTree(node->left_, prefix + "  |");
  std::cout << prefix << "  1" << '\n';
  printTree(node->right_, prefix + "  |");
}

flusova::MyDictionary< char, int > flusova::ShannonFano::calculateFrequencies(const std::string& data) {
  flusova::MyDictionary< char, int > frequencyTable;
  for (char c: data) {
    frequencyTable[c]++;
  }
  return frequencyTable;
}

void flusova::ShannonFano::generateEncodingTable(Node* node, const std::string& code) {
  if (node == nullptr) {
    return;
  }
  if (node->symbol_ != '\0') {
    encodingTable_[node->symbol_] = code;
  }
  generateEncodingTable(node->left_, code + '0');
  generateEncodingTable(node->right_, code + '1');
}

template< typename Iterator >
void flusova::ShannonFano::sort(Iterator begin, Iterator end) {
  for (Iterator i = begin; i != end; ++i) {
    Iterator j = i;
    while (j != begin && *(j - 1) > *j) {
      std::swap(*(j - 1), *j);
      --j;
    }
  }
}

void flusova::ShannonFano::cleanup(flusova::Node* node) {
  if (node == nullptr) {
    return;
  }
  cleanup(node->left_);
  cleanup(node->right_);
  delete node;
}

std::string flusova::generateRandomKey(int length) {
  std::string key;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution< > dis('A', 'Z');
  for (int i = 0; i < length; ++i) {
    key += static_cast<char>(dis(gen));
  }
  return key;
}

void flusova::writeFile(const std::string& filePath, flusova::ShannonFano& encoder) {
  std::ofstream outputFile(filePath);
  if (outputFile.is_open()) {
    std::streambuf* originalOutput = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());
    encoder.printEncodingTable("your_message");
    std::cout.rdbuf(originalOutput);
    outputFile.close();
    std::cout << "Encoding table has been written to the file: " << filePath << std::endl;
  } else {
    std::cerr << "Unable to open the file: " << filePath << std::endl;
  }
}

void flusova::mergeFiles(const std::string& filePath1, const std::string& filePath2, const std::string& outputFilePath) {
  std::ifstream file1(filePath1);
  std::ifstream file2(filePath2);
  std::ofstream outputFile(outputFilePath);
  if (file1.is_open() && file2.is_open() && outputFile.is_open()) {
    outputFile << file1.rdbuf();
    outputFile << file2.rdbuf();

    std::cout << "Files merged successfully. Merged content has been written to the file: " << outputFilePath << std::endl;
  } else {
    std::cerr << "Unable to open one or more input files or create the output file." << std::endl;
  }
  file1.close();
  file2.close();
  outputFile.close();
}