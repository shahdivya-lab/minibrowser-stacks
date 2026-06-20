#pragma once
#include <string>
#include <stack>
#include <vector>
#include <set>
#include <sstream>
#include <cctype>

struct ValidationResult {
    bool valid;
    std::string message;
};

// Validates that HTML tags are properly opened, closed, and nested --
// the exact same idea as checking matched parentheses "( [ { } ] )",
// just with <tag> / </tag> instead of brackets. A stack is the natural
// fit: push every opening tag, and every closing tag must match
// whatever is currently on top of the stack.
class HtmlValidator {
public:
    HtmlValidator() {
        // "void elements" never have a closing tag in HTML, so they
        // should never be pushed onto the stack.
        voidElements = {"br", "img", "hr", "input", "meta", "link", "area", "base"};
    }

    ValidationResult validate(const std::string& html) const {
        std::stack<std::string> openTags;
        size_t i = 0;
        int tagIndex = 0;

        while (i < html.size()) {
            if (html[i] != '<') { i++; continue; }

            size_t closeBracket = html.find('>', i);
            if (closeBracket == std::string::npos) {
                return {false, "Malformed tag: missing '>' near position " + std::to_string(i)};
            }

            std::string rawTag = html.substr(i + 1, closeBracket - i - 1);
            i = closeBracket + 1;
            tagIndex++;

            bool isClosing = !rawTag.empty() && rawTag[0] == '/';
            bool isSelfClosing = !rawTag.empty() && rawTag.back() == '/';
            if (isClosing) rawTag = rawTag.substr(1);
            if (isSelfClosing) rawTag.pop_back();

            std::string tagName = extractTagName(rawTag);
            if (tagName.empty()) continue;  // skip things like <!-- comments -->

            if (isClosing) {
                if (openTags.empty()) {
                    return {false, "Tag #" + std::to_string(tagIndex) +
                                        ": found closing </" + tagName +
                                        "> but no tag is open"};
                }
                if (openTags.top() != tagName) {
                    return {false, "Tag #" + std::to_string(tagIndex) +
                                        ": expected closing </" + openTags.top() +
                                        "> but found </" + tagName + ">"};
                }
                openTags.pop();
            } else if (!isSelfClosing && voidElements.find(tagName) == voidElements.end()) {
                openTags.push(tagName);
            }
            // self-closing tags and known void elements are not pushed
        }

        if (!openTags.empty()) {
            std::ostringstream oss;
            oss << "Unclosed tag(s) remaining: ";
            while (!openTags.empty()) {
                oss << "<" << openTags.top() << "> ";
                openTags.pop();
            }
            return {false, oss.str()};
        }

        return {true, "Valid: all tags properly opened and closed"};
    }

private:
    std::set<std::string> voidElements;

    static std::string extractTagName(const std::string& rawTag) {
        size_t start = 0;
        while (start < rawTag.size() && std::isspace((unsigned char)rawTag[start])) start++;
        if (start >= rawTag.size() || rawTag[start] == '!') return "";  // comments/doctype

        size_t end = start;
        while (end < rawTag.size() && !std::isspace((unsigned char)rawTag[end])) end++;
        std::string name = rawTag.substr(start, end - start);
        for (auto& c : name) c = (char)std::tolower((unsigned char)c);
        return name;
    }
};
