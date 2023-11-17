#include <iostream>
#include <BinarySearchTree.h>
#include <functional>
#include "Commands.h"

int main()
{
  std::cout << "Enter 'help' for more information\n";
  ponomarev::Commands commands(std::cin, std::cout, "log.txt");
  ponomarev::AVLTree< std::string, std::function< void() > > commandsMap = {
      {"help", std::bind(&ponomarev::Commands::doHelp, std::ref(commands))},
      {"feed", std::bind(&ponomarev::Commands::doFeed, std::ref(commands))},
      {"show_image", std::bind(&ponomarev::Commands::doShowImage, std::ref(commands))},
      {"view_session", std::bind(&ponomarev::Commands::doViewSession, std::ref(commands))},
      {"assess_last", std::bind(&ponomarev::Commands::doAssessLast, std::ref(commands))},
      {"clear_session", std::bind(&ponomarev::Commands::doClearSession, std::ref(commands))},
      {"show_last", std::bind(&ponomarev::Commands::doShowLast, std::ref(commands))},
      {"feed_percentage", std::bind(&ponomarev::Commands::doFeedPercentage, std::ref(commands))},
      {"slide_show", std::bind(&ponomarev::Commands::doSlideShow, std::ref(commands))}
    };
  while (!std::cin.eof()) {
    std::string command;
    std::cin >> command;
    if (!command.compare("quit")) {
      break;
    }

    auto it = commandsMap.find(command);
    if (it != commandsMap.end()) {
      it->second();
    } else {
      std::cout << "<INVALID COMMAND>\n";
      std::cout << "Type 'help' for more information\n";
    }
  }
  return 0;
}
