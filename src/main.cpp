#include "html_validator.hpp"
#include "browser_history.hpp"
#include <iostream>
#include <sstream>
#include <vector>

void runValidatorDemo() {
    HtmlValidator validator;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"Valid nested tags", "<div><p>Hello <b>world</b></p></div>"},
        {"Valid with void elements", "<div><img src='a.png'><br><p>text</p></div>"},
        {"Mismatched closing tag", "<div><p></div></p>"},
        {"Unclosed tag", "<div><p>Hello</div>"},
        {"Closing tag with nothing open", "<p>Hello</p></div>"},
    };

    std::cout << "=== HTML Tag Validator (stack-based) ===\n\n";
    for (auto& [label, html] : tests) {
        ValidationResult result = validator.validate(html);
        std::cout << "[" << label << "]\n";
        std::cout << "  Input : " << html << "\n";
        std::cout << "  Result: " << (result.valid ? "VALID" : "INVALID")
                   << " - " << result.message << "\n\n";
    }
}

void runHistoryRepl() {
    std::cout << "=== Browser History (back/forward stacks) ===\n";
    std::cout << "Commands: visit <url> | back | forward | current | exit\n\n";

    BrowserHistory history("home.com");
    std::cout << "Started at: " << history.current() << "\n";

    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line)) break;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        try {
            if (cmd == "visit") {
                std::string url;
                iss >> url;
                if (url.empty()) { std::cout << "Usage: visit <url>\n"; continue; }
                history.visit(url);
                std::cout << "Now at: " << history.current()
                          << " (back depth: " << history.backDepth() << ")\n";
            } else if (cmd == "back") {
                std::cout << "Now at: " << history.back() << "\n";
            } else if (cmd == "forward") {
                std::cout << "Now at: " << history.forward() << "\n";
            } else if (cmd == "current") {
                std::cout << "Current: " << history.current()
                          << " | can go back: " << (history.canGoBack() ? "yes" : "no")
                          << " | can go forward: " << (history.canGoForward() ? "yes" : "no") << "\n";
            } else if (cmd == "exit") {
                break;
            } else if (!cmd.empty()) {
                std::cout << "Unknown command: " << cmd << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    runValidatorDemo();
    runHistoryRepl();
    std::cout << "\nGoodbye.\n";
    return 0;
}
