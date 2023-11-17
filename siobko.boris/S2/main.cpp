#include "input.h"
#include "DictionariesManagment.h"

int main(int argc, const char *argv[])
{
  if (argc != 2) {
    std::cerr << "ERROR: invalid amount of argv.";
    return 1;
  }
  const char *filename = argv[1];
  std::ifstream fin(filename);

  std::deque< std::string > dictionariesInfo = siobko::inputTextLinesFromFile(fin);
  std::deque< std::string > commandsInfo = siobko::inputTextLines(std::cin);
  siobko::DictionariesManagment dictionariesManagment;

  try {
    for (const std::string& dictionaryInfo: dictionariesInfo) {
      if (dictionaryInfo.empty()) {
        continue;
      }
      dictionariesManagment.inputDictionary(siobko::splitTextLine(dictionaryInfo, ' '));
    }

    for (const std::string& commandInfo: commandsInfo) {
      if (commandInfo.empty()) {
        continue;
      }
      dictionariesManagment.inputCommand(siobko::splitTextLine(commandInfo, ' '));
    }

    dictionariesManagment.executeCommands();
  } catch (const std::exception& e) {
    std::cerr << e.what();
    return 1;
  }
}
